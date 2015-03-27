#pragma once

#include "og/net/OGSchema.h"
#include "og/net/OGSharedPtr.h"

#include "og/core/og.h"

namespace og
{

class og_schema_parameter;

namespace net
{


public ref class OGInteger
{
public:
  OGInteger();
  OGInteger(int _def, int _smin, int _smax, int _wmin, int _wmax);
  int get_default_value();
  int get_system_min();
  int get_system_max();
  int get_warn_min();
  int get_warn_max();

internal:
  OGSharedPtr<og::core::parameter_basetype_integer> og_basetype_integer_;
};

public ref class OGReal
{
public:
  OGReal();
  OGReal(double _def, double _smin, double _smax, double _wmin, double _wmax);
  double get_default_value();
  double get_system_min();
  double get_system_max();
  double get_warn_min();
  double get_warn_max();

internal:
  OGSharedPtr<og::core::parameter_basetype_real> og_basetype_real_;
};

public ref class OGText
{
public:
  OGText();
  OGText(String^ _def, int _smin, int _smax, int _wmin, int _wmax);
  String^ get_default_value();
  int get_system_min();
  int get_system_max();
  int get_warn_min();
  int get_warn_max();

internal:
  OGSharedPtr<og::core::parameter_basetype_text> og_basetype_text_;
};

public ref class OGBool
{
public:
  OGBool();
  OGBool(bool _def);
  bool get_default_value();

internal:
	OGSharedPtr<og::core::parameter_basetype_integer> og_basetype_integer_;
};

public ref class OGSchemaParameter
{
public:
  OGSchemaParameter(OGSharedPtr<og::og_schema_parameter> _p_ptr);
  virtual ~OGSchemaParameter();

  ///////////////////////////////
  // property
  ///////////////////////////////
  String^ get_id();
  String^ get_type();
  String^ get_name();
  void set_name(String^ _name);
  String^ get_comment();
  void set_comment(String^ _comment);
  String^ get_revision();
  String^ get_create_date();
  String^ get_update_date();

  // property ends.

internal:
  OGSharedPtr<og::og_schema_parameter> og_schema_parameter_;

};

}
}
