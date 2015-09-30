#include "og/stdafx.h"

#include "og/og_session_parameter.h"
#include "og/og_schema.h"
#include "og/core/schema_parameter.h"
#include "og/core/session_parameter.h"
#include "og/core/schema.h"

namespace og
{
og_session_parameter::og_session_parameter(
  list<og::core::parameter_value_variant > _values,
  og_schema_parameter_ptr _schm_par) : session_parameter_ptr_(
  new og::core::session_parameter( _values, _schm_par->schema_parameter_ptr_)
  )
{}

og_session_parameter::~og_session_parameter()
{}


} // namespace og;

