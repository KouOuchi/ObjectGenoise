#pragma once
#include <string>
#include <boost/shared_ptr.hpp>
#include "og/og_schema.h"
#include "og/core/schema.h"

namespace og
{
namespace core
{
class schema_parameter;
}
using namespace boost;
using namespace std;


class og_schema_parameter
{
public:
  og_schema_parameter(og::core::schema_parameter_ptr _rel_ptr);
  virtual ~og_schema_parameter();

  // member variables
  og::core::schema_parameter_ptr schema_parameter_ptr_;

  ///////////////////////////////
  // property
  ///////////////////////////////
  string get_id();
  string get_type();
  string get_name();
  void set_name(string _name);
  string get_comment();
  void set_comment(string _comment);
  string get_revision();
  string get_create_date();
  string get_update_date();

  // property ends.

};

}// namespace og
