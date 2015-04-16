#include "og/core/schema.h"
#include "og/core/mapping.h"
#include "og/core/serializer.h"
#include "og/core/xml_stream.h"
#include "og/core/where_condition.h"
#include "og/core/fetcher.h"


using namespace boost::property_tree;
using namespace boost::property_tree::xml_parser;
namespace pt = boost::posix_time;

namespace og
{
namespace core
{

schema::schema()
  : session_(NULL)
{ }

schema::~schema()
{ }

void schema::initialize(session* _session)
{
  session_ = _session;
}

schema_object_ptr schema::create_object(string _otype, string _oname)
{
  //create sequence
  int id_seed;
  *session_->soci_session_ <<
                           "INSERT INTO schema_object_seq(id_seed) VALUES(NULL)";
  *session_->soci_session_ << "SELECT MAX(id_seed) FROM schema_object_seq",
                           soci::into(id_seed);
  string oid(SCHEMA_OBJECT_ID_PREFIX + boost::lexical_cast<string>(id_seed));

  //create object
  schema_object_ptr schm_obj(new schema_object(this, oid, _otype, _oname));

  // set date
  pt::ptime now = pt::microsec_clock::local_time();
  schm_obj->set_create_date(pt::to_simple_string(now));
  schm_obj->set_update_date(pt::to_simple_string(now));

  // set revision
  schm_obj->set_revision(REVISION_INIT_VALUE);

  add_object(schm_obj);

  // drop updated/bulk sync flag
  //schm_obj->set_bulk_revision_up(false);
  //schm_obj->set_bulk_sync(false);

  return schm_obj;
}

void schema::add_object(schema_object_ptr _schm_obj)
{
//insert record
  *session_->soci_session_
      <<
      "INSERT INTO schema_object(id_, comment, type, "
      "name, revision, create_date, update_date) "
      "values(:id_, :comment, :type, "
      ":name, :revision, :create_date, :update_date)",
      soci::use(*(_schm_obj.get()));
}

schema_relation_ptr schema::create_relation(string _rel_type, string _rel_name,
    string _from_oid, string _to_oid)
{
  //create sequence
  int id_seed;
  *session_->soci_session_ <<
                           "INSERT INTO schema_relation_seq(id_seed) VALUES(NULL)";
  *session_->soci_session_ << "SELECT MAX(id_seed) FROM schema_relation_seq",
                           soci::into(id_seed);
  string rel_id(SCHEMA_RELATION_ID_PREFIX + boost::lexical_cast < string >
                (id_seed));

  //create object
  schema_relation_ptr schm_rel(new schema_relation(this, rel_id, _rel_type,
                               _rel_name,
                               _from_oid, _to_oid));

  // set date
  pt::ptime now = pt::microsec_clock::local_time();
  schm_rel->set_create_date(pt::to_simple_string(now));
  schm_rel->set_update_date(pt::to_simple_string(now));

  // set revision
  schm_rel->set_revision(REVISION_INIT_VALUE);

  add_relation(schm_rel);

  return schm_rel;
}

void schema::add_relation(schema_relation_ptr _schm_rel)
{
  //insert record
  *session_->soci_session_
      <<
      "INSERT INTO schema_relation(id_, from_id, to_id, comment, type, "
      "name, revision, create_date, update_date) "
      "VALUES(:id_, :from_id, :to_id, :comment, :type, "
      ":name, :revision, :create_date, :update_date)",
      soci::use(*(_schm_rel.get()));

  // drop updated/bulk sync flag
  //_schm_rel->set_bulk_revision_up(false);
  //_schm_rel->set_bulk_sync(false);
}

//schema_object_ptr schema::copy_object(schema_object _object)
//{
//  return schema_object_ptr();
//}

void schema::delete_object(string _oid)
{
  // delete from db
  *session_->soci_session_
      <<
      "DELETE FROM schema_object WHERE id_ = :id_",
      soci::use(_oid);
}


void schema::disconnect(string _rel_id)
{
  // delete from db
  *session_->soci_session_
      <<
      "DELETE FROM schema_relation WHERE id_ = :id_",
      soci::use(_rel_id);
}

void schema::sync_object(schema_object* _schm_obj)
{
  *(session_->soci_session_.get())
      <<
      "UPDATE schema_object SET comment = :comment, "
      " type = :type, name = :name, revision = :revision, "
      "create_date = :create_date, update_date = :update_date"
      " WHERE id_ = :id_",
      soci::use(*_schm_obj);
}
void schema::sync_relation(schema_relation* _schm_rel)
{
  *(session_->soci_session_.get())
      <<
      "UPDATE schema_relation SET from_id = :from_id, to_id = :to_id,"
      "comment = :comment, "
      " type = :type, name = :name, revision = :revision, "
      "create_date = :create_date, update_date = :update_date"
      " WHERE id_ = :id_",
      soci::use(*_schm_rel);
}

void schema::revision_up_object(schema_object* _schm_obj)
{
  string rev(_schm_obj->get_revision());
  _schm_obj->set_revision(revision_up_revision(rev));
}

void schema::revision_up_relation(schema_relation* _schm_rel)
{
  string rev(_schm_rel->get_revision());
  _schm_rel->set_revision(revision_up_revision(rev));
}

optional<schema_object_ptr> schema::get_object(string _id)
{
  schema_object_ptr schm_obj(new schema_object(this));
  *session_->soci_session_
      <<
      "SELECT * FROM schema_object WHERE id_ = :id_",
      soci::into(*schm_obj.get()),
      soci::use(_id);

  if(schm_obj->get_id().empty())
  {
    return optional<schema_object_ptr>();
  }

  // drop updated/bulk sync flag
  //schm_obj->set_bulk_revision_up(false);
  //schm_obj->set_bulk_sync(false);

  return optional<schema_object_ptr>(schm_obj);
}

void schema::get_object_type(list<string>* _type_list)
{
  fetcher<string>::fetch(
    *session_->soci_session_
    , "SELECT DISTINCT type FROM schema_object WHERE 1 = 1"
    , _type_list);
}
void schema::get_relation_type(list<string>* _rel_type_list)
{
  fetcher<string>::fetch(
    *session_->soci_session_
    , "SELECT DISTINCT type FROM schema_relation WHERE 1 = 1"
    , _rel_type_list);
}

void schema::get_object(list<string>& _oid_list,
                        list<schema_object_ptr>* _schm_obj_list)
{
  _schm_obj_list->clear();

  for(list<string>::iterator it = _oid_list.begin(); it != _oid_list.end(); ++it)
  {
    optional<schema_object_ptr> obj_ptr_opt = get_object(*it);
    if(obj_ptr_opt.is_initialized())
    {
      _schm_obj_list->push_back(obj_ptr_opt.get());
    }
  }
}

void schema::get_object(list<schema_object_ptr>* _schm_obj_list)
{
  _schm_obj_list->clear();
  get_object_by_query(where_clause<string>("1=1"), _schm_obj_list);
}

void schema::get_object_by_type(list<string>& _otype_list,
                                list<schema_object_ptr>* _schm_obj_list)
{
  get_object_by_query(where_condition<string>(_otype_list, "type"),
                      _schm_obj_list);
}

void schema::get_object_by_name(list<string>& _oname_list,
                                list<schema_object_ptr>* _schm_obj_list)
{
  get_object_by_query(where_condition<string>(_oname_list, "name"),
                      _schm_obj_list);
}

void schema::get_relation_by_type(list<string>& _rel_type_list,
                                  list<schema_relation_ptr>* _schm_rel_list)
{
  get_relation_by_query(where_condition<string>(_rel_type_list, "type"),
                        _schm_rel_list);
}

optional<schema_relation_ptr> schema::get_relation(string _rel_id)
{
  schema_relation_ptr schm_rel(new schema_relation(this));

  *session_->soci_session_
      <<
      "SELECT * FROM schema_relation WHERE id_ = :id_",
      soci::into(*(schm_rel.get())),
      soci::use(_rel_id);

  if(schm_rel->get_id().empty())
  {
    return optional<schema_relation_ptr>();
  }

  // drop updated/bulk sync flag
  //schm_rel->set_bulk_revision_up(false);
  //schm_rel->set_bulk_sync(false);

  return optional<schema_relation_ptr>(schm_rel);

}

void schema::get_relation(list<string>& _rel_id_list,
                          list<schema_relation_ptr>* _schm_rel_list)
{
  _schm_rel_list->clear();

  for(list<string>::iterator it = _rel_id_list.begin(); it != _rel_id_list.end();
      ++it)
  {
    optional<schema_relation_ptr> rel_ptr_opt = get_relation(*it);
    if(rel_ptr_opt.is_initialized())
    {
      _schm_rel_list->push_back(rel_ptr_opt.get());
    }
  }
}

void schema::get_relation(list<schema_relation_ptr>* _schm_rel_list)
{
  _schm_rel_list->clear();
  get_relation_by_query(where_clause<string>("1=1"), _schm_rel_list);
}

void schema::get_connected_object_to(string _from_o_id,
                                     list<string>& _rel_type_list,
                                     list<schema_object_ptr>* _schm_obj_list)
{
  _schm_obj_list->clear();

  get_connected_object_to_by_query(
    _from_o_id,
    where_condition<string>(_rel_type_list, "schema_relation.type"),
    _schm_obj_list);

}
void schema::get_connected_object_from(string _to_o_id,
                                       list<string>& _rel_type_list,
                                       list<schema_object_ptr>* _schm_obj_list)
{
  _schm_obj_list->clear();

  get_connected_object_from_by_query(
    _to_o_id,
    where_condition<string>(_rel_type_list, "schema_relation.type"),
    _schm_obj_list);
}

void schema::get_connected_relation_to(string _from_o_id,
                                       list<string>& _rel_type_list,
                                       list<schema_relation_ptr>* _schm_rel_list)
{
  _schm_rel_list->clear();

  get_relation_by_query(
    where_condition<string>(_rel_type_list, "type"),
    " AND from_id = '" + _from_o_id + "'",
    _schm_rel_list);

}
void schema::get_connected_relation_from(string _to_o_id,
    list<string>& _rel_type_list,
    list<schema_relation_ptr>* _schm_rel_list)
{
  _schm_rel_list->clear();

  get_relation_by_query(
    where_condition<string>(_rel_type_list, "type"),
    " AND to_id = '" + _to_o_id + "'",
    _schm_rel_list);
}

bool schema::validate_connect_by_type(string _otype_from, string _otype_to)
{
  return false;
}


bool schema::validate_connect_by_type_id(string _otype, string _oid_from,
    string _oid_to)
{

  return false;
}

template<class T>
void schema::get_object_by_query(const T& _query,
                                 list<schema_object_ptr>* _schm_obj_list)
{
  list<string> o_id_list;

  fetcher<string>::fetch(
    *session_->soci_session_
    , "SELECT id_ FROM schema_object WHERE 1 = 1" + _query()
    , &o_id_list);

  get_object(o_id_list, _schm_obj_list);
}

template<class T>
void schema::get_connected_object_to_by_query(string _from_o_id,
    const T& _query,
    list<schema_object_ptr>* _schm_obj_list)
{
  list<string> o_id_list;

  fetcher<string>::fetch_use1(
    *session_->soci_session_
    , "SELECT schema_object.id_ FROM schema_object INNER JOIN "
    "schema_relation ON schema_relation.to_id = schema_object.id_"
    " WHERE 1 = 1"
    + _query()
    + " AND schema_relation.from_id = :_from_o_id"
    , soci::use(_from_o_id)
    ,  &o_id_list);

  get_object(o_id_list, _schm_obj_list);
}

template<class T>
void schema::get_connected_object_from_by_query(string _to_o_id,
    const T& _query,
    list<schema_object_ptr>* _schm_obj_list)
{
  list<string> o_id_list;

  fetcher<string>::fetch_use1(
    *session_->soci_session_
    , "SELECT schema_object.id_ FROM schema_object INNER JOIN "
    "schema_relation ON schema_relation.from_id = schema_object.id_"
    " WHERE 1 = 1"
    + _query()
    + " AND schema_relation.to_id = :_to_o_id"
    , soci::use(_to_o_id)
    , &o_id_list);

  get_object(o_id_list, _schm_obj_list);
}

template<class T>
void schema::get_relation_by_query(const T& _query,
                                   list<schema_relation_ptr>* _schm_rel_list)
{
  get_relation_by_query(_query, "", _schm_rel_list);
}

template<class T>
void schema::get_relation_by_query(const T& _query,
                                   string _query_additional1,
                                   list<schema_relation_ptr>* _schm_rel_list)
{
  list<string> rel_id_list;

  fetcher<string>::fetch(
    *session_->soci_session_
    , "SELECT id_ FROM schema_relation WHERE 1 = 1"
    + _query_additional1
    + _query()
    , &rel_id_list);

  get_relation(rel_id_list, _schm_rel_list);
}

bool schema::import_from_file(string _path)
{
  // Create XML document
  boost::property_tree::ptree pt;
  xml_stream().read_from_file(_path, &pt);

  // schema parameter
  {
    BOOST_FOREACH(const ptree::value_type & child,
                  pt.get_child("og.schema.parameters"))
    {
      // deserialize
      schema_parameter_ptr schm_par(new schema_parameter(this));
      if(serializer<schema_parameter_ptr>::deserialize(child, schm_par))
      {
        insert_schema_param(schm_par, false);
        //import_parameter(schm_par, child);
      }
    }
  }

  // schema object
  {
    BOOST_FOREACH(const ptree::value_type & child,
                  pt.get_child("og.schema.objects"))
    {
      // deserialize
      schema_object_ptr schm_obj(new schema_object(this));

      if(serializer<schema_object_ptr>::deserialize(child, schm_obj))
      {
        import_object(schm_obj);
      }
    }
  }

  // schema relation
  {
    BOOST_FOREACH(const ptree::value_type & child,
                  pt.get_child("og.schema.relations"))
    {
      // deserialize
      schema_relation_ptr schm_rel(new schema_relation(this));

      if(serializer<schema_relation_ptr>::deserialize(child, schm_rel))
      {
        import_relation(schm_rel);
      }
    }
  }

  return true;
}

void schema::import_object(schema_object_ptr _schm_obj)
{
  add_object(_schm_obj);
}
void schema::import_relation(schema_relation_ptr _schm_rel)
{
  add_relation(_schm_rel);
}
/*
void schema::import_parameter(schema_parameter_ptr _schm_par,
                              const ptree::value_type& _pt)
{
  parameter_basetype_enum t = (parameter_basetype_enum)_schm_par->get_basetype();

  switch (t)
  {
  case parameter_basetype_enum::integer:
  {
    // deserialize
    parameter_basetype_integer base_i;
    if (serializer<parameter_basetype_integer*>::deserialize(_pt, &base_i))
    {
      //insert record
      insert_schema_basetype(_schm_par, base_i);
      insert_schema_param(_schm_par, false);
    }
  }
  break;

  case parameter_basetype_enum::real:
  {
    // deserialize
    parameter_basetype_real base_r;
    if (serializer<parameter_basetype_real*>::deserialize(_pt, &base_r))
    {
      //insert record
      insert_schema_basetype(_schm_par, base_r);
      insert_schema_param(_schm_par, false);
    }
  }
  break;

  case parameter_basetype_enum::text:
  {
    // deserialize
    parameter_basetype_text base_t;
    if (serializer<parameter_basetype_text*>::deserialize(_pt, &base_t))
    {
      //insert record
      insert_schema_basetype(_schm_par, base_t);
      insert_schema_param(_schm_par, false);
    }
  }
  break;
  }

}
*/
void schema::export_to_file(string _path)
{
  // Create XML document
  boost::property_tree::ptree pt;
  ptree& pt_1 = pt.add("og.schema", "");

  // schema parameter
  list<schema_parameter_ptr> schm_par_list;
  get_parameter(&schm_par_list);
  BOOST_FOREACH(schema_parameter_ptr s, schm_par_list)
  {
    // serialize
    serializer<schema_parameter_ptr>::serialize(pt_1, s);
  }

  // schema object
  list<schema_object_ptr> schm_obj_list;
  get_object(&schm_obj_list);
  BOOST_FOREACH(schema_object_ptr s, schm_obj_list)
  {
    // serialize
    serializer<schema_object_ptr>::serialize(pt_1, s);
  }

  // schema relation
  list<schema_relation_ptr> schm_rel_list;
  get_relation(&schm_rel_list);
  BOOST_FOREACH(schema_relation_ptr s, schm_rel_list)
  {
    // serialize
    serializer<schema_relation_ptr>::serialize(pt_1, s);
  }

  xml_stream().write_to_file(_path, pt);
}

void schema::purge()
{
  *session_->soci_session_ << "DELETE FROM schema_relation_parameter;";
  *session_->soci_session_ << "DELETE FROM schema_relation;";
  *session_->soci_session_ << "DELETE FROM schema_object_parameter;";
  *session_->soci_session_ << "DELETE FROM schema_object;";
  *session_->soci_session_ << "DELETE FROM schema_parameter_basetype_integer;";
  *session_->soci_session_ << "DELETE FROM schema_parameter_basetype_real;";
  *session_->soci_session_ << "DELETE FROM schema_parameter_basetype_text;";
  *session_->soci_session_ <<
                           "DELETE FROM schema_parameter_basetype_select_item;";
  *session_->soci_session_ << "DELETE FROM schema_parameter_basetype_select;";
}

template <>
void schema::insert_schema_basetype<parameter_basetype_integer>
(schema_parameter_ptr _schm_param,
 parameter_basetype_integer& _basetype)
{
  // parameter basetypes var copy
  //_schm_param->basetypes_value_ = _basetype;
  _basetype.id_ = _schm_param->get_id();

  *session_->soci_session_
      <<
      "INSERT INTO schema_parameter_basetype_integer(id_, default_value, system_min, system_max, warn_min, warn_max) "
      "VALUES (:id_, :default_value, :system_min, :system_max, :warn_min, :warn_max)"
//      , soci::use(_schm_param->get_id())
      , soci::use(_basetype);
}
template <>
void schema::insert_schema_basetype<parameter_basetype_real>
(schema_parameter_ptr _schm_param,
 parameter_basetype_real& _basetype)
{
  // parameter basetypes var copy
  //_schm_param->basetypes_value_ = _basetype;
  _basetype.id_ = _schm_param->get_id();

  *session_->soci_session_
      <<
      "INSERT INTO schema_parameter_basetype_real(id_, default_value, system_min, system_max, warn_min, warn_max) "
      "VALUES (:id_, :default_value, :system_min, :system_max, :warn_min, :warn_max)"
//	  , soci::use(_schm_param->get_id())
      , soci::use(_basetype);
}
template <>
void schema::insert_schema_basetype<parameter_basetype_text>
(schema_parameter_ptr _schm_param,
 parameter_basetype_text& _basetype)
{
  // parameter basetypes var copy
  //_schm_param->basetypes_value_ = _basetype;
  _basetype.id_ = _schm_param->get_id();

  *session_->soci_session_
      <<
      "INSERT INTO schema_parameter_basetype_text(id_, default_value, system_min, system_max, warn_min, warn_max) "
      "VALUES (:id_, :default_value, :system_min, :system_max, :warn_min, :warn_max)"
//      , soci::use(_schm_param->get_id())
      , soci::use(_basetype);
}

void schema::delete_parameter(string _param_id)
{
  // delete from db
  *session_->soci_session_
      <<
      "DELETE FROM schema_parameter WHERE id_ = :id_",
      soci::use(_param_id);
}

void schema::insert_schema_param(schema_parameter_ptr _schm_par, bool _initial)
{
  if (_initial)
  {
    //create sequence
    int id_seed;
    *session_->soci_session_ <<
                             "INSERT INTO schema_parameter_seq(id_seed) VALUES(NULL)";
    *session_->soci_session_ << "SELECT MAX(id_seed) FROM schema_parameter_seq",
                             soci::into(id_seed);
    string par_id( SCHEMA_PARAMETER_ID_PREFIX + boost::lexical_cast<string>
                   (id_seed) );
    _schm_par->set_id(par_id);

    // set date
    pt::ptime now = pt::microsec_clock::local_time();
    _schm_par->set_create_date(pt::to_simple_string(now));
    _schm_par->set_update_date(pt::to_simple_string(now));

    // set revision
    _schm_par->set_revision(REVISION_INIT_VALUE);
  }

  //insert record
  *session_->soci_session_
      <<
      "INSERT INTO schema_parameter(id_, comment, type, "
      "default_array_size, min_array_size, max_array_size, "
      "revision, create_date, update_date, basetype_name) "
      "values(:id_, :comment, :type,"
      ":default_array_size, :min_array_size, :max_array_size, "
      ":revision, :create_date, :update_date, :basetype_name)",
      soci::use(*(_schm_par.get()));

  parameter_basetype_enum t = (parameter_basetype_enum)_schm_par->get_basetype();

  switch (t)
  {
  case parameter_basetype_enum::integer:
  {
    parameter_basetype_integer* base_i = boost::get<parameter_basetype_integer>
                                         (&_schm_par->basetype_variant_);
    OG_LOG << "INTEGER:(def)" << base_i->default_value_;

    insert_schema_basetype(_schm_par, *base_i);
  }
  break;
  case parameter_basetype_enum::real:
  {
    parameter_basetype_real* base_r = boost::get<parameter_basetype_real>
                                      (&_schm_par->basetype_variant_);
    OG_LOG << "REAL:(def)" << base_r->default_value_;

    insert_schema_basetype(_schm_par, *base_r);
  }
  break;
  case parameter_basetype_enum::text:
  {
    parameter_basetype_text* base_t = boost::get<parameter_basetype_text>
                                      (&_schm_par->basetype_variant_);
    OG_LOG << "TEXT:(def)" << base_t->default_value_;

    insert_schema_basetype(_schm_par, *base_t);
  }
  break;
  }

}

void schema::get_object_parameters(string _o_id,
                                   list<boost::tuple<string, schema_parameter_ptr>>*
                                   _param_name_types)
{
  list<boost::tuple<string, string>> pname_id;

  tuple_fetcher<string, string>::fetch_use1(
    *session_->soci_session_
    , "SELECT schema_object_parameter.param_name, schema_parameter.id_ "
    "FROM schema_object_parameter "
    "INNER JOIN "
    "schema_parameter ON schema_object_parameter.pid = schema_parameter.id_ "
    "WHERE schema_object_parameter.schema_object_id = :_o_id "
    , soci::use(_o_id)
    , &pname_id);

  list<boost::tuple<string, string>>::iterator it = pname_id.begin();
  for (; it != pname_id.end(); it++)
  {
    optional<schema_parameter_ptr> p = get_parameter(it->get<1>());
    if (p.is_initialized())
    {
      _param_name_types->push_back(boost::tuple<string, schema_parameter_ptr>
                                   (it->get<0>(),
                                    p.get()));
    }
  }

}

void schema::get_relation_parameters(string _rel_id,
                                     list<boost::tuple<string, schema_parameter_ptr>>*
                                     _param_name_types)
{
  list<boost::tuple<string, string>> pname_id;

  tuple_fetcher<string, string>::fetch_use1(
    *session_->soci_session_
    , "SELECT schema_relation_parameter.param_name, schema_parameter.id_ "
    "FROM schema_relation_parameter "
    "INNER JOIN "
    "schema_parameter ON schema_relation_parameter.pid = schema_parameter.id_ "
    "WHERE schema_relation_parameter.schema_relation_id = :_o_id "
    , soci::use(_rel_id)
    , &pname_id);

  list<boost::tuple<string, string>>::iterator it = pname_id.begin();
  for (; it != pname_id.end(); it++)
  {
    optional<schema_parameter_ptr> p = get_parameter(it->get<1>());
    if (p.is_initialized())
    {
      _param_name_types->push_back(boost::tuple<string, schema_parameter_ptr>
                                   (it->get<0>(),
                                    p.get()));
    }
  }

}

parameter_basetype_enum
schema::get_object_parameter_basetype_enum_by_param_name(
  string _o_id,
  string _param_name)
{
  int param_name;

  *session_->soci_session_
      << "SELECT schema_parameter.parameter_name INNER JOIN "
      "schema_parameter ON schema_object_parameter.pid = schema_parameter.id_ "
      "WHERE schema_object_parameter.schema_object_id = :_o_id"
      "schema_object_parameter.param_name = :_param_name"
      , soci::use(_o_id)
      , soci::use(_param_name)
      , soci::into(param_name);

  return schema_parameter::convert_to_parameter_basetype_enum(param_name);
}
parameter_basetype_enum
schema::get_relation_parameter_basetype_enum_by_param_name(
  string _rel_id,
  string _param_name)
{
  int param_name;

  *session_->soci_session_
      << "SELECT schema_parameter.parameter_name INNER JOIN "
      "schema_parameter ON schema_relation_parameter.pid = schema_parameter.id_ "
      "WHERE schema_relatoin_parameter.schema_relation_id = :_rel_id"
      "schema_object_parameter.param_name = :_param_name"
      , soci::use(_rel_id)
      , soci::use(_param_name)
      , soci::into(param_name);

  return schema_parameter::convert_to_parameter_basetype_enum(param_name);
}

void schema::add_object_parameter_definition(string _o_id, string _param_name,
    schema_parameter_ptr _schm_par)
{
  schema_object_parameter schm_o_par(
    _o_id, _schm_par->get_id(), _param_name);

  *session_->soci_session_
      <<
      "INSERT INTO schema_object_parameter(schema_object_id, pid, param_name) "
      "VALUES (:schema_object_id, :pid, :param_name)",
      soci::use(schm_o_par);
}
void schema::add_relation_parameter_definition(string _rel_id,
    string _param_name,
    schema_parameter_ptr _schm_par)
{
  schema_relation_parameter schm_r_par(
    _rel_id, _schm_par->get_id(), _param_name);

  *session_->soci_session_
      <<
      "INSERT INTO schema_relation_parameter(schema_relation_id, pid, param_name) "
      "VALUES (:schema_relation_id, :pid, :param_name)",
      soci::use(schm_r_par);
}

template <>
void schema::get_object_parameter_definition(string _o_id, string _param_name,
    parameter_basetype_integer* _param_basetype)
{
  *session_->soci_session_
      << "SELECT schema_parameter_basetype_integer.* INNER JOIN "
      "schema_parameter ON schema_object_parameter.pid = schema_parameter.id_ INNER JOIN"
      "schema_parameter_basetype_integer ON schema_parameter.id_ = schema_parameter_basetype_integer.param_id "
      "WHERE schema_object_parameter.schema_object_id = :_o_id"
      "schema_object_parameter.param_name = :_param_name"
      , soci::use(_o_id)
      , soci::use(_param_name)
      , soci::into(*_param_basetype);
}
template <>
void schema::get_object_parameter_definition(string _o_id, string _param_name,
    parameter_basetype_real* _param_basetype)
{
  *session_->soci_session_
      << "SELECT schema_parameter_basetype_real.* INNER JOIN "
      "schema_parameter ON schema_object_parameter.pid = schema_parameter.id_ INNER JOIN"
      "schema_parameter_basetype_real ON schema_parameter.id_ = schema_parameter_basetype_real.param_id "
      "WHERE schema_object_parameter.schema_object_id = :_o_id"
      "schema_object_parameter.param_name = :_param_name"
      , soci::use(_o_id)
      , soci::use(_param_name)
      , soci::into(*_param_basetype);
}
template <>
void schema::get_object_parameter_definition(string _o_id, string _param_name,
    parameter_basetype_text* _param_basetype)
{
  *session_->soci_session_
      << "SELECT schema_parameter_basetype_text.* INNER JOIN "
      "schema_parameter ON schema_object_parameter.pid = schema_parameter.id_ INNER JOIN"
      "schema_parameter_basetype_text ON schema_parameter.id_ = schema_parameter_basetype_text.param_id "
      "WHERE schema_object_parameter.schema_object_id = :_o_id"
      "schema_object_parameter.param_name = :_param_name"
      , soci::use(_o_id)
      , soci::use(_param_name)
      , soci::into(*_param_basetype);
}

template <>
void schema::get_relation_parameter_definition(string _rel_id,
    string _param_name,
    parameter_basetype_integer* _param_basetype)
{
  *session_->soci_session_
      << "SELECT schema_parameter_basetype_integer.* INNER JOIN "
      "schema_parameter ON schema_relation_parameter.pid = schema_parameter.id_ INNER JOIN"
      "schema_parameter_basetype_integer ON schema_parameter.id_ = schema_parameter_basetype_integer.param_id "
      "WHERE schema_relation_parameter.schema_relation_id = :_o_id"
      "schema_relation_parameter.param_name = :_param_name"
      , soci::use(_rel_id)
      , soci::use(_param_name)
      , soci::into(*_param_basetype);
}
template <>
void schema::get_relation_parameter_definition(string _rel_id,
    string _param_name,
    parameter_basetype_real* _param_basetype)
{
  *session_->soci_session_
      << "SELECT schema_parameter_basetype_real.* INNER JOIN "
      "schema_parameter ON schema_relation_parameter.pid = schema_parameter.id_ INNER JOIN"
      "schema_parameter_basetype_real ON schema_parameter.id_ = schema_parameter_basetype_real.param_id "
      "WHERE schema_relation_parameter.schema_relation_id = :_o_id"
      "schema_relation_parameter.param_name = :_param_name"
      , soci::use(_rel_id)
      , soci::use(_param_name)
      , soci::into(*_param_basetype);
}
template <>
void schema::get_relation_parameter_definition(string _rel_id,
    string _param_name,
    parameter_basetype_text* _param_basetype)
{
  *session_->soci_session_
      << "SELECT schema_parameter_basetype_text.* INNER JOIN "
      "schema_parameter ON schema_relation_parameter.pid = schema_parameter.id_ INNER JOIN"
      "schema_parameter_basetype_text ON schema_parameter.id_ = schema_parameter_basetype_text.param_id "
      "WHERE schema_relation_parameter.schema_relation_id = :_o_id"
      "schema_relation_parameter.param_name = :_param_name"
      , soci::use(_rel_id)
      , soci::use(_param_name)
      , soci::into(*_param_basetype);
}

void schema::get_parameter(list<schema_parameter_ptr>* _schm_par_list)
{
  _schm_par_list->clear();
  get_parameter_by_query(where_clause<string>("1=1"), _schm_par_list);
}

template<class T>
void schema::get_parameter_by_query(const T& _query,
                                    list<schema_parameter_ptr>* _schm_par_list)
{
  list<string> p_id_list;

  fetcher<string>::fetch(
    *session_->soci_session_
    , "SELECT schema_parameter.id_ FROM schema_parameter "
    " WHERE 1 = 1"
    + _query()
    , &p_id_list);

  get_parameter(p_id_list, _schm_par_list);
}

void schema::get_parameter(list<string>& _p_id_list,
                           list<schema_parameter_ptr>* _schm_par_list)
{
  _schm_par_list->clear();

  for(list<string>::iterator it = _p_id_list.begin(); it != _p_id_list.end();
      it++)
  {
    optional<schema_parameter_ptr> par_ptr_opt = get_parameter(*it);
    if(par_ptr_opt.is_initialized())
    {
      _schm_par_list->push_back(par_ptr_opt.get());
    }
  }
}

optional<schema_parameter_ptr> schema::get_parameter(string _par_id)
{
  schema_parameter_ptr schm_par(new schema_parameter(this));
  *session_->soci_session_
      <<
      "SELECT * FROM schema_parameter WHERE id_ = :id_",
      soci::into(*(schm_par.get())),
      soci::use(_par_id);

  if(schm_par->get_id().empty())
  {
    return optional<schema_parameter_ptr>();
  }

  switch((parameter_basetype_enum)schm_par->get_basetype())
  {
  case parameter_basetype_enum::integer:
  {
    parameter_basetype_integer* base(boost::get<parameter_basetype_integer>
                                     (&schm_par->basetype_variant_));

    *session_->soci_session_
        <<
//		"SELECT id_, default_value, system_min, system_max, warn_min, warn_max FROM schema_parameter_basetype_integer WHERE id_ = :id_",
        "SELECT * FROM schema_parameter_basetype_integer WHERE id_ = :id_",
        soci::into(*base),
        soci::use(schm_par->get_id());
  }
  break;
  case parameter_basetype_enum::real:
  {
    parameter_basetype_real* base(boost::get<parameter_basetype_real>
                                  (&schm_par->basetype_variant_));
    *session_->soci_session_
        <<
        "SELECT * FROM schema_parameter_basetype_real WHERE id_ = :id_",
        soci::into(*base),
        soci::use(schm_par->get_id());
  }
  break;
  case parameter_basetype_enum::text:
  {
    parameter_basetype_text* base(boost::get<parameter_basetype_text>
                                  (&schm_par->basetype_variant_));

    *session_->soci_session_
        <<
//		"SELECT id_, default_value, system_min, system_max, warn_min, warn_max FROM schema_parameter_basetype_text WHERE id_ = :id_",
        "SELECT * FROM schema_parameter_basetype_text WHERE id_ = :id_",
        soci::into(*base),
        soci::use(schm_par->get_id());
  }
  break;
  }

  return optional<schema_parameter_ptr>(schm_par);
}

string schema::revision_up_revision(const string& _revision)
{
  int rev = boost::lexical_cast<int>(_revision);
  return boost::lexical_cast<string>(++rev);
}
} // namespace core;
} // namespace og;
