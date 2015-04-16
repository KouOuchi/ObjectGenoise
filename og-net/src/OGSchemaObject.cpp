#include "og/net/stdafx.h"

#include "og/net/OGSchemaObject.h"
//#include "og/net/OGSchemaRelation.h"
//#include "og/net/OGSession.h"
#include "og/net/OGConverter.h"

#include "og/og_schema_object.h"

namespace og
{
namespace net
{

OGSchemaObject::OGSchemaObject(OGSharedPtr<og::og_schema_object> _o_ptr) :
  og_schema_object_(_o_ptr)
{}

OGSchemaObject::~OGSchemaObject()
{}


String^ OGSchemaObject::get_id()
{
  return gcnew String(og_schema_object_->get_id().c_str());
}
String^ OGSchemaObject::get_type()
{
  return gcnew String(og_schema_object_->get_type().c_str());
}
String^ OGSchemaObject::get_name()
{
  return gcnew String(og_schema_object_->get_name().c_str());
}
void OGSchemaObject::set_name(String^ _name)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_name, &str);

  og_schema_object_->set_name(str);
}
String^ OGSchemaObject::get_comment()
{
  return gcnew String(og_schema_object_->get_comment().c_str());
}
void OGSchemaObject::set_comment(String^ _comment)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_comment, &str);

  og_schema_object_->set_comment(str);
}
String^ OGSchemaObject::get_revision()
{
  return gcnew String(og_schema_object_->get_revision().c_str());
}

String^ OGSchemaObject::get_create_date()
{
  return gcnew String(og_schema_object_->get_create_date().c_str());
}

String^ OGSchemaObject::get_update_date()
{
  return gcnew String(og_schema_object_->get_update_date().c_str());
}

OGSchemaRelation^ OGSchemaObject::connect_from(OGSchemaObject^ _from,
    String^ _rel_type)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_rel_type, &str);

  return gcnew OGSchemaRelation(
           OGSharedPtr<og::og_schema_relation>(og_schema_object_->connect_from(
                 _from->og_schema_object_, str)));
}
OGSchemaRelation^ OGSchemaObject::connect_to(OGSchemaObject^ _to,
    String^ _rel_type)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_rel_type, &str);

  return gcnew OGSchemaRelation(
           OGSharedPtr<og::og_schema_relation>(og_schema_object_->connect_to(
                 _to->og_schema_object_, str)));
}

//void OGSchemaObject::disconnect()
//{
//  og_schema_object_->disconnect();
//}
//void OGSchemaObject::disconnect(OGSchemaObject^ _object)
//{
//  og_schema_object_->disconnect(_object->og_schema_object_);
//}

//OGSchemaObject^ OGSchemaObject::copy_object(void)
//{
//  return gcnew OGSchemaObject(
//           OGSharedPtr<og::og_schema_object>(
//             og_schema_object_->copy_object()));
//}
void OGSchemaObject::delete_object()
{
  og_schema_object_->delete_object();
}

void OGSchemaObject::get_connected_object(List<OGSchemaObject^>^
    _schm_obj_list)
{
  std::list<og::og_schema_object_ptr> objs;
  og_schema_object_->get_connected_object(&objs);

  OGConverter::convert_std_to_clr
  <std::list<og::og_schema_object_ptr>, List<OGSchemaObject^>>
      (&objs, _schm_obj_list);
}
void OGSchemaObject::get_connected_object(List<String^>^ _rel_type_list,
    List<OGSchemaObject^>^ _schm_obj_list)
{
  std::list<std::string> strl;
  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
      (_rel_type_list, &strl);

  std::list<og::og_schema_object_ptr> objs;
  og_schema_object_->get_connected_object(strl, &objs);

  OGConverter::convert_std_to_clr
  <std::list<og::og_schema_object_ptr>, List<OGSchemaObject^>>
      (&objs, _schm_obj_list);
}
void OGSchemaObject::get_connected_object_to(List<OGSchemaObject^>^
    _schm_obj_list)
{
  std::list<og::og_schema_object_ptr> objs;
  og_schema_object_->get_connected_object_to(&objs);

  OGConverter::convert_std_to_clr
  <std::list<og::og_schema_object_ptr>, List<OGSchemaObject^>>
      (&objs, _schm_obj_list);
}

void OGSchemaObject::get_connected_object_to(List<String^>^ _rel_type_list,
    List<OGSchemaObject^>^ _schm_obj_list)
{
  std::list<std::string> strl;
  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
      (_rel_type_list, &strl);

  std::list<og::og_schema_object_ptr> objs;
  og_schema_object_->get_connected_object_to(strl, &objs);

  OGConverter::convert_std_to_clr
  <std::list<og::og_schema_object_ptr>, List<OGSchemaObject^>>
      (&objs, _schm_obj_list);
}

void OGSchemaObject::get_connected_object_from(List<OGSchemaObject^>^
    _schm_obj_list)
{
  std::list<og::og_schema_object_ptr> objs;
  og_schema_object_->get_connected_object_from(&objs);

  OGConverter::convert_std_to_clr
  <std::list<og::og_schema_object_ptr>, List<OGSchemaObject^>>
      (&objs, _schm_obj_list);
}

void OGSchemaObject::get_connected_object_from(List<String^>^ _rel_type_list,
    List<OGSchemaObject^>^ _schm_obj_list)
{
  std::list<std::string> strl;
  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
      (_rel_type_list, &strl);

  std::list<og::og_schema_object_ptr> objs;
  og_schema_object_->get_connected_object_from(strl, &objs);

  OGConverter::convert_std_to_clr
  <std::list<og::og_schema_object_ptr>, List<OGSchemaObject^>>
      (&objs, _schm_obj_list);
}

void OGSchemaObject::get_parameters(
  List<Tuple<String^, OGSchemaParameter^>^>^
  _param_name_types)
{
  std::list<boost::tuple<std::string, og::og_schema_parameter_ptr>> ps;
  og_schema_object_->get_parameters(&ps);

  OGConverter::convert_std_to_clr
  <std::list<boost::tuple<std::string, og::og_schema_parameter_ptr>>, List<Tuple<String^, OGSchemaParameter^>^>>
      (&ps, _param_name_types);
}

og::core::parameter_basetype_enum
OGSchemaObject::get_parameter_basetype_enum_by_param_name(
  String^ _param_name)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &str);
  return og_schema_object_->get_parameter_basetype_enum_by_param_name(str);
}

void OGSchemaObject::add_parameter_definition(String^ _param_name,
    OGSchemaParameter^ _schm_par)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &str);

  og_schema_object_->add_parameter_definition(str,
      _schm_par->og_schema_parameter_);
}
void OGSchemaObject::revision_up()
{
  og_schema_object_->revision_up();
}
}
}
