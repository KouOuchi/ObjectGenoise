#pragma once

#include "og/net/OGSchema.h"
#include "og/net/OGSession.h"
#include "og/net/OGSharedPtr.h"

#include "og/og_session_object.h"

namespace og
{
class og_session_object;

namespace net
{

public enum class OGConnectionDirection
{
  Direction_None = 0,
  Direction_To = 1,
  Direction_From = 2,
  Direction_Both = 4
};

[Flags]
public enum class OGConnectionValidationResult
{
  Valid = 0,
  Invalid = 1,
  Not_found_relation = 2,
  Multiplicity_violation = 4
};

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;

public ref class OGSessionObject
{
public:
  OGSessionObject(OGSharedPtr<og::og_session_object> _o_ptr);
  virtual ~OGSessionObject();

  ///////////////////////////////
  // property
  ///////////////////////////////
  String^ get_id();
  String^ get_instance_name();
  void set_instance_name(String^ _name);
  String^ get_comment();
  void set_comment(String^ _comment);
  String^ get_revision();
  String^ get_create_date();
  String^ get_update_date();

  String^ get_schema_object_id();
  String^ get_schema_object_type();
  String^ get_schema_object_name();
  String^ get_schema_object_comment();
  String^ get_schema_object_create_date();
  String^ get_schema_object_update_date();
  // property ends.

  ///////////////////////////////
  // relation
  ///////////////////////////////
  OGSessionRelation^ connect_to(OGSessionObject^ _sesn_obj,
                                String^ _rel_type);
  OGSessionRelation^ connect_from(OGSessionObject^ _sesn_obj,
                                  String^ _rel_type);
  void disconnect(OGSessionObject^ _sesn_obj);
  void delete_object();
  void delete_object(OGConnectionDirection _dir);
  OGSessionObject^ copy_object();
  OGSessionObject^ copy_object(OGConnectionDirection _dir);

  List<OGSessionObject^>^ get_connected_object();
  List<OGSessionObject^>^ get_connected_object(List<String^>^ _rel_type_List);
  List<OGSessionObject^>^ get_connected_object_from();
  List<OGSessionObject^>^ get_connected_object_from(List<String^>^ _rel_type_List);
  List<OGSessionObject^>^ get_connected_object_to();
  List<OGSessionObject^>^ get_connected_object_to(List<String^>^ _rel_type_List);
  //bool validate_connect_to(OGSessionObject^ _sesn_obj_ptr);
  OGConnectionValidationResult validate_connect_to(OGSessionObject^ _sesn_obj_ptr, String^ _rel_type);
  //bool validate_connect_from(OGSessionObject^ _sesn_obj_ptr);
  OGConnectionValidationResult validate_connect_from(OGSessionObject^ _sesn_obj_ptr,
                             String^ _rel_type);

  List<String^>^ get_connectable_relation_type_to();
  List<String^>^ get_connectable_relation_type_from();
  Dictionary<String^, List<OGSchemaObject^>^>^ get_connectable_to();
  Dictionary<String^, List<OGSchemaObject^>^>^ get_connectable_from();

  ///////////////////////////////
  // parameters
  ///////////////////////////////
  //template <typename T>
  //void set_parameter_value(String^ _param_name, T _value)
  //{
  //  session_object_ptr_->set_parameter_value(_param_name, _value);
  //  session_object_ptr_->sync();
  //}
  //template <typename T>
  //void set_parameter_values(String^ _param_name, List<T>^ _values)
  //{
  //  session_object_ptr_->set_parameter_values(_param_name, _values);
  //  session_object_ptr_->sync();
  //}
  //template <typename T>
  //void get_parameter_value(String^ _param_name, T^ _value)
  //{
  //  session_object_ptr_->get_parameter_value(_param_name, _value);
  //}
  //template <typename T>
  //void get_parameter_values(String^ _param_name, List<T>^ _values)
  //{
  //  session_object_ptr_->get_parameter_values(_param_name, _values);
  //}

  /*Nullable*/OGSessionParameter^ get_parameter(String^ _param_name);
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
  //OGSessionObject();
  OGSharedPtr<og::og_session_object> og_session_object_;

};

}
}