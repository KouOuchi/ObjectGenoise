#pragma once
#include <string>
#include <boost/shared_ptr.hpp>
#include "og/og_schema.h"

namespace og
{
namespace core
{
class session_parameter;
}
using namespace boost;
using namespace std;


class og_session_parameter
{
public:
  OG_COREAPI og_session_parameter(
    list<og::core::parameter_value_variant > _values,
    og_schema_parameter_ptr _schm_par);

  OG_COREAPI virtual ~og_session_parameter();

  // member variables
  og::core::session_parameter_ptr session_parameter_ptr_;
};

}// namespace og
