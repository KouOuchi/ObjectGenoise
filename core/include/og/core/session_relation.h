#pragma once
#include "session_base.h"
#include "session_parameter.h"

namespace og
{
namespace core
{

class session;

class session_relation : public session_base
{

public:
  OG_COREAPI session_relation(session* _session, string _id, schema_relation_ptr _schm_rel);
  OG_COREAPI session_relation(session* _session);
  OG_COREAPI virtual ~session_relation();

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

  void delete_relation();

  ///////////////////////////////
  // parameters
  ///////////////////////////////
  template <typename T>
  void set_parameter_value(std::string _param_name, T _value)
  {
    list<T> vs;
    get_parameter_values(_param_name, &vs);

    // swap 1st element
    vs.pop_front();
    vs.push_front(_value);

    set_parameter_values(_param_name, vs);
  }
  template <typename T>
  void set_parameter_values(std::string _param_name, list<T>& _values)
  {
    list<parameter_value_variant> vs;
    parameter_value_converter<T>().convert_from_list_to_variant_list
    (_values, &vs);

    set_parameter(_param_name, vs);
  }
  template <typename T>
  void get_parameter_value(std::string _param_name, T* _value)
  {
    // get only front element;
    list<T> vs;
    get_parameter_values(_param_name, &vs);
	*_value = *(vs.begin());
  }
  template <typename T>
  void get_parameter_values(std::string _param_name, list<T>* _values)
  {
    optional<session_parameter_ptr> param = get_parameter(_param_name);
    if (!param.is_initialized())
    {
      throw og::core::exception() << exception_message("param not found.");
    }

    if (param->get()->values_.size() == 0)
    {
      throw og::core::exception() << exception_message("param size is 0.");
    }

    parameter_value_converter<T>().convert_from_variant_list_to_list
    (param.get()->values_, _values);
  }

  OG_COREAPI optional<session_parameter_ptr> get_parameter(std::string _param_name);
  OG_COREAPI void set_parameter(string _param_name, list<parameter_value_variant>& _values);
  // <== parameter

  // schema ===>
  string get_schema_relation_id();
  schema_relation_ptr get_schema_relation();
  void set_schema_relation_by_id(string _id);
  void set_schema_relation(schema_relation_ptr _schm_rel);
  // forward to schema object
  virtual string get_type();
  // <=== schema

  virtual void sync();
  virtual void revision_up(){};

  session* get_session()
  {
    return session_;
  }
  map<string, session_parameter_ptr>* get_parameters()
  {
    return &session_parameter_;
  }

private:
  string from_id_;
  string to_id_;

  schema_relation_ptr schema_relation_ptr_;
  map<string, session_parameter_ptr> session_parameter_;
  session* session_;

};

} //namespace core
} //namespace og

