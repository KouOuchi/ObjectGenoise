#pragma once

#include "og/core/base.h"

#include <boost/property_tree/ptree.hpp>

//#include <soci.h>

namespace soci
{
class session;
} // namespace soci

namespace og
{
namespace core
{

using namespace boost::property_tree;

class schema;

class session
{

public:

  session(void);
  virtual ~session();

  ///////////////////////////////
  // file io and management
  ///////////////////////////////
  bool import_from_file(string _path);
  void export_to_file(string _path);

  void build();
  void connect(string _connection_string);
  void purge();
  // file io and management end

  ///////////////////////////////
  // object
  ///////////////////////////////
  session_object_ptr create_object(schema_object_ptr _schm_obj);
  void delete_object(string _id);

  optional<session_object_ptr> get_object(string _id);
  void get_object(list<session_object_ptr>* _sesn_obj_list);
  void get_object(list<string>& _oid_list,
                  list<session_object_ptr>* _sesn_obj_list);

  void get_object_by_type(list<string>& _otype_list,
                          list<session_object_ptr>* _sesn_obj_list);

  void get_object_by_name(list<string>& _oname_list,
                          list<session_object_ptr>* _sesn_obj_list);

  void get_object_by_schema_object_name(list<string>& _schm_obj_oname_list,
                                        list<session_object_ptr>* _sesn_obj_list);

  void get_object_by_schema_object(list<schema_object_ptr>& _schm_obj_list,
                                   list<session_object_ptr>* _sesn_obj_list);

  bool import_object(session_object_ptr _sesn_obj,
                     const ptree& _param_elm);
  // object end



  ///////////////////////////////
  // relation
  ///////////////////////////////
  session_relation_ptr connect(string _from_id, string _to_id,
                               schema_relation_ptr _schm_rel);
  void get_connected_object_from(string _to_id, list<string> _rel_type_list,
                                 list<session_object_ptr>* _sesn_obj_list);
  void get_connected_object_to(string _from_id, list<string> _rel_type_list,
                               list<session_object_ptr>* _sesn_obj_list);
  bool validate_connect(string _from_id, string _to_id,
                        list<string> _rel_type_list);

  optional<session_relation_ptr> get_relation(string _rel_id);
  void get_relation(list<session_relation_ptr>* _sesn_rel_list);
  void get_relation(list<string> _rel_id_list,
                    list<session_relation_ptr>* _sesn_rel_list);
  void get_relation(string _from_id, string _to_id,
                    list<session_relation_ptr>* _sesn_rel_list);
  void get_relation_by_type(list<string> _rel_type_list,
                            list<session_relation_ptr>* _sesn_rel_list);
  void get_relation_by_name(list<string> _rel_name_list,
                            list<session_relation_ptr>* _sesn_rel_list);

  void disconnect(string _rel_id);
  bool import_relation(session_relation_ptr _sesn_rel,
                       const ptree& _param_elm);

  // relation end

  boost::scoped_ptr<schema> schema_;
  boost::scoped_ptr<soci::session> soci_session_;

  void sync_object(session_object* _sesn_obj);
  void sync_relation(session_relation* _sesn_rel);

  template<typename T, typename U>
  void export_parameter_to_file(T _schema_base, U _session_base, ptree& _elm)
  {
      ptree& p1 = _elm.add("parameters", "");

	  list<boost::tuple<string, schema_parameter_ptr>> param_name_types;
    _schema_base->get_parameters(&param_name_types);
    for (list<boost::tuple<string, schema_parameter_ptr>>::iterator pit =
           param_name_types.begin(); pit != param_name_types.end(); pit++)
    {
      ptree& pchild = p1.add("parameter", "");

      string param = pit->get<0>();
      pchild.put("<xmlattr>.param_name", param);
      parameter_basetype_enum t =
        schema_parameter::convert_to_parameter_basetype_enum(
          pit->get<1>()->get_basetype());

      pchild.put("<xmlattr>.basetype", (int)t);

      ptree& items_child = pchild.add("items", "");

      switch (t)
      {
      case parameter_basetype_enum::integer:
      {
        list<int> items;
        _session_base->get_parameter_values<int>(param, &items);
        int index = 0;
        BOOST_FOREACH(const int& item , items)
        {
          ptree& ichild = items_child.add("item", "");
          ichild.put("<xmlattr>.index", index++);
          ichild.put("<xmlattr>.value", item);
        }
      }
      break;

      case parameter_basetype_enum::real:
      {
        list<double> items;
        _session_base->get_parameter_values<double>(param, &items);
        int index = 0;
        BOOST_FOREACH(const double& item, items)
        {
          ptree& ichild = items_child.add("item", "");
          ichild.put("<xmlattr>.index", index++);
          ichild.put("<xmlattr>.value", item);
        }
      }
      break;

      case parameter_basetype_enum::text:
      {
        list<string> items;
        _session_base->get_parameter_values<string>(param, &items);
        int index = 0;
        BOOST_FOREACH(const string& item, items)
        {
          ptree& ichild = items_child.add("item", "");
          ichild.put("<xmlattr>.index", index++);
          ichild.put("<xmlattr>.value", item);
        }
      }
      break;

      }
    }
  }

private:
  void initialize_sqlite_sequence(string _tbl);
  void initialize_sqlite_session();

  template<class T>
  void get_object_by_query(const T& _query,
                           list<session_object_ptr>* _sesn_obj_list);

  template<class T>
  void get_relation_by_query(const T& _query,
                             list<session_relation_ptr>* _sesn_rel_list);

  void get_sequence(string* _id);

  ///////////////////////////////
  // parameters
  ///////////////////////////////
  void sync_object_parameter(
    string _obj_id, string _param_name,
    session_parameter_ptr _sesn_parameter, parameter_basetype_enum _basetype);
  void sync_relation_parameter(
    string _rel_id, string _param_name,
    session_parameter_ptr _sesn_parameter, parameter_basetype_enum _basetype);

  // worker method
  void import_parameter_from_file(const ptree& _param_elm,
                                  map<string, list<parameter_value_variant>>* _params);

};

}// namespace core
}// namespace og
