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
  OG_COREAPI og_session_relation(og::core::session_relation_ptr _rel_ptr);
  OG_COREAPI virtual ~og_session_relation();

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

  OG_COREAPI boost::optional<og_session_parameter_ptr> get_parameter(
    std::string _param_name);
  OG_COREAPI void set_parameter(string _param_name,
                     list<og::core::parameter_value_variant>& _values);

  ///////////////////////////////
  // property
  ///////////////////////////////
  OG_COREAPI string get_id();
  OG_COREAPI string get_instance_name();
  OG_COREAPI void set_instance_name(string _name);
  OG_COREAPI string get_comment();
  OG_COREAPI void set_comment(string _comment);
  OG_COREAPI string get_revision();
  OG_COREAPI void set_revision(string _rev);
  OG_COREAPI string get_create_date();
  OG_COREAPI string get_update_date();

  OG_COREAPI string get_from_id();
  OG_COREAPI string get_to_id();

  OG_COREAPI string get_schema_relation_id();
  OG_COREAPI string get_schema_relation_type();
  OG_COREAPI string get_schema_relation_name();
  OG_COREAPI string get_schema_relation_comment();
  OG_COREAPI string get_schema_relation_create_date();
  OG_COREAPI string get_schema_relation_update_date();
  // property ends.

  // member variables
  og::core::session_relation_ptr session_relation_ptr_;

};

}// namespace og
