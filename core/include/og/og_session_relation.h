#pragma once
#include <string>
#include <boost/shared_ptr.hpp>
#include "og/og_session.h"
#include "og/core/session.h"
#include "og/core/session_relation.h"

namespace og
{
namespace core
{
class session_relation;
}
using namespace boost;
using namespace std;


class og_session_relation
{
public:
  og_session_relation(og::core::session_relation_ptr _rel_ptr);
  virtual ~og_session_relation();

  ///////////////////////////////
  // parameters
  ///////////////////////////////
  template <typename T>
  void set_parameter_value(std::string _param_name, T _value)
  {
    session_relation_ptr_->set_parameter_value(_param_name, _value);
	session_relation_ptr_->sync();
  }
  template <typename T>
  void set_parameter_values(std::string _param_name, list<T>& _values)
  {
    session_relation_ptr_->set_parameter_values(_param_name, _values);
	session_relation_ptr_->sync();
  }
  template <typename T>
  void get_parameter_value(std::string _param_name, T* _value)
  {
    session_relation_ptr_->get_parameter_value(_param_name, _value);
  }
  template <typename T>
  void get_parameter_values(std::string _param_name, list<T>* _values)
  {
    session_relation_ptr_->get_parameter_values(_param_name, _values);
  }

  boost::optional<og_session_parameter_ptr> get_parameter(
    std::string _param_name);
  void set_parameter(string _param_name,
                     list<og::core::parameter_value_variant>& _values);

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

  string get_from_id();
  string get_to_id();

  string get_schema_relation_id();
  string get_schema_relation_type();
  string get_schema_relation_name();
  string get_schema_relation_comment();
  string get_schema_relation_create_date();
  string get_schema_relation_update_date();
  // property ends.

  // member variables
  og::core::session_relation_ptr session_relation_ptr_;

};

}// namespace og
