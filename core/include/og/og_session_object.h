#pragma once
#include <string>
#include <boost/shared_ptr.hpp>
#include "og/og_schema.h"
#include "og/og_schema_relation.h"

#include "og/core/session_object.h"

namespace og
{
namespace core
{
class session_object;
}
using namespace boost;
using namespace std;

class og_session_object
{
public:
  og_session_object(og::core::session_object_ptr _o_ptr);
  virtual ~og_session_object();

  ///////////////////////////////
  // property
  ///////////////////////////////
  string get_id();
  string get_instance_name();
  void set_instance_name(string _name);
  string get_comment();
  void set_comment(string _comment);
  string get_revision();
  string get_create_date();
  string get_update_date();

  string get_schema_object_id();
  string get_schema_object_type();
  string get_schema_object_name();
  string get_schema_object_comment();
  string get_schema_object_create_date();
  string get_schema_object_update_date();
  // property ends.

  ///////////////////////////////
  // relation
  ///////////////////////////////
  og_session_relation_ptr connect_to(og_session_object_ptr _sesn_obj,
                                     string _rel_type);
  og_session_relation_ptr connect_from(og_session_object_ptr _sesn_obj,
                                       string _rel_type);
  void disconnect(og_session_object_ptr _sesn_obj);
  void delete_object();

  void get_connected_object(list<og_session_object_ptr>* _sesn_obj_list);
  void get_connected_object(list<string>& _rel_type_list,
                            list<og_session_object_ptr>* _sesn_obj_list);
  void get_connected_object_from(list<og_session_object_ptr>* _sesn_obj_list);
  void get_connected_object_from(list<string>& _rel_type_list,
                                 list<og_session_object_ptr>* _sesn_obj_list);
  void get_connected_object_to(list<og_session_object_ptr>* _sesn_obj_list);
  void get_connected_object_to(list<string>& _rel_type_list,
                               list<og_session_object_ptr>* _sesn_obj_list);
  bool validate_connect_to(og_session_object_ptr _sesn_obj_ptr);
  bool validate_connect_to(og_session_object_ptr _sesn_obj_ptr, string _rel_type);
  bool validate_connect_from(og_session_object_ptr _sesn_obj_ptr);
  bool validate_connect_from(og_session_object_ptr _sesn_obj_ptr,
                             string _rel_type);
  void get_connectable_relation_type_to(list<string>* _rel_type_list);
  void get_connectable_relation_type_from(list<string>* _rel_type_list);
  void get_connectable_to(map<string, list<og_schema_object_ptr> >*
                          _rel_type_map);
  void get_connectable_from(map<string, list<og_schema_object_ptr> >*
                            _rel_type_map);

  ///////////////////////////////
  // parameters
  ///////////////////////////////
  template <typename T>
  void set_parameter_value(std::string _param_name, T _value)
  {
    session_object_ptr_->set_parameter_value(_param_name, _value);
	session_object_ptr_->sync();
  }
  template <typename T>
  void set_parameter_values(std::string _param_name, list<T>& _values)
  {
    session_object_ptr_->set_parameter_values(_param_name, _values);
	session_object_ptr_->sync();
  }
  template <typename T>
  void get_parameter_value(std::string _param_name, T* _value)
  {
    session_object_ptr_->get_parameter_value(_param_name, _value);
  }
  template <typename T>
  void get_parameter_values(std::string _param_name, list<T>* _values)
  {
    session_object_ptr_->get_parameter_values(_param_name, _values);
  }

  boost::optional<og_session_parameter_ptr> get_parameter(string _param_name);
  void set_parameter(string _param_name,
                     list<og::core::parameter_value_variant>& _values);

  // object parameters end.

  // member variables
  og::core::session_object_ptr session_object_ptr_;
};

}// namespace og
