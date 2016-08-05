#pragma once

#include "og/net/OGSchema.h"
#include "og/net/OGSession.h"
#include "og/net/OGSharedPtr.h"

#include "og/og_session_relation.h"

namespace og
{
class og_session_relation;

namespace net
{

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;

public ref class OGSessionRelation
{
public:

  OGSessionRelation(OGSharedPtr<og::og_session_relation> _rel_ptr);
  virtual ~OGSessionRelation();

  ///////////////////////////////
  // property
  ///////////////////////////////
  String^ get_id();
  String^ get_instance_name();
  void set_instance_name(String^ _name);
  String^ get_comment();
  void set_comment(String^ _comment);
  String^ get_revision();
  void set_revision(String^ _rev);
  String^ get_create_date();
  String^ get_update_date();

  String^ get_from_id();
  String^ get_to_id();

  String^ get_schema_relation_id();
  String^ get_schema_relation_type();
  String^ get_schema_relation_name();
  String^ get_schema_relation_comment();
  String^ get_schema_relation_create_date();
  String^ get_schema_relation_update_date();
  // property ends.

  ///////////////////////////////
  // parameters
  ///////////////////////////////
  //template <typename T>
  //void set_parameter_value(String^ _param_name, T _value)
  //{
  //  session_relation_ptr_->set_parameter_value(_param_name, _value);
  //  session_relation_ptr_->sync();
  //}
  //template <typename T>
  //void set_parameter_values(String^ _param_name, List<T>^ _values)
  //{
  //  session_relation_ptr_->set_parameter_values(_param_name, _values);
  //  session_relation_ptr_->sync();
  //}
  //template <typename T>
  //void get_parameter_value(String^ _param_name, T^ _value)
  //{
  //  session_relation_ptr_->get_parameter_value(_param_name, _value);
  //}
  //template <typename T>
  //void get_parameter_values(String^ _param_name, List<T>^ _values)
  //{
  //  session_relation_ptr_->get_parameter_values(_param_name, _values);
  //}

  OGSessionParameter^ get_parameter(String^ _param_name);
  void set_parameter(String^ _param_name,
                     List<Object^>^ _values);

  //wrappers
  void set_parameter_value_integer(String^ _param_name, int _value);
  void set_parameter_value_real(String^ _param_name, double _value);
  void set_parameter_value_text(String^ _param_name, String^ _value);
  void set_parameter_value_bool(String^ _param_name, bool _value);
  void set_parameter_values_integer(String^ _param_name, List<int>^ _values);
  void set_parameter_values_real(String^ _param_name, List<double>^ _values);
  void set_parameter_values_text(String^ _param_name, List<String^>^ _value);
  void set_parameter_values_bool(String^ _param_name, List<bool>^ _value);

  int get_parameter_value_integer(String^ _param_name);
  double get_parameter_value_real(String^ _param_name);
  String^ get_parameter_value_text(String^ _param_name);
  bool get_parameter_value_bool(String^ _param_name);
  List<int>^ get_parameter_values_integer(String^ _param_name);
  List<double>^ get_parameter_values_real(String^ _param_name);
  List<String^>^ get_parameter_values_text(String^ _param_name);
  List<bool>^ get_parameter_values_bool(String^ _param_name);
// object parameters end.


internal:
  OGSharedPtr<og_session_relation> og_session_relation_;

};

}
}
