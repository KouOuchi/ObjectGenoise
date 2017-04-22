#include "og/og_schema_parameter.h"
#include "og/og_schema.h"
#include "og/core/schema_parameter.h"
#include "og/core/schema.h"

namespace og
{

og_schema_parameter::og_schema_parameter(og::core::schema_parameter_ptr _o) :
  schema_parameter_ptr_(_o)
{
}

og_schema_parameter::~og_schema_parameter()
{}

string og_schema_parameter::get_id()
{
  return schema_parameter_ptr_->get_id();
}
string og_schema_parameter::get_type()
{
  return schema_parameter_ptr_->get_type();
}
string og_schema_parameter::get_name()
{
  return schema_parameter_ptr_->get_name();
}
void og_schema_parameter::set_name(string _name)
{
  schema_parameter_ptr_->set_name(_name);
  schema_parameter_ptr_->sync();
}
string og_schema_parameter::get_comment()
{
  return schema_parameter_ptr_->get_comment();
}
void og_schema_parameter::set_comment(string _comment)
{
  schema_parameter_ptr_->set_comment(_comment);
  schema_parameter_ptr_->sync();
}
string og_schema_parameter::get_revision()
{
  return schema_parameter_ptr_->get_revision();
}
string og_schema_parameter::get_create_date()
{
  return schema_parameter_ptr_->get_create_date();
}
string og_schema_parameter::get_update_date()
{
  return schema_parameter_ptr_->get_update_date();
}

} // namespace og;

