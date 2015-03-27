#include "og/net/stdafx.h"

#include "og/net/OGSchemaParameter.h"
#include "og/net/OGConverter.h"

#include "og/og_schema_parameter.h"

namespace og
{
namespace net
{

OGSchemaParameter::OGSchemaParameter(OGSharedPtr < og::og_schema_parameter >
                                     _p_ptr) :
  og_schema_parameter_(_p_ptr)
{
}
OGSchemaParameter::~OGSchemaParameter()
{}

String^ OGSchemaParameter::get_id()
{
  return gcnew String(og_schema_parameter_->get_id().c_str());
}
String^ OGSchemaParameter::get_type()
{
  return gcnew String(og_schema_parameter_->get_type().c_str());
}
String^ OGSchemaParameter::get_name()
{
  return gcnew String(og_schema_parameter_->get_name().c_str());
}
void OGSchemaParameter::set_name(String^ _name)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_name, &str);

  og_schema_parameter_->set_comment(str);
}
String^ OGSchemaParameter::get_comment()
{
  return gcnew String(og_schema_parameter_->get_comment().c_str());
}
void OGSchemaParameter::set_comment(String^ _comment)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_comment, &str);

  og_schema_parameter_->set_comment(str);
}
String^ OGSchemaParameter::get_revision()
{
  return gcnew String(og_schema_parameter_->get_comment().c_str());
}
String^ OGSchemaParameter::get_create_date()
{
  return gcnew String(og_schema_parameter_->get_create_date().c_str());
}
String^ OGSchemaParameter::get_update_date()
{
  return gcnew String(og_schema_parameter_->get_update_date().c_str());
}

OGInteger::OGInteger()
  : og_basetype_integer_(new og::core::parameter_basetype_integer())
{}

OGInteger::OGInteger(int _def, int _smin, int _smax, int _wmin, int _wmax)
  : og_basetype_integer_(new og::core::parameter_basetype_integer())
{
  og_basetype_integer_->default_value_ = _def;
  og_basetype_integer_->system_min_ = _smin;
  og_basetype_integer_->system_max_ = _smax;
  og_basetype_integer_->warn_min_ = _wmin;
  og_basetype_integer_->warn_max_ = _wmax;
}

int OGInteger::get_default_value() { return og_basetype_integer_->default_value_; }
int OGInteger::get_system_min() { return og_basetype_integer_->system_min_; }
int OGInteger::get_system_max() { return og_basetype_integer_->system_max_; }
int OGInteger::get_warn_min() { return og_basetype_integer_->warn_min_; }
int OGInteger::get_warn_max() { return og_basetype_integer_->warn_max_; }

OGReal::OGReal()
  : og_basetype_real_(new og::core::parameter_basetype_real())
{}
OGReal::OGReal(double _def, double _smin, double _smax, double _wmin,
               double _wmax)
  : og_basetype_real_(new og::core::parameter_basetype_real())
{
  og_basetype_real_->default_value_ = _def;
  og_basetype_real_->system_min_ = _smin;
  og_basetype_real_->system_max_ = _smax;
  og_basetype_real_->warn_min_ = _wmin;
  og_basetype_real_->warn_max_ = _wmax;
}
double OGReal::get_default_value() {return og_basetype_real_->default_value_;}
double OGReal::get_system_min() {return og_basetype_real_->system_min_;}
double OGReal::get_system_max() {return og_basetype_real_->system_max_;}
double OGReal::get_warn_min() {return og_basetype_real_->warn_min_;}
double OGReal::get_warn_max() {return og_basetype_real_->warn_max_;}

OGText::OGText()
  : og_basetype_text_(new og::core::parameter_basetype_text())
{}
OGText::OGText(String^ _def, int _smin, int _smax, int _wmin, int _wmax)
  : og_basetype_text_(new og::core::parameter_basetype_text())
{
  std::string str1;
  OGConverter::convert_clr_to_std<String, std::string>(_def, &str1);

  og_basetype_text_->default_value_ = str1;
  og_basetype_text_->system_min_ = _smin;
  og_basetype_text_->system_max_ = _smax;
  og_basetype_text_->warn_min_ = _wmin;
  og_basetype_text_->warn_max_ = _wmax;
}

String^ OGText::get_default_value()
{
  return gcnew String(og_basetype_text_->default_value_.c_str());
}
int OGText::get_system_min() {return og_basetype_text_->system_min_;}
int OGText::get_system_max() {return og_basetype_text_->system_max_;}
int OGText::get_warn_min() { return og_basetype_text_->warn_min_;}
int OGText::get_warn_max() {return og_basetype_text_->warn_max_;}

OGBool::OGBool()
  : og_basetype_integer_(new og::core::parameter_basetype_integer())
{}

OGBool::OGBool(bool _def)
  : og_basetype_integer_(new og::core::parameter_basetype_integer())
{
  og_basetype_integer_->default_value_ = _def ? 1 : 0;
  og_basetype_integer_->system_min_ = 0;
  og_basetype_integer_->system_max_ = 1;
  og_basetype_integer_->warn_min_ = 0;
  og_basetype_integer_->warn_max_ = 1;
}

bool OGBool::get_default_value() { return (og_basetype_integer_->default_value_ == 1); }

}
}

