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
  OG_COREAPI og_schema_parameter(og::core::schema_parameter_ptr _rel_ptr);
  OG_COREAPI virtual ~og_schema_parameter();

  // member variables
  og::core::schema_parameter_ptr schema_parameter_ptr_;

  ///////////////////////////////
  // property
  ///////////////////////////////
  OG_COREAPI string get_id();
  OG_COREAPI string get_type();
  OG_COREAPI string get_name();
  OG_COREAPI void set_name(string _name);
  OG_COREAPI string get_comment();
  OG_COREAPI void set_comment(string _comment);
  OG_COREAPI string get_revision();
  OG_COREAPI string get_create_date();
  OG_COREAPI string get_update_date();

  // property ends.

};

}// namespace og
