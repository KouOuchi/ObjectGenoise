
#include "og/net/stdafx.h"

#include "og/net/OGSchemaRelation.h"
#include "og/net/OGConverter.h"

#include "og/og_schema_relation.h"

namespace og
{
namespace net
{

OGSchemaRelation::OGSchemaRelation(OGSharedPtr<og::og_schema_relation> _rel_ptr)
  :
  og_schema_relation_(_rel_ptr)
{
}
OGSchemaRelation::~OGSchemaRelation(void)
{
}

String^ OGSchemaRelation::get_id()
{
  return gcnew String(og_schema_relation_->get_id().c_str());
}
String^ OGSchemaRelation::get_type()
{
  return gcnew String(og_schema_relation_->get_type().c_str());
}
String^ OGSchemaRelation::get_name()
{
  return gcnew String(og_schema_relation_->get_name().c_str());
}
void OGSchemaRelation::set_name(String^ _name)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_name, &str);

  og_schema_relation_->set_name(str);
}
String^ OGSchemaRelation::get_comment()
{
  return gcnew String(og_schema_relation_->get_comment().c_str());
}
void OGSchemaRelation::set_comment(String^ _comment)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_comment, &str);

  og_schema_relation_->set_comment(str);
}
String^ OGSchemaRelation::get_revision()
{
  return gcnew String(og_schema_relation_->get_revision().c_str());
}
String^ OGSchemaRelation::get_create_date()
{
  return gcnew String(og_schema_relation_->get_create_date().c_str());
}
String^ OGSchemaRelation::get_update_date()
{
  return gcnew String(og_schema_relation_->get_update_date().c_str());
}
String^ OGSchemaRelation::get_from_id()
{
  return gcnew String(og_schema_relation_->get_from_id().c_str());
}
String^ OGSchemaRelation::get_to_id()
{
  return gcnew String(og_schema_relation_->get_to_id().c_str());
}
void OGSchemaRelation::get_parameters(
  List<Tuple<String^, OGSchemaParameter^>^>^
  _param_name_types)
{
  std::list<boost::tuple<std::string, og::og_schema_parameter_ptr>> ps;
  og_schema_relation_->get_parameters(&ps);

  OGConverter::convert_std_to_clr
  <std::list<boost::tuple<std::string, og::og_schema_parameter_ptr>>, List<Tuple<String^, OGSchemaParameter^>^>>
      (&ps, _param_name_types);
}

og::core::parameter_basetype_enum
OGSchemaRelation::get_parameter_basetype_enum_by_param_name(
  String^ _param_name)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &str);
  return og_schema_relation_->get_parameter_basetype_enum_by_param_name(str);
}


void OGSchemaRelation::add_parameter_definition(String^ _param_name,
    OGSchemaParameter^ _schm_par)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &str);

  og_schema_relation_->add_parameter_definition(str,
      _schm_par->og_schema_parameter_);
}
void OGSchemaRelation::revision_up()
{
  og_schema_relation_->revision_up();
}

}
}