#include "og/og_schema_relation.h"
#include "og/core/schema_relation.h"
#include "og/core/schema.h"
#include "og/core/base.h"
#include "og/og_schema_parameter.h"

namespace og
{

og_schema_relation::og_schema_relation(og::core::schema_relation_ptr _o) :
  schema_relation_ptr_(_o)
{
}

og_schema_relation::~og_schema_relation()
{}

void og_schema_relation::get_parameters(
  list<boost::tuple<string, og_schema_parameter_ptr>>*
  _param_name_types)
{
  _param_name_types->clear();

  list<boost::tuple<string, og::core::schema_parameter_ptr>> types;
  schema_relation_ptr_->get_parameters(&types);

  list<boost::tuple<string, og::core::schema_parameter_ptr>>::iterator it =
        types.begin();

  for (; it != types.end(); it++)
  {
    _param_name_types->push_back(
      boost::make_tuple(it->get<0>(), og_schema_parameter_ptr(
                          new og_schema_parameter(it->get<1>()))));
  }
}

og::core::parameter_basetype_enum
og_schema_relation::get_parameter_basetype_enum_by_param_name(
  string _param_name)
{
  return schema_relation_ptr_->get_parameter_basetype_enum_by_param_name(
           _param_name);
}

void og_schema_relation::add_parameter_definition(string _param_name,
    const og_schema_parameter_ptr _schm_par)
{
  schema_relation_ptr_->add_parameter_definition(_param_name,
      _schm_par->schema_parameter_ptr_);
}

string og_schema_relation::get_id()
{
  return schema_relation_ptr_->get_id();
}
string og_schema_relation::get_type()
{
  return schema_relation_ptr_->get_type();
}
string og_schema_relation::get_name()
{
  return schema_relation_ptr_->get_name();
}
void og_schema_relation::set_name(string _name)
{
  schema_relation_ptr_->set_name(_name);
  schema_relation_ptr_->sync();
}
string og_schema_relation::get_comment()
{
  return schema_relation_ptr_->get_comment();
}
void og_schema_relation::set_comment(string _comment)
{
  schema_relation_ptr_->set_comment(_comment);
  schema_relation_ptr_->sync();
}
string og_schema_relation::get_revision()
{
  return schema_relation_ptr_->get_revision();
}
string og_schema_relation::get_create_date()
{
  return schema_relation_ptr_->get_create_date();
}
string og_schema_relation::get_update_date()
{
  return schema_relation_ptr_->get_update_date();
}
string og_schema_relation::get_from_id()
{
  return schema_relation_ptr_->get_from_id();
}
string og_schema_relation::get_to_id()
{
  return schema_relation_ptr_->get_to_id();
}


} // namespace og;
