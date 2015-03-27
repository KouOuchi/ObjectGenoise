#pragma once
#include "og/core/session_base.h"
#include "og/core/schema_parameter.h"

namespace og
{
namespace core
{

class session;

/*template <typename T>*/
class session_parameter
{
public:
  //session_parameter(session* _session);
  session_parameter(list<parameter_value_variant> _values,
                    schema_parameter_ptr _schm_par);
  virtual ~session_parameter();

  /*list<T> values_;*/
  list<parameter_value_variant> values_;

  schema_parameter_ptr schema_parameter_;
  //session* session_;

};

} //namespace core;
} //namespace og;
