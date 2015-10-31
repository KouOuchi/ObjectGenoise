#pragma once

#include "og/net/OGSchemaParameter.h"
#include "og/net/OGSchemaRelation.h"
#include "og/net/OGSharedPtr.h"

#include "og/core/og.h"

namespace og
{
class og_schema_object;

namespace net
{
using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;

//ref class OGSession;
//ref class OGSchemaRelation;

public ref class OGSchemaObject
{
public:
  OGSchemaObject(OGSharedPtr<og::og_schema_object> _o_ptr);
  virtual ~OGSchemaObject(void);

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
  void revision_up();
  // property ends.

  ///////////////////////////////
  // relation
  ///////////////////////////////
  OGSchemaRelation^ connect_from(OGSchemaObject^ _from, String^ _rel_type);
  OGSchemaRelation^ connect_to(OGSchemaObject^ _to, String^ _rel_type);
  void disconnect();
  void disconnect(OGSchemaObject^ _object);

  ///////////////////////////////
  // object
  ///////////////////////////////
//  OGSchemaObject^ copy_object(void);
  void delete_object();

  List<OGSchemaObject^>^ get_connected_object();
  List<OGSchemaObject^>^ get_connected_object(List<String^>^ _rel_type_List);

  List<OGSchemaObject^>^ get_connected_object_to();
  List<OGSchemaObject^>^ get_connected_object_to(List<String^>^ _rel_type_List);

  List<OGSchemaObject^>^ get_connected_object_from();
  List<OGSchemaObject^>^ get_connected_object_from(List<String^>^ _rel_type_List);

  ///////////////////////////////
  // parameters
  ///////////////////////////////
  void get_parameters(
    List<Tuple<String^, OGSchemaParameter^>^>^
    _param_name_types);

  og::core::parameter_basetype_enum  get_parameter_basetype_enum_by_param_name(
    String^ _param_name);

  void add_parameter_definition(String^ _param_name,
                                OGSchemaParameter^ _schm_par);

  template <typename P>
  void get_parameter_definition(String^ _param_name, P^ _param_basetype) {}

  void delete_parameter_definition(String^ _param_name, OGSchemaParameter^ _schm_par);
  
internal:
  OGSharedPtr<og::og_schema_object> og_schema_object_;

};

}
}