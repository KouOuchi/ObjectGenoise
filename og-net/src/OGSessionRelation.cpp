#include "og/net/stdafx.h"
#include "og/net/OGSessionRelation.h"
#include "og/net/OGSchemaRelation.h"
#include "og/net/OGConverter.h"

#include "og/og_session_relation.h"

namespace og
{
namespace net
{

OGSessionRelation::OGSessionRelation(OGSharedPtr<og_session_relation> _rel_ptr)
  :
  og_session_relation_(_rel_ptr)
{}
OGSessionRelation::~OGSessionRelation()
{}

String^ OGSessionRelation::get_id()
{
  return gcnew String(og_session_relation_->get_id().c_str());
}
String^ OGSessionRelation::get_instance_name()
{
  return gcnew String(og_session_relation_->get_instance_name().c_str());
}
void OGSessionRelation::set_instance_name(String^ _name)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_name, &str);

  og_session_relation_->set_instance_name(str);
}
String^ OGSessionRelation::get_comment()
{
  return gcnew String(og_session_relation_->get_comment().c_str());
}
void OGSessionRelation::set_comment(String^ _comment)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_comment, &str);

  og_session_relation_->set_comment(str);
}
String^ OGSessionRelation::get_revision()
{
  return gcnew String(og_session_relation_->get_revision().c_str());
}
void OGSessionRelation::set_revision(String^ _rev)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_rev, &str);

  og_session_relation_->set_revision(str);
}
String^ OGSessionRelation::get_create_date()
{
  return gcnew String(og_session_relation_->get_create_date().c_str());
}
String^ OGSessionRelation::get_update_date()
{
  return gcnew String(og_session_relation_->get_update_date().c_str());
}
String^ OGSessionRelation::get_from_id()
{
  return gcnew String(og_session_relation_->get_from_id().c_str());
}
String^ OGSessionRelation::get_to_id()
{
  return gcnew String(og_session_relation_->get_to_id().c_str());
}
String^ OGSessionRelation::get_schema_relation_id()
{
  return gcnew String(og_session_relation_->get_schema_relation_id().c_str());
}
String^ OGSessionRelation::get_schema_relation_type()
{
  return gcnew String(og_session_relation_->get_schema_relation_type().c_str());
}
String^ OGSessionRelation::get_schema_relation_name()
{
  return gcnew String(og_session_relation_->get_schema_relation_name().c_str());
}
String^ OGSessionRelation::get_schema_relation_comment()
{
  return gcnew String(
           og_session_relation_->get_schema_relation_comment().c_str());
}
String^ OGSessionRelation::get_schema_relation_create_date()
{
  return gcnew String(
           og_session_relation_->get_schema_relation_create_date().c_str());
}
String^ OGSessionRelation::get_schema_relation_update_date()
{
  return gcnew String(
           og_session_relation_->get_schema_relation_update_date().c_str());
}


/*Nullable*/OGSessionParameter^ OGSessionRelation::get_parameter(
  String^ _param_name)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &str);

  boost::optional<og::og_session_parameter_ptr> o =
    og_session_relation_->get_parameter(str);

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

void OGSessionRelation::set_parameter(String^ _param_name,
                                      List<Object^>^ _values)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &str);

  std::list<og::core::parameter_value_variant> v;
  OGConverter::convert_clr_to_std
  <List<Object^>, std::list<og::core::parameter_value_variant>>
      (_values, &v);

  og_session_relation_->set_parameter(str, v);
}


void OGSessionRelation::set_parameter_value_integer(String^ _param_name,
    int _value)
{
  std::string p;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &p);

  og_session_relation_->set_parameter_value(p, _value);
}
void OGSessionRelation::set_parameter_value_real(String^ _param_name,
    double _value)
{
  std::string p;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &p);

  og_session_relation_->set_parameter_value(p, _value);
}
void OGSessionRelation::set_parameter_value_text(String^ _param_name,
    String^ _value)
{
  std::string p;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &p);

  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_value, &str);

  og_session_relation_->set_parameter_value(p, str);
}
void OGSessionRelation::set_parameter_value_bool(String^ _param_name,
    bool _value)
{
  std::string p;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &p);

  og_session_relation_->set_parameter_value(p, _value ? 1 : 0);
}

void OGSessionRelation::set_parameter_values_integer(String^ _param_name,
    List<int>^ _values)
{
  std::string p;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &p);

  std::list<int> vl;
  OGConverter::convert_value_clr_to_std<List<int>, std::list<int>>(_values, &vl);

  og_session_relation_->set_parameter_values<int>(p, vl);
}
void OGSessionRelation::set_parameter_values_real(String^ _param_name,
    List<double>^ _values)
{
  std::string p;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &p);

  std::list<double> vl;
  OGConverter::convert_value_clr_to_std<List<double>, std::list<double>>(_values,
      &vl);

  og_session_relation_->set_parameter_values<double>(p, vl);
}
void OGSessionRelation::set_parameter_values_text(String^ _param_name,
    List<String^>^ _value)
{
  std::string p;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &p);

  std::list<std::string> strl;
  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
      (_value, &strl);

  og_session_relation_->set_parameter_values(p, strl);
}
void OGSessionRelation::set_parameter_values_bool(String^ _param_name,
    List<bool>^ _value)
{
  std::string p;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &p);

  std::list<int> strl;
  OGConverter::convert_value_clr_to_std<List<bool>, std::list<int>>
      (_value, &strl);

  og_session_relation_->set_parameter_values(p, strl);
}

int OGSessionRelation::get_parameter_value_integer(String^ _param_name)
{
  std::string p;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &p);

  int v;
  og_session_relation_->get_parameter_value<int>(p, &v);
  return v;
}
double OGSessionRelation::get_parameter_value_real(String^ _param_name)
{
  std::string p;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &p);

  double v;
  og_session_relation_->get_parameter_value<double>(p, &v);
  return v;
}
String^ OGSessionRelation::get_parameter_value_text(String^ _param_name)
{
  std::string p;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &p);

  std::string vl;
  og_session_relation_->get_parameter_value<string>(p, &vl);
  return gcnew String(vl.c_str());
}
bool OGSessionRelation::get_parameter_value_bool(String^ _param_name)
{
  std::string p;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &p);

  int v;
  og_session_relation_->get_parameter_value<int>(p, &v);
  return (v == 1);
}

List<int>^ OGSessionRelation::get_parameter_values_integer(String^ _param_name)
{
  std::string p;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &p);

  List<int>^ _value = gcnew List<int>();
  std::list<int> vl;
  og_session_relation_->get_parameter_values<int>(p, &vl);
  OGConverter::convert_value_std_to_clr
  <std::list<int>, List<int>>(&vl, _value);
  return _value;
}
List<double>^ OGSessionRelation::get_parameter_values_real(String^ _param_name)
{
  std::string p;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &p);

  List<double>^ _value = gcnew List<double>();
  std::list<double> vl;
  og_session_relation_->get_parameter_values<double>(p, &vl);
  OGConverter::convert_value_std_to_clr
  <std::list<double>, List<double>>(&vl, _value);
  return _value;
}
List<String^>^ OGSessionRelation::get_parameter_values_text(String^ _param_name)
{
  std::string p;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &p);

  std::list<std::string> vl;
  og_session_relation_->get_parameter_values<string>(p, &vl);

  List<String^>^ _value = gcnew List<String^>();
  OGConverter::convert_std_to_clr
  <std::list<std::string>, List<String^>>(&vl, _value);
  return _value;
}
List<bool>^ OGSessionRelation::get_parameter_values_bool(String^ _param_name)
{
  std::string p;
  OGConverter::convert_clr_to_std<String, std::string>(_param_name, &p);

  List<bool>^ _value = gcnew List<bool>();
  std::list<int> vl;
  og_session_relation_->get_parameter_values<int>(p, &vl);
  OGConverter::convert_value_std_to_clr
  <std::list<int>, List<bool>>(&vl, _value);
  return _value;
}

}
}