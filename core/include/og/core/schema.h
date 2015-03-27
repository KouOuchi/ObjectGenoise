#pragma once

#include <list>
#include <string>
#include <vector>
#include <map>

#include "og/core/session.h"

#include "og/core/schema_parameter.h"
#include "og/core/schema_object.h"
#include "og/core/schema_relation.h"
//#include <boost/shared_ptr.hpp>
//#include <boost/scoped_ptr.hpp>
//#include <boost/weak_ptr.hpp>
#include <boost/optional.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tuple/tuple.hpp>

#include <soci.h>
//#include <sqlite3/soci-sqlite3.h>

namespace og
{
namespace core
{
using std::string;
using std::list;
using std::vector;
using std::map;

//nusing boost::shared_ptr;
//using boost::scoped_ptr;
//using boost::weak_ptr;
//using boost::optional;

class session;

class schema
{

public:
  schema();
  virtual ~schema();

  schema_object_ptr create_object(string _otype, string _oname);
  //schema_object_ptr copy_object(schema_object _object);
  void delete_object(string _oid);

  schema_relation_ptr create_relation(string _rel_type, string _rel_name,
                                      string _from_oid,
                                      string _to_oid);
//  void disconnect(string _to_oid, string _from_oid);
  void disconnect(string _rel_id);

  // ===> transaction and synchronizing
  //void sync();
  void sync_object(schema_object* _schm_obj);
  void sync_relation(schema_relation* _schm_rel);
  // <=== transaction and synchronizing

  void get_object(list<string>& _oid_list,
                  list<schema_object_ptr>* _schm_obj_list);
  void get_object(list<schema_object_ptr>* _schm_obj_list);
  optional<schema_object_ptr> get_object(string _oid);
  void get_object_by_type(list<string>& _otype_list,
                          list<schema_object_ptr>* _schm_obj_list);
  //necesarry?
  //void get_object_by_instance_name(list<string> _iname_list,
  //                                 list<schema_object_ref>* _schm_obj_list);
  void get_object_by_name(list<string>& _name_list,
                          list<schema_object_ptr>* _schm_obj_list);
  void get_object_type(list<string>* _type_list);

  void get_relation_by_type(list<string>& _otype_list,
                            list<schema_relation_ptr>* _schm_rel_list);
  void get_relation(list<string>& _relid_list,
                    list<schema_relation_ptr>* _schm_rel_list);
  void get_relation(list<schema_relation_ptr>* _schm_rel_list);
  optional<schema_relation_ptr> get_relation(string _relid);
  void get_relation_type(list<string>* _rel_type_list);

  void get_connected_object_to(string _from_o_id, list<string>& _rel_type_list,
                               list<schema_object_ptr>* _schm_object_list);
  void get_connected_object_from(string _to_o_id, list<string>& _rel_type_list,
                                 list<schema_object_ptr>* _schm_object_list);

  void get_connected_relation_to(string _from_o_id, list<string>& _rel_type_list,
                                 list<schema_relation_ptr>* _schm_rel_list);
  void get_connected_relation_from(string _to_o_id, list<string>& _rel_type_list,
                                   list<schema_relation_ptr>* _schm_rel_list);

  bool validate_connect_by_type(string _otype_from, string _otype_to);
  bool validate_connect_by_type_id(string _otype, string _oid_from,
                                   string _oid_to);

  ///////////////////////////////
  // parameters
  ///////////////////////////////
  template <typename P>
  schema_parameter_ptr create_parameter(string _param_type,
                                        string _comment, P* _basetype)
  {
    return create_parameter(_param_type, _comment, _basetype, 1, 1, 1);
  }
  template <typename P>
  schema_parameter_ptr create_parameter(string _param_type,
	  string _comment, P* _basetype, int _default_array_size, int _min_array_size,
	  int _max_array_size)
  {
    //create sequence
    int id_seed;
    *session_->soci_session_ <<
                             "INSERT INTO schema_parameter_seq(id_seed) VALUES(NULL)";
    *session_->soci_session_ << "SELECT MAX(id_seed) FROM schema_parameter_seq",
                             soci::into(id_seed);
    string pid( SCHEMA_PARAMETER_ID_PREFIX + boost::lexical_cast<string>(id_seed) );

    //create pointer. basetype_variant is filled
    schema_parameter_ptr schm_param(new schema_parameter(this,
                                    pid,
                                    _param_type, _comment, _default_array_size, _min_array_size, _max_array_size));

	//// manually setup
 //   switch (_basetype->basetype_id())
 //   {
 //   case parameter_basetype_enum::integer:
 //     schm_param->basetype_variant_ = _basetype;
 //     break;
 //   case parameter_basetype_enum::real:
 //     schm_param->basetype_variant_ = _basetype;
 //     break;
 //   case parameter_basetype_enum::text:
 //     schm_param->basetype_variant_ = _basetype;
 //     break;
 //   }
	schm_param->basetype_variant_ = *_basetype;

    // insert record
    insert_schema_param(schm_param, true);

    // drop updated flag
    //schm_param->set_updated(false);

    return schm_param;
  }

  void delete_parameter(string _param_id);
  void get_object_parameters(string _o_id,
                             list<boost::tuple<string, schema_parameter_ptr>>* _param_name_types);
  void get_relation_parameters(string _rel_id,
                               list<boost::tuple<string, schema_parameter_ptr>>* _param_name_types);
  optional<schema_parameter_ptr> get_parameter(string _par_id);
  void get_parameter(list<schema_parameter_ptr>* _schm_par_list);
  void get_parameter(list<string>& _p_id_list,
                     list<schema_parameter_ptr>* _schm_par_list);

  parameter_basetype_enum get_object_parameter_basetype_enum_by_param_name(
    string _o_id,
    string _param_name);
  parameter_basetype_enum get_relation_parameter_basetype_enum_by_param_name(
    string _rel_id,
    string _param_name);

  void add_object_parameter_definition(string _o_id, string _param_name,
                                       schema_parameter_ptr _schm_par);
  void add_relation_parameter_definition(string _rel_id, string _param_name,
                                         schema_parameter_ptr _schm_par);

  template <typename P>
  void get_object_parameter_definition(string _o_id, string _param_name,
                                       P* _param_basetype);
  template <typename P>
  void get_relation_parameter_definition(string _o_id, string _param_name,
                                         P* _param_basetype);
  // parameters end.


  // session
  session* session_;

  ///////////////////////////////
  // file io and management
  ///////////////////////////////
  void export_to_file(string _path);
  bool import_from_file(string _path);
  void purge();

  void initialize(session* _session);
  // file io and management end.

private:
  ///////////////////////////////
  // parameters
  ///////////////////////////////
  void insert_schema_param(schema_parameter_ptr _schm_par, bool );

  template <typename T>
  void insert_schema_basetype(schema_parameter_ptr _schm_param,
                              T& _basetype);
  template <>
  void schema::insert_schema_basetype<parameter_basetype_integer>
  (schema_parameter_ptr _schm_param,
   parameter_basetype_integer& _basetype);
  template <>
  void schema::insert_schema_basetype<parameter_basetype_real>
  (schema_parameter_ptr _schm_param,
   parameter_basetype_real& _basetype);
  template <>
  void schema::insert_schema_basetype<parameter_basetype_text>
  (schema_parameter_ptr _schm_param,
   parameter_basetype_text& _basetype);
  template <typename T>
  void get_parameter_by_query(const T& _query,
                              list<schema_parameter_ptr>* _schm_par_list);

  // parameters end.

  ///////////////////////////////
  // object
  ///////////////////////////////
  void add_object(schema_object_ptr _schm_obj);



  template<class T>
  void get_object_by_query(const T& _query,
                           list<schema_object_ptr>* _schm_obj_list);

  ///////////////////////////////
  // relation
  ///////////////////////////////
  template<class T>
  void get_relation_by_query(const T& _query,
                             string _query_additional1,
                             list<schema_relation_ptr>* _schm_rel_list);

  template<class T>
  void get_relation_by_query(const T& _query,
                             list<schema_relation_ptr>* _schm_rel_list);

  template<class T>
  void get_connected_object_to_by_query(string _from_o_id, const T& _query,
                                        list<schema_object_ptr>* _schm_obj_list);

  template<class T>
  void get_connected_object_from_by_query(string _to_o_id, const T& _query,
                                          list<schema_object_ptr>* _schm_obj_list);

  void add_relation(schema_relation_ptr _schm_rel);

  ///////////////////////////////
  // file io and management
  ///////////////////////////////
  void import_object(schema_object_ptr _schm_obj);
  void import_relation(schema_relation_ptr _schm_rel);
  void import_parameter(schema_parameter_ptr _schm_par,
                        const ptree::value_type& _pt);
};
} //namespace core;
} //namespace og;

