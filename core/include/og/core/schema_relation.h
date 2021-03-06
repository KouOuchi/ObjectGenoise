#pragma once
#include "schema_base.h"
#include "multiplicity.h"

namespace og
{
namespace core
{

class schema_relation : public schema_base
{

public:
  schema_relation(schema* _schema);
  schema_relation(schema* _schema, string _oid, string _rel_type,
                  string _rel_name, string _from_o_id, string _to_o_id);
  schema_relation(schema* _schema, string _oid, string _rel_type,
                  string _rel_name, string _from_o_id, string _to_o_id, multiplicity& _from_mul,
                  multiplicity& _to_mul);
  schema_relation();
  virtual ~schema_relation();

  string get_from_id() { return from_id_; }
  string get_to_id() { return to_id_; }
  void set_from_id(string _from_id)
  {
    from_id_ = _from_id;
    set_updated();
  }
  void set_to_id(string _to_id)
  {
    to_id_ = _to_id;
    set_updated();
  }

  ///////////////////////////////
  // parameters
  ///////////////////////////////
  void get_parameters(
    list<boost::tuple<string, schema_parameter_ptr>>*
    _param_name_types);
  parameter_basetype_enum get_parameter_basetype_enum_by_param_name(
    string _param_name);

  void add_parameter_definition(string _param_name,
                                const schema_parameter_ptr _schm_par);
  void delete_parameter_definition(string _param_name,
                                   const schema_parameter_ptr _schm_par);

  template <typename P>
  void get_parameter_definition(string _param_name, P* _param_basetype) {}

  void delete_relation();
  //void insert_parameter(string _param_name, string _pid);

  /*
  template <>
  void get_parameter_definition(string _param_name,
                                parameter_basetype_boolean* _param_basetype);
  */
  // parameters end.



  // concrete object  --->
  //session_relation_ptr create_session_relation();
  // <--- concrete object

  ///////////////////////////////
  // multiplicity
  ///////////////////////////////
  multiplicity& get_from_multiplicity() { return from_multiplicity_; }
  multiplicity& get_to_multiplicity() { return to_multiplicity_; }
  void set_from_multiplicity(multiplicity& _mul)
  { from_multiplicity_.set_multiplicity(_mul.get_min(), _mul.get_max()); }
  void set_to_multiplicity(multiplicity& _mul)
  { to_multiplicity_.set_multiplicity(_mul.get_min(), _mul.get_max()); }
  // multiplicity ends.

  // by defualt, sync is automatically.
  virtual void sync();

  // by default, revision up is automatically.
  virtual void revision_up();

private:
  string from_id_;
  string to_id_;

  multiplicity from_multiplicity_;
  multiplicity to_multiplicity_;
};


} //namespace core;
} //namespace og;
