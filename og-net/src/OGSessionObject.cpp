#include "og/net/stdafx.h"
#include "og/net/OGSessionObject.h"
#include "og/net/OGConverter.h"

#include "og/og_session_object.h"

namespace og
{
namespace net
{

OGSessionObject::OGSessionObject(OGSharedPtr<og::og_session_object> _o_ptr) :
  og_session_object_(_o_ptr)
{}
OGSessionObject::~OGSessionObject()
{
}

String^ OGSessionObject::get_id()
{
  return gcnew String(og_session_object_->get_id().c_str());
}
String^ OGSessionObject::get_instance_name()
{
  return gcnew String(og_session_object_->get_instance_name().c_str());
}

void OGSessionObject::set_instance_name(String^ _name)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_name, &str);

  og_session_object_->set_instance_name(str);
}
String^ OGSessionObject::get_comment()
{
  return gcnew String(og_session_object_->get_comment().c_str());
}

void OGSessionObject::set_comment(String^ _comment)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_comment, &str);

  og_session_object_->set_comment(str);
}
String^ OGSessionObject::get_revision()
{
  return gcnew String(og_session_object_->get_revision().c_str());
}
void OGSessionObject::set_revision(String^ _rev)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_rev, &str);

  og_session_object_->set_revision(str);
}
String^ OGSessionObject::get_create_date()
{
  return gcnew String(og_session_object_->get_create_date().c_str());
}
String^ OGSessionObject::get_update_date()
{
  return gcnew String(og_session_object_->get_update_date().c_str());
}
String^ OGSessionObject::get_schema_object_id()
{
  return gcnew String(og_session_object_->get_schema_object_id().c_str());
}
String^ OGSessionObject::get_schema_object_type()
{
  return gcnew String(og_session_object_->get_schema_object_type().c_str());
}
String^ OGSessionObject::get_schema_object_name()
{
  return gcnew String(og_session_object_->get_schema_object_name().c_str());
}
String^ OGSessionObject::get_schema_object_comment()
{
  return gcnew String(og_session_object_->get_schema_object_comment().c_str());
}
String^ OGSessionObject::get_schema_object_create_date()
{
  return gcnew String(
           og_session_object_->get_schema_object_create_date().c_str());
}
String^ OGSessionObject::get_schema_object_update_date()
{
  return gcnew String(
           og_session_object_->get_schema_object_update_date().c_str());
}

OGSessionRelation^ OGSessionObject::connect_to(OGSessionObject^ _sesn_obj,
    String^ _rel_type)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_rel_type, &str);

  return gcnew OGSessionRelation(OGSharedPtr<og::og_session_relation>
                                 (og_session_object_->connect_to(_sesn_obj->og_session_object_, str)));
}
OGSessionRelation^ OGSessionObject::connect_from(OGSessionObject^ _sesn_obj,
    String^ _rel_type)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_rel_type, &str);

  return gcnew OGSessionRelation(OGSharedPtr<og::og_session_relation>
                                 (og_session_object_->connect_from(_sesn_obj->og_session_object_, str)));
}

void OGSessionObject::disconnect(OGSessionObject^ _sesn_obj)
{
  og_session_object_->disconnect(_sesn_obj->og_session_object_);
}

void OGSessionObject::delete_object()
{
  og_session_object_->delete_object();
}

void OGSessionObject::delete_object(OGConnectionDirection _direction)
{
  og_session_object_->delete_object((og::core::connection_direction_enum)
                                    _direction);
}

OGSessionObject^ OGSessionObject::copy_object()
{
  return gcnew OGSessionObject(
           OGSharedPtr<og::og_session_object>(
             og_session_object_->copy_object()));
}

OGSessionObject^ OGSessionObject::copy_object(OGConnectionDirection _direction)
{
  return gcnew OGSessionObject(
           OGSharedPtr<og::og_session_object>(
             og_session_object_->copy_object((og::core::connection_direction_enum)
                 _direction)));
}

List<OGSessionObject^>^ OGSessionObject::get_connected_object()
{
  std::list<og::og_session_object_ptr> objs;
  og_session_object_->get_connected_object(&objs);

  List<OGSessionObject^>^ sesn_obj_list = gcnew List<OGSessionObject^>();
  OGConverter::convert_std_to_clr
  <std::list<og::og_session_object_ptr>, List<OGSessionObject^>>
      (&objs, sesn_obj_list);

  return sesn_obj_list;
}
List<OGSessionObject^>^ OGSessionObject::get_connected_object(
  List<String^>^ _rel_type_list)
{
  std::list<std::string> strl;
  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
      (_rel_type_list, &strl);

  std::list<og::og_session_object_ptr> objs;
  og_session_object_->get_connected_object(strl, &objs);

  List<OGSessionObject^>^ sesn_obj_list = gcnew List<OGSessionObject^>();
  OGConverter::convert_std_to_clr
  <std::list<og::og_session_object_ptr>, List<OGSessionObject^>>
      (&objs, sesn_obj_list);

  return sesn_obj_list;
}
List<OGSessionObject^>^ OGSessionObject::get_connected_object_from()
{
  std::list<og::og_session_object_ptr> objs;
  og_session_object_->get_connected_object_from(&objs);

  List<OGSessionObject^>^ sesn_obj_list = gcnew List<OGSessionObject^>();
  OGConverter::convert_std_to_clr
  <std::list<og::og_session_object_ptr>, List<OGSessionObject^>>
      (&objs, sesn_obj_list);

  return sesn_obj_list;
}

List<OGSessionObject^>^ OGSessionObject::get_connected_object_from(
  List<String^>^ _rel_type_list)
{
  std::list<std::string> strl;
  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
      (_rel_type_list, &strl);

  std::list<og::og_session_object_ptr> objs;
  og_session_object_->get_connected_object_from(strl, &objs);

  List<OGSessionObject^>^ sesn_obj_list = gcnew List<OGSessionObject^>();
  OGConverter::convert_std_to_clr
  <std::list<og::og_session_object_ptr>, List<OGSessionObject^>>
      (&objs, sesn_obj_list);

  return sesn_obj_list;
}
List<OGSessionObject^>^ OGSessionObject::get_connected_object_to()
{
  std::list<og::og_session_object_ptr> objs;
  og_session_object_->get_connected_object_to(&objs);

  List<OGSessionObject^>^ sesn_obj_list = gcnew List<OGSessionObject^>();
  OGConverter::convert_std_to_clr
  <std::list<og::og_session_object_ptr>, List<OGSessionObject^>>
      (&objs, sesn_obj_list);

  return sesn_obj_list;
}

List<OGSessionObject^>^ OGSessionObject::get_connected_object_to(
  List<String^>^ _rel_type_list)
{
  std::list<std::string> strl;
  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
      (_rel_type_list, &strl);

  List<OGSessionObject^>^ sesn_obj_list = gcnew List<OGSessionObject^>();
  std::list<og::og_session_object_ptr> objs;
  og_session_object_->get_connected_object_to(strl, &objs);

  OGConverter::convert_std_to_clr
  <std::list<og::og_session_object_ptr>, List<OGSessionObject^>>
      (&objs, sesn_obj_list);

  return sesn_obj_list;
}

//bool OGSessionObject::validate_connect_to(OGSessionObject^ _sesn_obj_ptr)
//{
//  return 	og_session_object_->validate_connect_to(
//            _sesn_obj_ptr->og_session_object_);
//}
OGConnectionValidationResult OGSessionObject::validate_connect_to(
  OGSessionObject^ _sesn_obj_ptr,
  String^ _rel_type)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_rel_type, &str);

  return (OGConnectionValidationResult)(int)(
           og_session_object_->validate_connect_to(
             _sesn_obj_ptr->og_session_object_, str));
}

//bool OGSessionObject::validate_connect_from(OGSessionObject^ _sesn_obj_ptr)
//{
//  return 	og_session_object_->validate_connect_from(
//            _sesn_obj_ptr->og_session_object_);
//}
OGConnectionValidationResult OGSessionObject::validate_connect_from(
  OGSessionObject^ _sesn_obj_ptr,
  String^ _rel_type)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_rel_type, &str);

  return (OGConnectionValidationResult)(int)(
           og_session_object_->validate_connect_from(
             _sesn_obj_ptr->og_session_object_, str));
}


List<String^>^ OGSessionObject::get_connectable_relation_type_to()
{
  std::list<std::string> strl;
  og_session_object_->get_connectable_relation_type_to(&strl);

  List<String^>^ rel_type_list = gcnew List<String^>();
  OGConverter::convert_std_to_clr<std::list<std::string>, List<String^>>
      (&strl, rel_type_list);

  return rel_type_list;
}

List<String^>^ OGSessionObject::get_connectable_relation_type_from()
{
  std::list<std::string> strl;
  og_session_object_->get_connectable_relation_type_from(&strl);

  List<String^>^ rel_type_list = gcnew List<String^>();
  OGConverter::convert_std_to_clr<std::list<std::string>, List<String^>>
      (&strl, rel_type_list);

  return rel_type_list;
}
Dictionary<String^, List<OGSchemaObject^>^>^
OGSessionObject::get_connectable_to()
{
  std::map<std::string, std::list<og::og_schema_object_ptr>> objm;
  og_session_object_->get_connectable_to(&objm);

  Dictionary<String^, List<OGSchemaObject^>^>^ rel_type_map = gcnew
      Dictionary<String^, List<OGSchemaObject^>^>();
  OGConverter::convert_std_to_clr
  <std::map<std::string, std::list<og::og_schema_object_ptr>>, Dictionary<String^, List<OGSchemaObject^>^>>
      (&objm, rel_type_map);
  return rel_type_map;
}
Dictionary<String^, List<OGSchemaObject^>^>^
OGSessionObject::get_connectable_from()
{
  std::map<std::string, std::list<og::og_schema_object_ptr>> objm;
  og_session_object_->get_connectable_from(&objm);

  Dictionary<String^, List<OGSchemaObject^>^>^ rel_type_map = gcnew
      Dictionary<String^, List<OGSchemaObject^>^>();
  OGConverter::convert_std_to_clr
  <std::map<std::string, std::list<og::og_schema_object_ptr>>, Dictionary<String^, List<OGSchemaObject^>^>>
      (&objm, rel_type_map);

  return rel_type_map;
}

/*Nullable*/OGSessionParameter^ OGSessionObject::get_parameter(
  String^ _param_name)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &str);

  boost::optional<og::og_session_parameter_ptr> o =
    og_session_object_->get_parameter(str);

  if (!o.is_initialized())
  {
    return nullptr;
  }
  else
  {
    OGSharedPtr<og::og_session_parameter> s = OGSharedPtr<og::og_session_parameter>
        (o.get());
    //return gcnew OGSessionParameter(s);
    return nullptr;
  }
}

void OGSessionObject::set_parameter(String^ _param_name,
                                    List<Object^>^ _values)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &str);

  std::list<og::core::parameter_value_variant> v;
  OGConverter::convert_clr_to_std
  <List<Object^>, std::list<og::core::parameter_value_variant>>
      (_values, &v);

  og_session_object_->set_parameter(str, v);
}


void OGSessionObject::set_parameter_value_integer(String^ _param_name,
    int _value)
{
  std::string p;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &p);

  og_session_object_->set_parameter_value(p, _value);
}
void OGSessionObject::set_parameter_value_real(String^ _param_name,
    double _value)
{
  std::string p;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &p);

  og_session_object_->set_parameter_value(p, _value);
}
void OGSessionObject::set_parameter_value_text(String^ _param_name,
    String^ _value)
{
  std::string p;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &p);

  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_value, &str);

  og_session_object_->set_parameter_value(p, str);
}
void OGSessionObject::set_parameter_value_bool(String^ _param_name,
    bool _value)
{
  std::string p;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &p);

  og_session_object_->set_parameter_value(p, _value ? 1 : 0);
}

void OGSessionObject::set_parameter_values_integer(String^ _param_name,
    List<int>^ _values)
{
  std::string p;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &p);

  std::list<int> vl;
  OGConverter::convert_value_clr_to_std<List<int>, std::list<int>>(_values, &vl);

  og_session_object_->set_parameter_values<int>(p, vl);
}
void OGSessionObject::set_parameter_values_real(String^ _param_name,
    List<double>^ _values)
{
  std::string p;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &p);

  std::list<double> vl;
  OGConverter::convert_value_clr_to_std<List<double>, std::list<double>>(_values,
      &vl);

  og_session_object_->set_parameter_values<double>(p, vl);
}
void OGSessionObject::set_parameter_values_text(String^ _param_name,
    List<String^>^ _value)
{
  std::string p;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &p);

  std::list<std::string> strl;
  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
      (_value, &strl);

  og_session_object_->set_parameter_values(p, strl);
}
void OGSessionObject::set_parameter_values_bool(String^ _param_name,
    List<bool>^ _value)
{
  std::string p;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &p);

  std::list<int> strl;
  OGConverter::convert_value_clr_to_std<List<bool>, std::list<int>>
      (_value, &strl);

  og_session_object_->set_parameter_values(p, strl);
}

int OGSessionObject::get_parameter_value_integer(String^ _param_name)
{
  std::string p;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &p);

  int v;
  og_session_object_->get_parameter_value<int>(p, &v);
  return v;
}
double OGSessionObject::get_parameter_value_real(String^ _param_name)
{
  std::string p;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &p);

  double v;
  og_session_object_->get_parameter_value<double>(p, &v);
  return v;
}
String^ OGSessionObject::get_parameter_value_text(String^ _param_name)
{
  std::string p;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &p);

  std::string vl;
  og_session_object_->get_parameter_value<string>(p, &vl);
  return gcnew String(vl.c_str());
}
bool OGSessionObject::get_parameter_value_bool(String^ _param_name)
{
  std::string p;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &p);

  int v;
  og_session_object_->get_parameter_value<int>(p, &v);
  return (v == 1);
}

List<int>^ OGSessionObject::get_parameter_values_integer(String^ _param_name)
{
  std::string p;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &p);

  List<int>^ _value = gcnew List<int>();
  std::list<int> vl;
  og_session_object_->get_parameter_values<int>(p, &vl);
  OGConverter::convert_value_std_to_clr
  <std::list<int>, List<int>>(&vl, _value);
  return _value;
}
List<double>^ OGSessionObject::get_parameter_values_real(String^ _param_name)
{
  std::string p;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &p);

  List<double>^ _value = gcnew List<double>();
  std::list<double> vl;
  og_session_object_->get_parameter_values<double>(p, &vl);
  OGConverter::convert_value_std_to_clr
  <std::list<double>, List<double>>(&vl, _value);
  return _value;
}
List<String^>^ OGSessionObject::get_parameter_values_text(String^ _param_name)
{
  std::string p;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &p);

  std::list<std::string> vl;
  og_session_object_->get_parameter_values<string>(p, &vl);

  List<String^>^ _value = gcnew List<String^>();
  OGConverter::convert_std_to_clr
  <std::list<std::string>, List<String^>>(&vl, _value);
  return _value;
}
List<bool>^ OGSessionObject::get_parameter_values_bool(String^ _param_name)
{
  std::string p;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &p);

  List<bool>^ _value = gcnew List<bool>();
  std::list<int> vl;
  og_session_object_->get_parameter_values<int>(p, &vl);
  OGConverter::convert_value_std_to_clr
  <std::list<int>, List<bool>>(&vl, _value);
  return _value;
}


}
}
