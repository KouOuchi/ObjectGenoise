#pragma once
#include "og/core/schema_base.h"
#include "og/core/schema.h"
#include "og/core/schema_parameter.h"

namespace og
{
namespace core
{
class multiplicity;

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
  schema_relation_ptr connect_from(schema_object_ptr _from, string _oname, multiplicity& _from_mul, multiplicity& _to_mul);
  schema_relation_ptr connect_to(schema_object_ptr _to, string _otype, multiplicity& _from_mul, multiplicity& _to_mul);
  void disconnect();
  void disconnect(schema_object_ptr _schm_obj);

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

  // note : delete all parameters with session_object's
  void delete_parameter_definition(string _param_name,
                                   const schema_parameter_ptr _schm_par);

  template <typename P>
  void get_parameter_definition(string _param_name, P* _param_basetype) {}
  // parameters end.

  // by defualt, sync is automatically.
  virtual void sync();

  // by default, revision up is automatically.
  virtual void revision_up();

  static bool equals(const schema_object_ptr& _x, const schema_object_ptr& _y);

protected:

private:

};

} //namespace core;
} //namespace og;
