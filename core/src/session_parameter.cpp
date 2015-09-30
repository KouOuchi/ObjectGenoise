#include "og/stdafx.h"

#include "og/core/session_parameter.h"

#include "og/core/schema.h"

#include "og/core/exception.h"
#include "og/core/mapping.h"

namespace og
{
namespace core
{
/*
session_parameter::session_parameter(session* _session)
{}
*/

session_parameter::session_parameter(list<parameter_value_variant> _values, schema_parameter_ptr _schm_par) :
values_(_values), schema_parameter_(_schm_par)
{

}

session_parameter::~session_parameter()
{
}




} //namespace core;
} //namespace og;
