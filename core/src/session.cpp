#include "og/core/session.h"
#include "og/core/schema.h"
#include "og/core/where_condition.h"
#include "og/core/exception.h"
#include "og/core/mapping.h"
#include "og/core/schema_sql.h"
#include "og/core/xml_stream.h"
#include "og/core/serializer.h"
#include "og/core/where_condition.h"
#include "og/core/fetcher.h"

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

#include <sqlite3/soci-sqlite3.h>

namespace og
{
namespace core
{

using namespace boost::property_tree;
using namespace boost::property_tree::xml_parser;
namespace pt = boost::posix_time;
namespace fs = boost::filesystem;

session::session(void) :
  soci_session_(new soci::session()),
  schema_(new schema())
{
}

session::~session()
{
}

void session::build()
{
  set_foreign_key(false);

  vector<string> sql_commands;
  boost::algorithm::split(sql_commands, schema_sql,
                          boost::algorithm::is_any_of(";"));

  for (vector<string>::const_iterator it = sql_commands.begin();
       it != sql_commands.end(); ++it)
  {
    string ddl(*it);
    boost::trim(ddl);

    if (ddl.length() != 0)
    {
      OG_LOG << "DDL:" << ddl;
      *(soci_session_.get()) << ddl;
    }
  }

  // enabled
  set_foreign_key(true);

  // check seq initial value
  initialize_sqlite_sequence("schema_object_seq");
  initialize_sqlite_sequence("schema_relation_seq");
  initialize_sqlite_sequence("schema_parameter_seq");
  initialize_sqlite_sequence("session_object_seq");
  initialize_sqlite_sequence("session_relation_seq");
}

void session::set_foreign_key(bool _enable)
{
  if (_enable)
  {
    *(soci_session_.get()) << "PRAGMA foreign_keys = true";
  }
  else
  {
    *(soci_session_.get()) << "PRAGMA foreign_keys = false";
  }
}

void session::initialize_sqlite_sequence(string _tbl)
{
  int count;
  *(soci_session_.get()) << "SELECT COUNT(*) FROM " << _tbl,
                         soci::into(count);

  if (count == 0)
  {
    *(soci_session_.get()) << "INSERT INTO " << _tbl << " VALUES(0)";
  }
}

void session::open(string _connection_string)
{
  soci_session_->open(soci::sqlite3, _connection_string);

  // enable foreign key
  set_foreign_key(true);

  // set session
  schema_->initialize(this);
}
void session::close()
{
  soci_session_->close();
}

void session::purge(bool _delete_schema_property_object)
{
  list<session_relation_ptr> rels;
  get_relation_by_query(where_clause<string>("1=1"), &rels);

  {
    transaction tran(*this);
    for (list<session_relation_ptr>::iterator it = rels.begin(); it != rels.end();
         ++it)
    {
      it->get()->delete_relation();
    }
    tran.commit();
  }

  list<session_object_ptr> objs;
  get_object_by_query(where_clause<string>("1=1"), &objs);

  {
    transaction tran(*this);
    for (list<session_object_ptr>::iterator it = objs.begin(); it != objs.end();
         ++it)
    {
      if (it->get()->get_type().compare(schema::schema_property_object_type_) == 0 &&
          !_delete_schema_property_object)
      {
        continue;
      }
      else
      {
        it->get()->delete_object();
      }
    }
    tran.commit();
  }

  /*
  (*soci_session_) << "DELETE FROM session_relation_parameter_basetype_integer;";
  (*soci_session_) << "DELETE FROM session_relation_parameter_basetype_real;";
  (*soci_session_) << "DELETE FROM session_relation_parameter_basetype_text;";
  (*soci_session_) << "DELETE FROM session_relation;";
  (*soci_session_) << "DELETE FROM session_object_parameter_basetype_integer;";
  (*soci_session_) << "DELETE FROM session_object_parameter_basetype_real;";
  (*soci_session_) << "DELETE FROM session_object_parameter_basetype_text;";
  (*soci_session_) << "DELETE FROM session_object;";
  */
}

void session::get_sequence(string* _id)
{
  //create sequence
  int id_seed;
  *(soci_session_.get()) << "INSERT INTO session_object_seq VALUES(NULL)";
  *(soci_session_.get()) << "SELECT MAX(id_seed) FROM session_object_seq",
                         soci::into(id_seed);
  *_id = string(SESSION_OBJECT_ID_PREFIX + boost::lexical_cast<string>
                (id_seed));
}

session_object_ptr session::create_object(schema_object_ptr _schm_obj)
{
  string id;
  get_sequence(&id);

  //create object
  session_object_ptr sesn_obj(new session_object(this, id, _schm_obj));

  // set date
  pt::ptime now = pt::microsec_clock::local_time();
  sesn_obj->set_create_date(pt::to_simple_string(now));
  sesn_obj->set_update_date(pt::to_simple_string(now));

  // set revision
  sesn_obj->set_revision(REVISION_INIT_VALUE);

  //insert record
  insert_object(sesn_obj);

  list<boost::tuple<string, schema_parameter_ptr>> param_name_types;
  _schm_obj->get_parameters(&param_name_types);

  for (list<boost::tuple<string, schema_parameter_ptr>>::iterator pit
       = param_name_types.begin(); pit != param_name_types.end(); pit++)
  {
    insert_object_parameter_by_default(sesn_obj, *pit);
  }

  return sesn_obj;
}

bool session::import_object(session_object_ptr _sesn_obj,
                            const ptree& _param_elm)
{
  transaction tran(*this);

  // declare insert target and current schema's target
  map<string, list<parameter_value_variant>> params_file;

  // get xml values
  import_parameter(_param_elm, &params_file);

  //insert record
  insert_object(_sesn_obj);

  // declare and get current parameters
  list<boost::tuple<string, schema_parameter_ptr>> param_name_types;
  _sesn_obj->get_schema_object()->get_parameters(&param_name_types);

  for (list<boost::tuple<string, schema_parameter_ptr>>::iterator pit =
         param_name_types.begin();
       pit != param_name_types.end(); pit++)
  {
    map<string, list<parameter_value_variant>>::iterator fit =
        params_file.find(pit->get<0>());

    if (fit == params_file.end())
    {
      // not found. parameter may be created as new.
      insert_object_parameter_by_default(_sesn_obj, *pit);
    }
    else
    {
      // fonud.
      insert_object_parameter_with_arg(_sesn_obj, *pit, fit->second);
    }
  }

  tran.commit();
  return true;
}

bool session::import_relation(session_relation_ptr _sesn_rel,
                              const ptree& _param_elm)
{
  transaction tran(*this);

  // declare insert target
  map<string, list<parameter_value_variant>> params;

  // get xml values
  import_parameter(_param_elm, &params);

  // check revision up or not
  //int schema_rev = boost::lexical_cast<int>
  //                 (_sesn_rel->get_schema_relation()->get_revision());
  //int session_rev = boost::lexical_cast<int>
  //                  (_sesn_rel->get_revision());

  //if (schema_rev > session_rev)
  //{
  //}
  //else
  //{

  //}

  //insert record
  insert_relation(_sesn_rel);

  list<boost::tuple<string, schema_parameter_ptr>> param_name_types;
  _sesn_rel->get_schema_relation()->get_parameters(&param_name_types);

  for (list<boost::tuple<string, schema_parameter_ptr>>::iterator pit
       = param_name_types.begin(); pit != param_name_types.end(); pit++)
  {
    map<string, list<parameter_value_variant>>::iterator fit =
        params.find(pit->get<0>());

    if (fit == params.end())
    {
      // not found. parameter may be created as new.
      insert_relation_parameter_by_default(_sesn_rel, *pit);
    }
    else
    {
      // fonud.
      insert_relation_parameter_with_arg(_sesn_rel, *pit, fit->second);
    }

  }

  tran.commit();
  return true;
}

void session::import_parameter(const ptree& _param_elm,
                               map<string, list<parameter_value_variant>>* _params)
{
  BOOST_FOREACH(const ptree::value_type& pt, _param_elm.get_child("parameters"))
  {
    optional<string> param_name =
      pt.second.get_optional<string>("<xmlattr>.param_name");
    optional<int> basetype = pt.second.get_optional<int>("<xmlattr>.basetype");

    list<parameter_value_variant> values;
    //	const  ptree& item_elm = pt.second.get_child("items");

    switch ((parameter_basetype_enum)basetype.get())
    {

    case parameter_basetype_enum::integer:

      BOOST_FOREACH(const ptree::value_type& it,
                    pt.second.get_child("items"))
      {
        optional<int> index = it.second.get_optional<int>("<xmlattr>.index");
        optional<int> value = it.second.get_optional<int>("<xmlattr>.value");

        // store variant
        values.push_back(parameter_value_variant(value.get()));
      }

      break;

    case parameter_basetype_enum::real:

      BOOST_FOREACH(const ptree::value_type& it,
                    pt.second.get_child("items"))
      {
        optional<int> index = it.second.get_optional<int>("<xmlattr>.index");
        optional<double> value = it.second.get_optional<double>("<xmlattr>.value");

        // store variant
        values.push_back(parameter_value_variant(value.get()));
      }

      break;

    case parameter_basetype_enum::text:

      BOOST_FOREACH(const ptree::value_type& it,
                    pt.second.get_child("items"))
      {
        optional<int> index = it.second.get_optional<int>("<xmlattr>.index");
        optional<string> value = it.second.get_optional<string>("<xmlattr>.value");

        // store variant
        values.push_back(parameter_value_variant(value.get()));
      }

      break;

    }

    // add xml values to map
    _params->insert(std::make_pair(param_name.get(), values));
  }
}


optional<session_object_ptr> session::get_object(string _id)
{
  session_object_ptr sesn_obj(new session_object(this));
  *(soci_session_.get())
      <<
      "SELECT * FROM session_object WHERE id_ = :id_",
      soci::into(*(sesn_obj.get())),
      soci::use(_id);

  if (sesn_obj->get_id().empty())
  {
    // no id found
    return optional<session_object_ptr>();
  }

  // parameter
  map<string, session_parameter_ptr>* param_map(
    sesn_obj->get_parameters());

  list<boost::tuple<string, schema_parameter_ptr>> param_name_types;
  sesn_obj->get_schema_object()->get_parameters(&param_name_types);

  for (list<boost::tuple<string, schema_parameter_ptr>>::iterator pit
       = param_name_types.begin(); pit != param_name_types.end(); pit++)
  {
    // insert target
    list<parameter_value_variant> var_value_list;

    switch ((parameter_basetype_enum)boost::get<1>(*pit)->get_basetype())
    {
    case parameter_basetype_enum::integer:
    {
      list<int> value_list;

      fetcher<int>::fetch_use2(
        *soci_session_
        , "SELECT value FROM session_object_parameter_basetype_integer "
        "WHERE oid = :oid AND param_name = :param_name "
        "ORDER BY array_index"
        , soci::use(_id)
        , soci::use(pit->get<0>())
        , &value_list);
      // convert
      parameter_value_converter<int>().convert_from_list_to_variant_list
      (value_list, &var_value_list);
    }
    break;

    case parameter_basetype_enum::real:
    {
      list<double> value_list;

      fetcher<double>::fetch_use2(
        *soci_session_
        , "SELECT value FROM session_object_parameter_basetype_real "
        "WHERE oid = :oid AND param_name = :param_name "
        "ORDER BY array_index"
        , soci::use(_id)
        , soci::use(pit->get<0>())
        , &value_list);
      // convert
      parameter_value_converter<double>().convert_from_list_to_variant_list
      (value_list, &var_value_list);
    }
    break;

    case parameter_basetype_enum::text:
    {
      list<string> value_list;

      fetcher<string>::fetch_use2(
        *soci_session_
        , "SELECT value FROM session_object_parameter_basetype_text "
        "WHERE oid = :oid AND param_name = :param_name "
        "ORDER BY array_index"
        , soci::use(_id)
        , soci::use(pit->get<0>())
        , &value_list);
      // convert
      parameter_value_converter<string>().convert_from_list_to_variant_list
      (value_list, &var_value_list);
    }
    break;
    }

    // register
    param_map->insert(
      std::make_pair(pit->get<0>(),
                     session_parameter_ptr(new session_parameter(var_value_list, pit->get<1>()))));
  }

  return optional<session_object_ptr>(sesn_obj);
}

void session::get_object(list<session_object_ptr>* _sesn_obj_list)
{
  _sesn_obj_list->clear();

  get_object_by_query(where_clause<string>("1=1"),
                      _sesn_obj_list);
}

void session::get_object(list<string>& _oid_list,
                         list<session_object_ptr>* _sesn_obj_list)
{
  _sesn_obj_list->clear();

  for (list<string>::iterator it = _oid_list.begin(); it != _oid_list.end(); ++it)
  {
    optional<session_object_ptr> ptr = get_object(*it);
    if (ptr.is_initialized())
    {
      _sesn_obj_list->push_back(ptr.get());
    }
  }
}

void session::get_object_relation(list<string>& _oid_list,
                                  list<string>& _rel_list,
                                  list<boost::tuple<session_object_ptr, session_relation_ptr>>* _sesn_objrel_list)
{
  _sesn_objrel_list->clear();

  list<string>::iterator it = _oid_list.begin();
  list<string>::iterator it2 = _rel_list.begin();
  for (; it != _oid_list.end(); ++it, ++it2)
  {
    optional<session_object_ptr> ptr = get_object(*it);
    optional<session_relation_ptr> ptr_rel = get_relation(*it2);

    if (ptr.is_initialized() && ptr_rel.is_initialized())
    {
      _sesn_objrel_list->push_back(
        boost::make_tuple<session_object_ptr, session_relation_ptr>(ptr.get(),
            ptr_rel.get()));
    }
  }
}

void session::delete_object(string _id,
                            map<string, session_parameter_ptr>* _param_map)
{
  for (map<string, session_parameter_ptr>::iterator it = _param_map->begin();
       it != _param_map->end(); ++it)
  {
    parameter_basetype_enum t =
      schema_parameter::convert_to_parameter_basetype_enum(
        it->second->schema_parameter_->get_basetype());

    switch (t)
    {
    case parameter_basetype_enum::integer:

      *soci_session_
          << "DELETE FROM session_object_parameter_basetype_integer "
          "WHERE oid = :oid AND param_name = :param_name "
          , soci::use(_id)
          , soci::use(it->first);
      break;

    case parameter_basetype_enum::real:
      *soci_session_
          << "DELETE FROM session_object_parameter_basetype_real "
          "WHERE oid = :oid AND param_name = :param_name "
          , soci::use(_id)
          , soci::use(it->first);
      break;

    case parameter_basetype_enum::text:
      *soci_session_
          << "DELETE FROM session_object_parameter_basetype_text "
          "WHERE oid = :oid AND param_name = :param_name "
          , soci::use(_id)
          , soci::use(it->first);
      break;

    case parameter_basetype_enum::select:
      *soci_session_
          << "DELETE FROM session_object_parameter_basetype_select "
          "WHERE oid = :oid AND param_name = :param_name "
          , soci::use(_id)
          , soci::use(it->first);
      break;
    }
  }

  // delete from db
  *(soci_session_.get())
      <<
      "DELETE FROM session_object WHERE id_ = :id_",
      soci::use(_id);
}

session_object_ptr session::copy_object(schema_object_ptr _schm_obj,
                                        map<string, session_parameter_ptr>* _param_map)
{
  session_object_ptr created(create_object(_schm_obj));

  list<boost::tuple<string, schema_parameter_ptr>> param_name_types;
  created->get_schema_object()->get_parameters(&param_name_types);

  for (list<boost::tuple<string, schema_parameter_ptr>>::iterator fit =
         param_name_types.begin();
       fit != param_name_types.end(); fit++)
  {
    map<string, session_parameter_ptr>::iterator it = _param_map->find(
          boost::get<0>(*fit));
    if (it != _param_map->end())
    {
      insert_object_parameter_with_arg(created, *fit, it->second->values_);
    }
  }

  return created;
}

void session::copy_relation(session_relation_ptr _sesn_rel,
                            map<string, session_parameter_ptr>* _param_map)
{
  list<boost::tuple<string, schema_parameter_ptr>> param_name_types;
  _sesn_rel->get_schema_relation()->get_parameters(&param_name_types);

  for (list<boost::tuple<string, schema_parameter_ptr>>::iterator fit =
         param_name_types.begin();
       fit != param_name_types.end(); fit++)
  {
    map<string, session_parameter_ptr>::iterator it = _param_map->find(
          boost::get<0>(*fit));
    if (it != _param_map->end())
    {
      insert_relation_parameter_with_arg(_sesn_rel, *fit, it->second->values_);
    }
  }
}

void session::get_object_by_type(list<string>& _otype_list,
                                 list<session_object_ptr>* _sesn_obj_list)
{
  get_object_by_query(where_condition<string>(_otype_list, "schema_object.type"),
                      _sesn_obj_list);
}

void session::get_object_by_name(list<string>& _oname_list,
                                 list<session_object_ptr>* _sesn_obj_list)
{
  get_object_by_query(where_condition<string>(_oname_list, "session_object.name"),
                      _sesn_obj_list);
}

void session::get_object_by_schema_object_name(list<string>& _oname_list,
    list<session_object_ptr>* _sesn_obj_list)
{
  get_object_by_query(where_condition<string>(_oname_list, "schema_object.name"),
                      _sesn_obj_list);
}

void session::get_object_by_schema_object(list<schema_object_ptr>&
    _schm_obj_list,
    list<session_object_ptr>* _sesn_obj_list)
{
  get_object_by_query(where_condition<schema_object_ptr>(_schm_obj_list,
                      "session_object.schema_object_id")
                      , _sesn_obj_list);
}


// relation ===>
session_relation_ptr session::connect(string _from_id, string _to_id,
                                      schema_relation_ptr _schm_rel)
{
  //create sequence
  int id_seed;
  *(soci_session_.get()) << "INSERT INTO session_relation_seq VALUES(NULL)";
  *(soci_session_.get()) << "SELECT MAX(id_seed) FROM session_relation_seq",
                         soci::into(id_seed);
  string id(SESSION_RELATION_ID_PREFIX + boost::lexical_cast<string>(id_seed));

  //create object
  session_relation_ptr sesn_rel(new session_relation(this, id, _schm_rel));

  // set date
  pt::ptime now = pt::microsec_clock::local_time();
  sesn_rel->set_create_date(pt::to_simple_string(now));
  sesn_rel->set_update_date(pt::to_simple_string(now));

  // set revision
  sesn_rel->set_revision(REVISION_INIT_VALUE);

  // set from/to
  sesn_rel->set_from_id(_from_id);
  sesn_rel->set_to_id(_to_id);

  //insert record
  insert_relation(sesn_rel);

  list<boost::tuple<string, schema_parameter_ptr>> param_name_types;
  _schm_rel->get_parameters(&param_name_types);

  for (list<boost::tuple<string, schema_parameter_ptr>>::iterator pit
       = param_name_types.begin(); pit != param_name_types.end(); pit++)
  {
    insert_relation_parameter_by_default(sesn_rel, *pit);
  }

  return sesn_rel;
}

void session::get_connected_object_from(string _to_id,
                                        list<string>& _rel_type_list,
                                        list<session_object_ptr>* _sesn_obj_list)
{
  list<string> id_list;

  fetcher<string>::fetch_use1(
    *soci_session_
    , "SELECT SO_F.id_ FROM session_object SO_F "
    "INNER JOIN session_relation SR ON "
    "SR.from_id = SO_F.id_ "
    "INNER JOIN schema_relation R ON "
    "R.id_ = SR.schema_relation_id "
    "WHERE SR.to_id = :to_id "
    + where_condition<string>(_rel_type_list, "R.type")()
    , soci::use(_to_id, "to_id")
    , &id_list);

  get_object(id_list, _sesn_obj_list);
}
void session::get_connected_object_to(string _from_id,
                                      list<string>& _rel_type_list,
                                      list<session_object_ptr>* _sesn_obj_list)
{
  list<string> id_list;

  fetcher<string>::fetch_use1(
    *soci_session_
    , "SELECT SO_T.id_ FROM session_object SO_T "
    "INNER JOIN session_relation SR ON "
    "SR.to_id = SO_T.id_ "
    "INNER JOIN schema_relation R ON "
    "R.id_ = SR.schema_relation_id "
    "WHERE SR.from_id = :from_id "
    + where_condition<string>(_rel_type_list, "R.type")()
    , soci::use(_from_id, "from_id")
    , &id_list);

  get_object(id_list, _sesn_obj_list);
}

void session::validate_connect(string _from_id,
                               string _to_id,
                               list<string>& _rel_type_list,
                               list<schema_relation_ptr>* _schm_rel_list
                              )
{
  list<string> id_list;

  fetcher<string>::fetch_use2(
    *soci_session_
    , "SELECT R.id_ FROM schema_relation R "
    "INNER JOIN schema_object O1 ON R.from_id = O1.id_ "
    "INNER JOIN session_object S1 ON O1.id_ = S1.schema_object_id "
    "INNER JOIN schema_object O2 ON R.to_id = O2.id_ "
    "INNER JOIN session_object S2 ON O2.id_ = S2.schema_object_id "
    "WHERE S1.id_ = :from_id AND S2.id_ = :to_id "
    + where_condition<string>(_rel_type_list, "R.type")()
    , soci::use(_from_id, "from_id")
    , soci::use(_to_id, "to_id")
    , &id_list);

  for (list<string>::iterator it = id_list.begin(); it != id_list.end(); it++)
  {
    optional<schema_relation_ptr> schm_rel = schema_->get_relation(*it);
    _schm_rel_list->push_back(schm_rel.get());
  }
}

optional<session_relation_ptr> session::get_relation(string _rel_id)
{
  session_relation_ptr sesn_rel(new session_relation(this));

  *(soci_session_.get())
      <<
      "SELECT * FROM session_relation WHERE id_ = :id_",
      soci::into(*(sesn_rel.get())),
      soci::use(_rel_id);

  if (sesn_rel->get_id().empty())
  {
    // no id found
    return optional<session_relation_ptr>();
  }

  // parameter
  map<string, session_parameter_ptr>* param_map(
    sesn_rel->get_parameters());

  list<boost::tuple<string, schema_parameter_ptr>> param_name_types;
  sesn_rel->get_schema_relation()->get_parameters(&param_name_types);

  for (list<boost::tuple<string, schema_parameter_ptr>>::iterator pit
       = param_name_types.begin(); pit != param_name_types.end(); pit++)
  {
    // insert target
    list<parameter_value_variant> var_value_list;

    switch ((parameter_basetype_enum)boost::get<1>(*pit)->get_basetype())
    {

    case parameter_basetype_enum::integer:
    {
      list<int> value_list;

      fetcher<int>::fetch_use2(
        *soci_session_
        , "SELECT value FROM session_relation_parameter_basetype_integer "
        "WHERE relid = :oid AND param_name = :param_name "
        "ORDER BY array_index"
        , soci::use(_rel_id)
        , soci::use(pit->get<0>())
        , &value_list);

      // convert
      parameter_value_converter<int>().convert_from_list_to_variant_list
      (value_list, &var_value_list);
    }
    break;

    case parameter_basetype_enum::real:
    {
      list<double> value_list;

      fetcher<double>::fetch_use2(
        *soci_session_
        , "SELECT value FROM session_relation_parameter_basetype_real "
        "WHERE relid = :oid AND param_name = :param_name "
        "ORDER BY array_index"
        , soci::use(_rel_id)
        , soci::use(pit->get<0>())
        , &value_list);

      // convert
      parameter_value_converter<double>().convert_from_list_to_variant_list
      (value_list, &var_value_list);
    }
    break;

    case parameter_basetype_enum::text:
    {
      list<string> value_list;

      fetcher<string>::fetch_use2(
        *soci_session_
        , "SELECT value FROM session_relation_parameter_basetype_text "
        "WHERE relid = :oid AND param_name = :param_name "
        "ORDER BY array_index"
        , soci::use(_rel_id)
        , soci::use(pit->get<0>())
        , &value_list);

      // convert
      parameter_value_converter<string>().convert_from_list_to_variant_list
      (value_list, &var_value_list);
    }
    break;
    }

    // register
    param_map->insert(
      std::make_pair(pit->get<0>(),
                     session_parameter_ptr(new session_parameter(var_value_list, pit->get<1>()))));
  }
  return optional<session_relation_ptr>(sesn_rel);
}

void session::get_relation(list<session_relation_ptr>* _sesn_rel_list)
{
  get_relation_by_query(where_clause<string>("1=1")
                        , _sesn_rel_list);
}

void session::get_relation(list<string> _rel_id_list,
                           list<session_relation_ptr>* _sesn_rel_list)
{
  _sesn_rel_list->clear();

  for (list<string>::iterator it = _rel_id_list.begin(); it != _rel_id_list.end();
       ++it)
  {
    optional<session_relation_ptr> ptr = get_relation(*it);
    if (ptr.is_initialized())  { _sesn_rel_list->push_back(ptr.get()); }
  }
}

void session::get_relation(string _from_id, string _to_id,
                           list<session_relation_ptr>* _sesn_rel_list)
{
  list<string> rel_id_list;

  fetcher<string>::fetch_use2(
    *soci_session_
    , "SELECT id_ FROM session_relation WHERE "
    "from_id = :from_id AND to_id = :to_id"
    , soci::use(_from_id, "from_id")
    , soci::use(_to_id, "to_id")
    , &rel_id_list);

  get_relation(rel_id_list, _sesn_rel_list);
}

void session::get_relation_by_type(list<string> _rel_type_list,
                                   list<session_relation_ptr>* _sesn_rel_list)
{
  get_relation_by_query(where_condition<string>(_rel_type_list,
                        "schema_relation.type")
                        , _sesn_rel_list);
}

void session::get_relation_by_name(list<string> _rel_name_list,
                                   list<session_relation_ptr>* _sesn_rel_list)
{
  list<string> rel_id_list;

  fetcher<string>::fetch(
    *soci_session_
    , "SELECT id_ FROM session_relation WHERE 1 = 1"
    , &rel_id_list);

  get_relation(rel_id_list, _sesn_rel_list);
}

void session::disconnect(string _rel_id,
                         map<string, session_parameter_ptr>* _param_map)
{
  // delete from db
  for (map<string, session_parameter_ptr>::iterator it = _param_map->begin();
       it != _param_map->end(); ++it)
  {
    parameter_basetype_enum t =
      schema_parameter::convert_to_parameter_basetype_enum(
        it->second->schema_parameter_->get_basetype());

    switch (t)
    {
    case parameter_basetype_enum::integer:

      *soci_session_
          << "DELETE FROM session_relation_parameter_basetype_integer "
          "WHERE relid = :oid AND param_name = :param_name "
          , soci::use(_rel_id)
          , soci::use(it->first);
      break;

    case parameter_basetype_enum::real:
      *soci_session_
          << "DELETE FROM session_relation_parameter_basetype_real "
          "WHERE relid = :oid AND param_name = :param_name "
          , soci::use(_rel_id)
          , soci::use(it->first);
      break;

    case parameter_basetype_enum::text:
      *soci_session_
          << "DELETE FROM session_relation_parameter_basetype_text "
          "WHERE relid = :oid AND param_name = :param_name "
          , soci::use(_rel_id)
          , soci::use(it->first);
      break;

    case parameter_basetype_enum::select:
      *soci_session_
          << "DELETE FROM session_relation_parameter_basetype_select "
          "WHERE relid = :oid AND param_name = :param_name "
          , soci::use(_rel_id)
          , soci::use(it->first);
      break;
    }
  }

  *(soci_session_.get())
      <<
      "DELETE FROM session_relation WHERE id_ = :_rel_id",
      soci::use(_rel_id);
}
// <=== relation

void session::sync_object(session_object* _sesn_obj)
{
  *(soci_session_.get())
      <<
      "UPDATE session_object SET comment = :comment, "
      "name = :name, revision = :revision, "
      "create_date = :create_date, update_date = :update_date"
      " WHERE id_ = :id_",
      soci::use(*_sesn_obj);

  map<string, session_parameter_ptr>* p(
    _sesn_obj->get_parameters());

  for (map<string, session_parameter_ptr>::iterator it =
         p->begin(); it != p->end(); it++)
  {
    sync_object_parameter(_sesn_obj->get_id(), it->first, it->second,
                          (parameter_basetype_enum)it->second->schema_parameter_->get_basetype());
  }
}
void session::sync_relation(session_relation* _sesn_rel)
{
  *(soci_session_.get())
      <<
      "UPDATE session_relation SET comment = :comment, "
      "name = :name, revision = :revision, "
      "create_date = :create_date, update_date = :update_date"
      " WHERE id_ = :id_",
      soci::use(*_sesn_rel);

  map<string, session_parameter_ptr>* p(
    _sesn_rel->get_parameters());

  for (map<string, session_parameter_ptr>::iterator it =
         p->begin(); it != p->end(); it++)
  {
    sync_relation_parameter(_sesn_rel->get_id(), it->first, it->second,
                            (parameter_basetype_enum)it->second->schema_parameter_->get_basetype());
  }
}

template<class T>
void session::get_object_by_query(const T& _query,
                                  list<session_object_ptr>* _sesn_obj_list)
{
  list<string> o_id_list;

  fetcher<string>::fetch(
    *soci_session_
    , "SELECT session_object.id_ FROM session_object "
    "INNER JOIN schema_object ON session_object.schema_object_id = "
    "schema_object.id_ WHERE 1 = 1"
    + _query()
    , &o_id_list);

  get_object(o_id_list, _sesn_obj_list);
}

template<class T>
void session::get_relation_by_query(const T& _query,
                                    list<session_relation_ptr>* _sesn_rel_list)
{
  list<string> rel_id_list;

  fetcher<string>::fetch(
    *soci_session_
    , "SELECT session_relation.id_ FROM session_relation "
    "INNER JOIN schema_relation ON session_relation.schema_relation_id = "
    "schema_relation.id_ WHERE 1 = 1"
    + _query()
    , &rel_id_list);

  get_relation(rel_id_list, _sesn_rel_list);
}

bool session::import_from_file(string _path)
{
  fs::path imp(_path);

  boost::system::error_code error;
  const bool result = fs::exists(imp, error);
  if (!result || error)
  {
    OG_LOG << "import file is not found:" << _path;
    return false;
  }

  // delete session property object in advance
  {
    session_object_ptr prop_obj = get_property_object();
    if (prop_obj != nullptr) { prop_obj->delete_object(); }
  }

  boost::property_tree::ptree pt;
  xml_stream().read_from_file(_path, &pt);

  int count_object = 0;
  // session object
  BOOST_FOREACH(ptree::value_type & child,
                pt.get_child("og.session.objects"))
  {
    OG_LOG << "import session object:" << count_object++;

    session_object_ptr sesn_obj(new session_object(this));

    // deserialize
    if (serializer<session_object_ptr>::deserialize(child, sesn_obj))
    {
      boost::optional<schema_object_ptr> schm_obj =
        schema_->get_object(sesn_obj->get_schema_object_id());

      if (schm_obj.is_initialized())
      {
        if (schm_obj.get()->get_type().compare(schema::schema_property_object_type_) ==
            0)
        {
          list<string> prep;
          list<schema_object_ptr> schm_objs;
          prep.push_back(schema::schema_property_object_type_);

          schema_->get_object_by_type(prep, &schm_objs);
          sesn_obj->set_schema_object(schm_objs.front());
        }

        if (!import_object(sesn_obj, child.second)) { continue; }
      }
    }
  }

  // session relation
  int count_relation = 0;
  BOOST_FOREACH(ptree::value_type & child,
                pt.get_child("og.session.relations"))
  {
    OG_LOG << "import session relation:" << count_relation++;

    session_relation_ptr sesn_rel(new session_relation(this));

    // deserialize
    if (serializer<session_relation_ptr>::deserialize(child, sesn_rel))
    {
      boost::optional<schema_relation_ptr> schm_rel =
        schema_->get_relation(sesn_rel->get_schema_relation_id());

      if (schm_rel.is_initialized())
      {
        if (schm_rel.get()->get_type().compare(schema::schema_property_object_type_) ==
            0)
        {
          list<string> prep;
          list<schema_relation_ptr> schm_rels;
          prep.push_back(schema::schema_property_object_type_);

          schema_->get_relation_by_type(prep, &schm_rels);
          sesn_rel->set_schema_relation(schm_rels.front());
        }

        // check from/to
        boost::optional<session_object_ptr> sesn_from =
          get_object(sesn_rel->get_from_id());
        boost::optional<session_object_ptr> sesn_to =
          get_object(sesn_rel->get_to_id());

        if (sesn_from.is_initialized() && sesn_to.is_initialized())
        {
          if (!import_relation(sesn_rel, child.second)) { continue; }
        }
      }
    }
  }

  if (get_property_object() == nullptr)
  {
    OG_LOG << "building property.";
    build_property_object();
  }

  return true;
}
session_object_ptr session::get_property_object()
{
  schema_object_ptr schm_obj = this->schema_->get_property_object();
  if (schm_obj == nullptr)
  {
    return nullptr;
  }

  list<session_object_ptr> sesn_objs;
  list<schema_object_ptr> schm_objs;
  schm_objs.push_back(schm_obj);

  get_object_by_schema_object(schm_objs, &sesn_objs);

  if (sesn_objs.size() == 0)
  {
    return nullptr;
  }
  else
  {
    return sesn_objs.front();
  }
}

session_object_ptr session::build_property_object()
{
  schema_object_ptr schm_obj = this->schema_->get_property_object();
  if (schm_obj == nullptr)
  {
    schm_obj = this->schema_->build_property_object();
  }

  list<session_object_ptr> sesn_objs;
  list<schema_object_ptr> schm_objs;
  schm_objs.push_back(schm_obj);

  get_object_by_schema_object(schm_objs, &sesn_objs);

  if (sesn_objs.size() == 0)
  {
    return create_object(schm_objs.front());
  }
  else
  {
    string mes("fatal. schema property object exists.");
    OG_LOG << mes;
    throw og::core::exception() << exception_message(mes);
  }
}

void session::export_to_file(string _path)
{
  // Create XML document
  boost::property_tree::ptree pt;
  ptree& pt_1 = pt.add("og.session", "");

  // session object
  list<session_object_ptr> sesn_obj_list;
  get_object(&sesn_obj_list);

  BOOST_FOREACH(const session_object_ptr& s, sesn_obj_list)
  {
    // serialize
    serializer<session_object_ptr>::serialize(pt_1, s);
    //export_parameter_to_file(s->get_schema_object(), s, pt_1);
  }

  // session relation
  list<session_relation_ptr> sesn_rel_list;
  get_relation(&sesn_rel_list);

  BOOST_FOREACH(const session_relation_ptr& s, sesn_rel_list)
  {
    // serialize
    serializer<session_relation_ptr>::serialize(pt_1, s);
    //export_parameter_to_file(s->get_schema_relation(), s, pt_1);
  }

  xml_stream().write_to_file(_path, pt);
}

void session::sync_object_parameter(
  string _obj_id, string _param_name,
  session_parameter_ptr _sesn_parameter, parameter_basetype_enum _basetype)
{
  int i = 0;
  for (list<parameter_value_variant>::iterator it =
         _sesn_parameter->values_.begin();
       it != _sesn_parameter->values_.end(); it++, i++)
  {
    switch (_basetype)
    {
    case parameter_basetype_enum::integer:
      if (i == 0)
      {
        *(soci_session_.get())
            <<
            "DELETE FROM session_object_parameter_basetype_integer "
            "WHERE oid = :_obj_id AND param_name = :param_name"
            , soci::use(_obj_id)
            , soci::use(_param_name);
      }

      *(soci_session_.get())
          <<
          "INSERT INTO session_object_parameter_basetype_integer(oid, param_name, "
          "array_index, value) VALUES "
          "(:relid, :param_name, :array_index, :value)"
          , soci::use(_obj_id)
          , soci::use(_param_name)
          , soci::use(i)
          , soci::use(boost::get<int>(*it));
      break;
    case parameter_basetype_enum::real:
      if (i == 0)
      {
        *(soci_session_.get())
            <<
            "DELETE FROM session_object_parameter_basetype_real "
            "WHERE oid = :_obj_id AND param_name = :param_name"
            , soci::use(_obj_id)
            , soci::use(_param_name);
      }

      *(soci_session_.get())
          <<
          "INSERT INTO session_object_parameter_basetype_real(oid, param_name, "
          "array_index, value) VALUES "
          "(:relid, :param_name, :array_index, :value)"
          , soci::use(_obj_id)
          , soci::use(_param_name)
          , soci::use(i)
          , soci::use(boost::get<double>(*it));
      break;
    case parameter_basetype_enum::text:
      if (i == 0)
      {
        *(soci_session_.get())
            <<
            "DELETE FROM session_object_parameter_basetype_text "
            "WHERE oid = :_obj_id AND param_name = :param_name"
            , soci::use(_obj_id)
            , soci::use(_param_name);
      }

      *(soci_session_.get())
          <<
          "INSERT INTO session_object_parameter_basetype_text(oid, param_name, "
          "array_index, value) VALUES "
          "(:relid, :param_name, :array_index, :value)"
          , soci::use(_obj_id)
          , soci::use(_param_name)
          , soci::use(i)
          , soci::use(boost::get<string>(*it));
      break;
      break;
    case parameter_basetype_enum::select:
      break;
    }
  }
}

void session::sync_relation_parameter(
  string _rel_id, string _param_name,
  session_parameter_ptr _sesn_parameter, parameter_basetype_enum _basetype)
{
  int i = 0;
  for (list<parameter_value_variant>::iterator it =
         _sesn_parameter->values_.begin();
       it != _sesn_parameter->values_.end(); it++, i++)
  {
    switch (_basetype)
    {
    case parameter_basetype_enum::integer:
      if (i == 0)
      {
        *(soci_session_.get())
            <<
            "DELETE FROM session_relation_parameter_basetype_integer "
            "WHERE relid = :_rel_id AND param_name = :param_name"
            , soci::use(_rel_id)
            , soci::use(_param_name);
      }

      *(soci_session_.get())
          <<
          "INSERT INTO session_relation_parameter_basetype_integer(relid, param_name, "
          "array_index, value) VALUES "
          "(:relid, :param_name, :array_index, :value)"
          , soci::use(_rel_id)
          , soci::use(_param_name)
          , soci::use(i)
          , soci::use(boost::get<int>(*it));
      break;

    case parameter_basetype_enum::real:
      if (i == 0)
      {
        *(soci_session_.get())
            <<
            "DELETE FROM session_relation_parameter_basetype_real "
            "WHERE relid = :_rel_id AND param_name = :param_name"
            , soci::use(_rel_id)
            , soci::use(_param_name);
      }

      *(soci_session_.get())
          <<
          "INSERT INTO session_relation_parameter_basetype_real(relid, param_name, "
          "array_index, value) VALUES "
          "(:relid, :param_name, :array_index, :value)"
          , soci::use(_rel_id)
          , soci::use(_param_name)
          , soci::use(i)
          , soci::use(boost::get<double>(*it));
      break;
    case parameter_basetype_enum::text:
      if (i == 0)
      {
        *(soci_session_.get())
            <<
            "DELETE FROM session_relation_parameter_basetype_text "
            "WHERE relid = :_rel_id AND param_name = :param_name"
            , soci::use(_rel_id)
            , soci::use(_param_name);
      }

      *(soci_session_.get())
          <<
          "INSERT INTO session_relation_parameter_basetype_text(relid, param_name, "
          "array_index, value) VALUES "
          "(:relid, :param_name, :array_index, :value)"
          , soci::use(_rel_id)
          , soci::use(_param_name)
          , soci::use(i)
          , soci::use(boost::get<string>(*it));
      break;
    case parameter_basetype_enum::select:
      break;
    }
  }
}

bool session::catchup_schema(string _path)
{
  OG_LOG << "catchup_schema start.";

  fs::path schema_new(_path);

  boost::system::error_code error;
  const bool result = fs::exists(schema_new, error);
  if (!result || error)
  {
    OG_LOG << "schema file is not found:" << _path;
    throw og::core::exception() << exception_message("schema file is not found.");
  }

  // get current schema version
  std::string rev = schema_->get_property_object()->get_revision();
  long current_schema_rev = boost::lexical_cast<long>(rev);
  OG_LOG << "current schema revision: " << current_schema_rev;
  long file_schema_rev = schema_->get_schema_rev_from_file(_path);
  OG_LOG << "file schema revision: " << file_schema_rev;

  if (file_schema_rev <= current_schema_rev)
  {
    OG_LOG << "It's not necessary to catchup.";
    return true;
  }

  char* tempdir = getenv("TEMP");
  stringstream session_tempname, schema_tempname;
  session_tempname << tempdir << "/SESSION-%%%%-%%%%-%%%%-%%%%.xml.gz";
  schema_tempname << tempdir << "/SCHEMA-%%%%-%%%%-%%%%-%%%%.xml.gz";
  fs::path session_temp = fs::unique_path(session_tempname.str());
  fs::path schema_temp = fs::unique_path(schema_tempname.str());

  OG_LOG << "session backup:" << session_temp;
  export_to_file(session_temp.string());

  OG_LOG << "schema backup:" << schema_temp;
  schema_->export_to_file(schema_temp.string());

  OG_LOG << "purge schema";
  schema_->purge();

  OG_LOG << "import schema from file";
  if (!schema_->import_from_file(_path))
  {
    return false;
  }

  OG_LOG << "import session from backup";
  if (!import_from_file(session_temp.string()))
  {
    return false;
  }

  OG_LOG << "catchup done.";

  return true;
}

bool session::reload_schema(string _path, string _sess_tmp)
{
  OG_LOG << "reload_schema start.";

  fs::path schema_new(_path);

  boost::system::error_code error;
  const bool result = fs::exists(schema_new, error);
  if (!result || error)
  {
    OG_LOG << "schema file is not found:" << _path;
    throw og::core::exception() << exception_message("schema file is not found.");
  }

  long file_schema_rev = schema_->get_schema_rev_from_file(_path);
  OG_LOG << "file schema revision: " << file_schema_rev;

  fs::path session_temp(_sess_tmp);

  OG_LOG << "purge schema";
  schema_->purge();

  OG_LOG << "import schema from file";
  if (!schema_->import_from_file(_path))
  {
    return false;
  }

  OG_LOG << "import session from backup";
  if (!import_from_file(session_temp.string()))
  {
    return false;
  }

  OG_LOG << "reload done.";

  return true;
}

void session::delete_object_parameter_definition(string _param_name,
    schema_parameter_ptr _param)
{
  parameter_basetype_enum t =
    schema_parameter::convert_to_parameter_basetype_enum(
      _param->get_basetype());

  switch (t)
  {
  case parameter_basetype_enum::integer:

    *(soci_session_.get())
        << "DELETE FROM session_object_parameter_basetype_integer "
        "WHERE param_name = :param_name "
        , soci::use(_param_name);
    break;

  case parameter_basetype_enum::real:
    *(soci_session_.get())
        << "DELETE FROM session_object_parameter_basetype_real "
        "WHERE param_name = :param_name "
        , soci::use(_param_name);
    break;

  case parameter_basetype_enum::text:
    *(soci_session_.get())
        << "DELETE FROM session_object_parameter_basetype_text "
        "WHERE param_name = :param_name "
        , soci::use(_param_name);
    break;

  case parameter_basetype_enum::select:
    *(soci_session_.get())
        << "DELETE FROM session_object_parameter_basetype_select "
        "WHERE param_name = :param_name "
        , soci::use(_param_name);
    break;
  }
}

void session::delete_relatoin_parameter_definition(string _param_name,
    schema_parameter_ptr _param)
{
  parameter_basetype_enum t =
    schema_parameter::convert_to_parameter_basetype_enum(
      _param->get_basetype());

  switch (t)
  {
  case parameter_basetype_enum::integer:
    *(soci_session_.get())
        << "DELETE FROM session_relation_parameter_basetype_integer "
        "WHERE param_name = :param_name "
        , soci::use(_param_name);
    break;

  case parameter_basetype_enum::real:
    *(soci_session_.get())
        << "DELETE FROM session_relation_parameter_basetype_real "
        "WHERE param_name = :param_name "
        , soci::use(_param_name);
    break;

  case parameter_basetype_enum::text:
    *(soci_session_.get())
        << "DELETE FROM session_relation_parameter_basetype_text "
        "WHERE param_name = :param_name "
        , soci::use(_param_name);
    break;

  case parameter_basetype_enum::select:
    *(soci_session_.get())
        << "DELETE FROM session_relation_parameter_basetype_select "
        "WHERE param_name = :param_name "
        , soci::use(_param_name);
    break;
  }
}

void session::insert_object(session_object_ptr _sesn_obj)
{
  *(soci_session_.get())
      <<
      "INSERT INTO session_object(id_, schema_object_id, comment, "
      "name, revision, create_date, update_date) "
      "values(:id_, :schema_object_id, :comment, "
      ":name, :revision, :create_date, :update_date)",
      soci::use(*(_sesn_obj.get()));
}

void session::insert_relation(session_relation_ptr _sesn_rel)
{
  *(soci_session_.get())
      <<
      "INSERT INTO session_relation(id_, schema_relation_id, comment, "
      "name, revision, create_date, update_date, from_id, to_id) "
      "values(:id_, :schema_relation_id, :comment, "
      ":name, :revision, :create_date, :update_date, :from_id, :to_id)"
      , soci::use(*(_sesn_rel));
}

void session::insert_object_parameter_by_default(session_object_ptr _sesn_obj,
    boost::tuple<string, schema_parameter_ptr>& _sesn_obj_param)
{
  string schema_par_id = _sesn_obj_param.get<1>()->get_id();
  parameter_basetype_enum t =
    schema_parameter::convert_to_parameter_basetype_enum(
      _sesn_obj_param.get<1>()->get_basetype());

  //data target
  list<parameter_value_variant> target_list;

  int i_max = _sesn_obj_param.get<1>()->get_default_array_size();
  for (int i = 0; i < i_max; i++)
  {
    switch (t)
    {
    case parameter_basetype_enum::integer:
    {
      int def(0);
      *(soci_session_.get())
          <<
          "SELECT default_value FROM schema_parameter "
          "INNER JOIN schema_parameter_basetype_integer "
          "ON schema_parameter.id_ = schema_parameter_basetype_integer.id_ "
          "WHERE schema_parameter.id_ = :pid"
          , soci::use(schema_par_id)
          , soci::into(def);

      *(soci_session_.get())
          <<
          "INSERT INTO session_object_parameter_basetype_integer(oid, param_name, "
          "array_index, value) VALUES "
          "(:oid, :param_name, :array_index, :default_value)"
          , soci::use(_sesn_obj->get_id())
          , soci::use(_sesn_obj_param.get<0>())
          , soci::use(i)
          , soci::use(def);

      target_list.push_back(def);
    }
    break;

    case parameter_basetype_enum::real:
    {
      double def(0);
      *(soci_session_.get())
          <<
          "SELECT default_value FROM schema_parameter "
          "INNER JOIN schema_parameter_basetype_real "
          "ON schema_parameter.id_ = schema_parameter_basetype_real.id_ "
          "WHERE schema_parameter.id_ = :pid"
          , soci::use(schema_par_id)
          , soci::into(def);

      *(soci_session_.get())
          <<
          "INSERT INTO session_object_parameter_basetype_real(oid, param_name, "
          "array_index, value) VALUES "
          "(:oid, :param_name, :array_index, :default_value)"
          , soci::use(_sesn_obj->get_id())
          , soci::use(_sesn_obj_param.get<0>())
          , soci::use(i)
          , soci::use(def);

      target_list.push_back(def);
    }
    break;
    case parameter_basetype_enum::text:
    {
      string def("");
      *(soci_session_.get())
          <<
          "SELECT default_value FROM schema_parameter "
          "INNER JOIN schema_parameter_basetype_text "
          "ON schema_parameter.id_ = schema_parameter_basetype_text.id_ "
          "WHERE schema_parameter.id_ = :pid"
          , soci::use(schema_par_id)
          , soci::into(def);

      *(soci_session_.get())
          <<
          "INSERT INTO session_object_parameter_basetype_text(oid, param_name, "
          "array_index, value) VALUES "
          "(:oid, :param_name, :array_index, :default_value)"
          , soci::use(_sesn_obj->get_id())
          , soci::use(_sesn_obj_param.get<0>())
          , soci::use(i)
          , soci::use(def);

      target_list.push_back(def);
    }
    break;

    }
  }

  // register data
  _sesn_obj->get_parameters()->insert(
    std::make_pair(boost::get<0>(_sesn_obj_param),
                   session_parameter_ptr(new session_parameter(target_list,
                                         boost::get<1>(_sesn_obj_param)))));
}

void session::insert_object_parameter_with_arg(session_object_ptr _sesn_obj,
    boost::tuple<string, schema_parameter_ptr>& _sesn_obj_param,
    list<parameter_value_variant>& _arg)
{
  string schema_par_id = _sesn_obj_param.get<1>()->get_id();
  parameter_basetype_enum t =
    schema_parameter::convert_to_parameter_basetype_enum(
      _sesn_obj_param.get<1>()->get_basetype());

  int i = 0;
  for (list<parameter_value_variant>::iterator it = _arg.begin();
       it != _arg.end(); it++, i++)
  {
    switch (t)
    {
    case parameter_basetype_enum::integer:
    {
      if (i == 0)
      {
        *(soci_session_.get())
            <<
            "DELETE FROM session_object_parameter_basetype_integer WHERE "
            "oid = :oid AND param_name = :param_name"
            , soci::use(_sesn_obj->get_id())
            , soci::use(_sesn_obj_param.get<0>());
      }
      *(soci_session_.get())
          <<
          "INSERT INTO session_object_parameter_basetype_integer(oid, param_name, "
          "array_index, value) VALUES "
          "(:oid, :param_name, :array_index, :default_value)"
          , soci::use(_sesn_obj->get_id())
          , soci::use(_sesn_obj_param.get<0>())
          , soci::use(i)
          , soci::use(boost::get<int>(*it));
    }
    break;

    case parameter_basetype_enum::real:
    {
      if (i == 0)
      {
        *(soci_session_.get())
            <<
            "DELETE FROM session_object_parameter_basetype_real WHERE "
            "oid = :oid AND param_name = :param_name"
            , soci::use(_sesn_obj->get_id())
            , soci::use(_sesn_obj_param.get<0>());
      }

      *(soci_session_.get())
          <<
          "INSERT INTO session_object_parameter_basetype_real(oid, param_name, "
          "array_index, value) VALUES "
          "(:oid, :param_name, :array_index, :default_value)"
          , soci::use(_sesn_obj->get_id())
          , soci::use(_sesn_obj_param.get<0>())
          , soci::use(i)
          , soci::use(boost::get<double>(*it));
    }
    break;

    case parameter_basetype_enum::text:
    {
      if (i == 0)
      {
        *(soci_session_.get())
            <<
            "DELETE FROM session_object_parameter_basetype_text WHERE "
            "oid = :oid AND param_name = :param_name"
            , soci::use(_sesn_obj->get_id())
            , soci::use(_sesn_obj_param.get<0>());
      }

      *(soci_session_.get())
          <<
          "INSERT INTO session_object_parameter_basetype_text(oid, param_name, "
          "array_index, value) VALUES "
          "(:oid, :param_name, :array_index, :default_value)"
          , soci::use(_sesn_obj->get_id())
          , soci::use(_sesn_obj_param.get<0>())
          , soci::use(i)
          , soci::use(boost::get<string>(*it));
    }
    break;

    default:
      throw og::core::exception() << exception_message("unexpected param type.");
    }
  }

  // register data
  boost::optional<og::core::session_parameter_ptr> sesn_p =
    _sesn_obj->get_parameter(boost::get<0>(_sesn_obj_param));

  if (!sesn_p.is_initialized())
  {
    list<parameter_value_variant> empty_list;
    _sesn_obj->get_parameters()->insert
    (std::make_pair(boost::get<0>(_sesn_obj_param),
                    session_parameter_ptr(new session_parameter(empty_list,
                                          boost::get<1>(_sesn_obj_param)))));

    sesn_p =
      _sesn_obj->get_parameter(boost::get<0>(_sesn_obj_param));
  }

  if (!sesn_p.is_initialized())
  {
    throw og::core::exception() <<
                                exception_message("unexpected. parameter not found.");
  }

  sesn_p.get()->values_.clear();
  sesn_p.get()->values_.assign(_arg.begin(), _arg.end());
}

void session::insert_relation_parameter_by_default(session_relation_ptr
    _sesn_rel,
    boost::tuple<string, schema_parameter_ptr>& _sesn_rel_param)
{
  string schema_par_id = _sesn_rel_param.get<1>()->get_id();
  parameter_basetype_enum t =
    schema_parameter::convert_to_parameter_basetype_enum(
      _sesn_rel_param.get<1>()->get_basetype());

  //data target
  list<parameter_value_variant> target_list;

  int i_max = _sesn_rel_param.get<1>()->get_default_array_size();
  for (int i = 0; i < i_max; i++)
  {
    switch (t)
    {
    case parameter_basetype_enum::integer:
    {
      int def(0);
      *(soci_session_.get())
          <<
          "SELECT default_value FROM schema_parameter "
          "INNER JOIN schema_parameter_basetype_integer "
          "ON schema_parameter.id_ = schema_parameter_basetype_integer.id_ "
          "WHERE schema_parameter.id_ = :pid"
          , soci::use(schema_par_id)
          , soci::into(def);

      *(soci_session_.get())
          <<
          "INSERT INTO session_relation_parameter_basetype_integer(relid, param_name, "
          "array_index, value) VALUES "
          "(:relid, :param_name, :array_index, :default_value)"
          , soci::use(_sesn_rel->get_id())
          , soci::use(_sesn_rel_param.get<0>())
          , soci::use(i)
          , soci::use(def);

      target_list.push_back(def);
    }
    break;

    case parameter_basetype_enum::real:
    {
      double def(0);

      *(soci_session_.get())
          <<
          "SELECT default_value FROM schema_parameter "
          "INNER JOIN schema_parameter_basetype_real "
          "ON schema_parameter.id_ = schema_parameter_basetype_real.id_ "
          "WHERE schema_parameter.id_ = :pid"
          , soci::use(schema_par_id)
          , soci::into(def);

      *(soci_session_.get())
          <<
          "INSERT INTO session_relation_parameter_basetype_real(relid, param_name, "
          "array_index, value) VALUES "
          "(:relid, :param_name, :array_index, :default_value)"
          , soci::use(_sesn_rel->get_id())
          , soci::use(_sesn_rel_param.get<0>())
          , soci::use(i)
          , soci::use(def);

      target_list.push_back(def);
    }
    break;

    case parameter_basetype_enum::text:
    {
      string def("");

      *(soci_session_.get())
          <<
          "SELECT default_value FROM schema_parameter "
          "INNER JOIN schema_parameter_basetype_text "
          "ON schema_parameter.id_ = schema_parameter_basetype_text.id_ "
          "WHERE schema_parameter.id_ = :pid"
          , soci::use(schema_par_id)
          , soci::into(def);

      *(soci_session_.get())
          <<
          "INSERT INTO session_relation_parameter_basetype_text(relid, param_name, "
          "array_index, value) VALUES"
          "(:relid, :param_name, :array_index, :default_value) "
          , soci::use(_sesn_rel->get_id())
          , soci::use(_sesn_rel_param.get<0>())
          , soci::use(i)
          , soci::use(def);

      target_list.push_back(def);
    }
    break;
    }
  }
  // register data
  _sesn_rel->get_parameters()->insert(
    std::make_pair(boost::get<0>(_sesn_rel_param),
                   session_parameter_ptr(new session_parameter(target_list,
                                         boost::get<1>(_sesn_rel_param)))));
}

void session::insert_relation_parameter_with_arg(session_relation_ptr
    _sesn_rel,
    boost::tuple<string, schema_parameter_ptr>& _sesn_rel_param,
    list<parameter_value_variant>& _arg)
{
  string schema_par_id = _sesn_rel_param.get<1>()->get_id();
  parameter_basetype_enum t =
    schema_parameter::convert_to_parameter_basetype_enum(
      _sesn_rel_param.get<1>()->get_basetype());

  int i = 0;
  for (list<parameter_value_variant>::iterator it = _arg.begin();
       it != _arg.end(); it++, i++)
  {
    switch (t)
    {
    case parameter_basetype_enum::integer:
    {
      if (i == 0)
      {
        *(soci_session_.get())
            <<
            "DELETE FROM session_relation_parameter_basetype_integer WHERE "
            "relid = :oid AND param_name = :param_name"
            , soci::use(_sesn_rel->get_id())
            , soci::use(_sesn_rel_param.get<0>());
      }
      *(soci_session_.get())
          <<
          "INSERT INTO session_relation_parameter_basetype_integer(relid, param_name, "
          "array_index, value) VALUES "
          "(:relid, :param_name, :array_index, :default_value)"
          , soci::use(_sesn_rel->get_id())
          , soci::use(_sesn_rel_param.get<0>())
          , soci::use(i)
          , soci::use(boost::get<int>(*it));
    }
    break;

    case parameter_basetype_enum::real:
    {
      if (i == 0)
      {
        *(soci_session_.get())
            <<
            "DELETE FROM session_relation_parameter_basetype_real WHERE "
            "relid = :oid AND param_name = :param_name"
            , soci::use(_sesn_rel->get_id())
            , soci::use(_sesn_rel_param.get<0>());
      }
      *(soci_session_.get())
          <<
          "INSERT INTO session_relation_parameter_basetype_real(relid, param_name, "
          "array_index, value) VALUES "
          "(:relid, :param_name, :array_index, :default_value)"
          , soci::use(_sesn_rel->get_id())
          , soci::use(_sesn_rel_param.get<0>())
          , soci::use(i)
          , soci::use(boost::get<double>(*it));
    }
    break;

    case parameter_basetype_enum::text:
    {
      if (i == 0)
      {
        *(soci_session_.get())
            <<
            "DELETE FROM session_relation_parameter_basetype_text WHERE "
            "relid = :oid AND param_name = :param_name"
            , soci::use(_sesn_rel->get_id())
            , soci::use(_sesn_rel_param.get<0>());
      }
      *(soci_session_.get())
          <<
          "INSERT INTO session_relation_parameter_basetype_text(relid, param_name, "
          "array_index, value) VALUES"
          "(:relid, :param_name, :array_index, :default_value) "
          , soci::use(_sesn_rel->get_id())
          , soci::use(_sesn_rel_param.get<0>())
          , soci::use(i)
          , soci::use(boost::get<string>(*it));
    }
    break;
    }
  }

  // register data
  boost::optional<og::core::session_parameter_ptr> sesn_p =
    _sesn_rel->get_parameter(boost::get<0>(_sesn_rel_param));

  if (!sesn_p.is_initialized())
  {
    list<parameter_value_variant> empty_list;
    _sesn_rel->get_parameters()->insert
    (std::make_pair(boost::get<0>(_sesn_rel_param),
                    session_parameter_ptr(new session_parameter(empty_list,
                                          boost::get<1>(_sesn_rel_param)))));

    sesn_p =
      _sesn_rel->get_parameter(boost::get<0>(_sesn_rel_param));
  }

  if (!sesn_p.is_initialized())
  {
    throw og::core::exception() <<
                                exception_message("unexpected. parameter not found.");
  }

  sesn_p.get()->values_.clear();
  sesn_p.get()->values_.assign(_arg.begin(), _arg.end());
}

void session::get_connected_from(string _to_id, list<string>& _rel_type_list,
                                 list<boost::tuple<session_object_ptr, session_relation_ptr>>* _sesn_objrel_list)
{
  list<string> id_list;
  list<string> rel_list;

  fetcher<string>::fetch_use1(
    *soci_session_
    , "SELECT SO_F.id_, SR.id_ FROM session_object SO_F "
    "INNER JOIN session_relation SR ON "
    "SR.from_id = SO_F.id_ "
    "INNER JOIN schema_relation R ON "
    "R.id_ = SR.schema_relation_id "
    "WHERE SR.to_id = :to_id "
    + where_condition<string>(_rel_type_list, "R.type")()
    , soci::use(_to_id, "to_id")
    , &id_list, &rel_list);

  get_object_relation(id_list, rel_list, _sesn_objrel_list);
}

void session::get_connected_to(string _from_id, list<string>& _rel_type_list,
                               list<boost::tuple<session_object_ptr, session_relation_ptr>>* _sesn_objrel_list)
{
  list<string> id_list;
  list<string> rel_list;

  fetcher<string>::fetch_use1(
    *soci_session_
    , "SELECT SO_T.id_, SR.id_ FROM session_object SO_T "
    "INNER JOIN session_relation SR ON "
    "SR.to_id = SO_T.id_ "
    "INNER JOIN schema_relation R ON "
    "R.id_ = SR.schema_relation_id "
    "WHERE SR.from_id = :from_id "
    + where_condition<string>(_rel_type_list, "R.type")()
    , soci::use(_from_id, "from_id")
    , &id_list, &rel_list);

  get_object_relation(id_list, rel_list, _sesn_objrel_list);
}

} // namespace core;
} // namespace og;
