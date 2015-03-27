#pragma once
#include "session_base.h"
#include "session_parameter.h"

namespace og
{
namespace core
{

class session;

class session_object : public session_base
{

public:
  session_object(session* _session, string _id, schema_object_ptr _schm_obj);
  session_object(session* _session);
  virtual ~session_object();

  void delete_object();

  ///////////////////////////////
  // relation
  ///////////////////////////////
  session_relation_ptr connect_to(session_object_ptr _sesn_obj, string _rel_type);
  session_relation_ptr connect_from(session_object_ptr _sesn_obj,
                                    string _rel_type);
  void disconnect(session_object_ptr _sesn_obj);

  void get_connected_object(list<session_object_ptr>* _sesn_obj_list);
  void get_connected_object(list<string>& _rel_type_list,
                            list<session_object_ptr>* _sesn_obj_list);
  void get_connected_object_from(list<session_object_ptr>* _sesn_obj_list);
  void get_connected_object_from(list<string>& _rel_type_list,
                                 list<session_object_ptr>* _sesn_obj_list);
  void get_connected_object_to(list<session_object_ptr>* _sesn_obj_list);
  void get_connected_object_to(list<string>& _rel_type_list,
                               list<session_object_ptr>* _sesn_obj_list);
  bool validate_connect_to(session_object_ptr _sesn_obj_ptr);
  bool validate_connect_to(session_object_ptr _sesn_obj_ptr, string _rel_type);
  bool validate_connect_from(session_object_ptr _sesn_obj_ptr);
  bool validate_connect_from(session_object_ptr _sesn_obj_ptr, string _rel_type);
  void get_connectable_relation_type_to(list<string>* _rel_type_list);
  void get_connectable_relation_type_from(list<string>* _rel_type_list);
  void get_connectable_to(map<string, list<schema_object_ptr> >* _rel_type_map);
  void get_connectable_from(map<string, list<schema_object_ptr> >* _rel_type_map);
  // relation ends.

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

    if(param->get()->values_.size() == 0)
    {
      throw og::core::exception() << exception_message("param size is 0.");
    }

    parameter_value_converter<T>().convert_from_variant_list_to_list
    (param.get()->values_, _values);
  }

  optional<session_parameter_ptr> get_parameter(string _param_name);
  void set_parameter(string _param_name, list<parameter_value_variant>& _values);

  // object parameters end.

  // schema ===>
  string get_schema_object_id();
  schema_object_ptr get_schema_object();
  void set_schema_object_by_id(string _id);
  // forward to schema object
  virtual string get_type();
  void set_schema_object(schema_object_ptr _schm_obj);
  // <=== schema

  virtual void sync();

  // accessor
  session* get_session()
  {
    return session_;
  }
  map<string, session_parameter_ptr>* get_parameters()
  {
    return &session_parameter_;
  }

private:
  schema_object_ptr schema_object_ptr_;
  map<string, session_parameter_ptr> session_parameter_;
  session* session_;

};

} //namespace core;
} //namespace og;
