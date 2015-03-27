#pragma once
#include "og/core/schema_base.h"
#include "og/core/schema.h"
#include "og/core/schema_parameter.h"

namespace og
{
namespace core
{

class schema_object : public schema_base
{

public:
  schema_object(schema* _schema);
  schema_object(schema* _schema, string _oid, string _otype, string _oname);
  virtual ~schema_object();
  ///////////////////////////////
  // relation
  ///////////////////////////////
  schema_relation_ptr connect_from(schema_object_ptr _from, string _oname);
  schema_relation_ptr connect_to(schema_object_ptr _to, string _otype);
//  void disconnect();
//  void disconnect(schema_object_ptr _object);

  schema_object_ptr copy_object();
  void delete_object();

  void get_connected_object(list<schema_object_ptr>* _schm_object_list);
  void get_connected_object(list<string>& _rel_type_list,
                            list<schema_object_ptr>* _schm_object_list);

  void get_connected_object_to(list<schema_object_ptr>* _schm_object_list);
  void get_connected_object_to(list<string>& _rel_type_list,
                               list<schema_object_ptr>* _schm_object_list);

  void get_connected_object_from(list<schema_object_ptr>* _schm_object_list);
  void get_connected_object_from(list<string>& _rel_type_list,
                                 list<schema_object_ptr>* _schm_object_list);

  ///////////////////////////////
  // parameters
  ///////////////////////////////
  void get_parameters(
    list<boost::tuple<string, schema_parameter_ptr>>*
    _param_name_types);

  parameter_basetype_enum  get_parameter_basetype_enum_by_param_name(
    string _param_name);

  void add_parameter_definition(string _param_name,
                                const schema_parameter_ptr _schm_par);

  template <typename P>
  void get_parameter_definition(string _param_name, P* _param_basetype) {}

  //void insert_parameter(string _param_name, string _pid);

  /*
  template <>
  void get_parameter_definition(string _param_name,
                                parameter_basetype_boolean* _param_basetype);
  */
  // parameters end.

  ///////////////////////////////
  // validation
  ///////////////////////////////
  bool validate_connect_From(string _otype);
  bool validate_connect_from(schema_object_ptr _obj, string _otype);
  bool validate_connect_to(string _otype);
  bool validate_connect_to(schema_object_ptr _obj, string _otype);
  bool validate_parameter_valuerange();
  // validations end.

  // concrete object  --->
//  session_object_ptr create_session_object();
  virtual void sync();

  // <--- concrete object

private:
  //map<string, schema_object_parameter> parameters_;
};

} //namespace core;
} //namespace og;
