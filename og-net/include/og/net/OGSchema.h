#pragma once

//#include "og/net/OGSchemaObject.h"

#include "og/net/OGSharedPtr.h"
//#include "og/core/base.h"
namespace og
{
class og_schema;
namespace net
{
using namespace System;
using namespace System::Collections::Generic;

ref class OGSchemaObject;
ref class OGSchemaRelation;
ref class OGSchemaParameter;
ref class OGSessionObject;
ref class OGSessionRelation;
ref class OGSessionParameter;
ref class OGInteger;
ref class OGReal;
ref class OGText;
ref class OGBool;

public ref class OGSchema
{
public:
  OGSchema(og::og_schema*);
  OGSchema();
  virtual ~OGSchema(void);

  ///////////////////////////////
  // object and its relation
  ///////////////////////////////
  OGSchemaObject^ create_object(String^ _otype, String^ _oname);

  List<OGSchemaObject^>^ get_object(List<String^>^ _oid_list);
  List<OGSchemaObject^>^ get_object();
  void delete_object(String^ _oid);
  /*Nullable*/OGSchemaObject^ get_object(String^ _oid);
  List<OGSchemaObject^>^ get_object_by_type(List<String^>^ _otype_list);
  List<OGSchemaObject^>^ get_object_by_name(List<String^>^ _name_list);

  List<String^>^ get_object_type();

  List<OGSchemaRelation^>^ get_relation_by_type(List<String^>^ _otype_list);

  List<OGSchemaRelation^>^ get_relation(List<String^>^ _relid_list);
  List<OGSchemaRelation^>^ get_relation();
  /*Nullable*/OGSchemaRelation^ get_relation(String^ _relid);
  List<String^>^ get_relation_type();

  List<OGSchemaObject^>^ get_connected_object_to(String^ _from_o_id, List<String^>^ _rel_type_list);

  List<OGSchemaObject^>^ get_connected_object_from(String^ _to_o_id, List<String^>^ _rel_type_list);

  List<OGSchemaRelation^>^ get_connected_relation_to(String^ _from_o_id,
	  List<String^>^ _rel_type_list);
  List<OGSchemaRelation^>^ get_connected_relation_from(String^ _to_o_id,
	  List<String^>^ _rel_type_list);

  bool validate_connect_by_type(String^ _otype_from, String^ _otype_to);
  bool validate_connect_by_type_id(String^ _otype, String^ _oid_from,
                                   String^ _oid_to);
  //object and its relation ends.


  ///////////////////////////////
  // parameters
  ///////////////////////////////
  //template <typename P>
  //OGSchemaParameter^ create_parameter(String^ _param_type,
  //                                    String^ _comment, P^ _basetype)
  //{
  //  og::core::schema_parameter_ptr p =
  //    schema_->create_parameter(_param_type, _comment, _basetype);

  //  return gcnew OGSchemaParameter(/*new og_schema_parameter(p)*/);
  //}

  //template <typename P>
  //OGSchemaParameter^ create_parameter(String^ _param_type,
  //                                    String^ _comment, P^ _basetype, int _default_array_size, int _min_array_size,
  //                                    int _max_array_size)
  //{
  //  og::core::schema_parameter_ptr p =
  //    schema_->create_parameter(_param_type, _comment, _basetype, _default_array_size,
  //                              _min_array_size, _max_array_size);

  //  return gcnew OGSchemaParameter(/*new og_schema_parameter(p)*/);
  //}
  // wrappers
  OGSchemaParameter^ create_parameter_interger(String^ _param_type,
	  String^ _comment, OGInteger^ _basetype);
  OGSchemaParameter^ create_parameter_real(String^ _param_type,
	  String^ _comment, OGReal^ _basetype);
  OGSchemaParameter^ create_parameter_text(String^ _param_type,
	  String^ _comment, OGText^ _basetype);
  OGSchemaParameter^ create_parameter_bool(String^ _param_type,
	  String^ _comment, OGBool^ _basetype);

  OGSchemaParameter^ create_parameter_interger(String^ _param_type,
	  String^ _comment, OGInteger^ _basetype, int _default_array_size, int _min_array_size, int _max_array_size);
  OGSchemaParameter^ create_parameter_real(String^ _param_type,
	  String^ _comment, OGReal^ _basetype, int _default_array_size, int _min_array_size, int _max_array_size);
  OGSchemaParameter^ create_parameter_text(String^ _param_type,
	  String^ _comment, OGText^ _basetype, int _default_array_size, int _min_array_size, int _max_array_size);
  OGSchemaParameter^ create_parameter_bool(String^ _param_type,
	  String^ _comment, OGBool^ _basetype, int _default_array_size, int _min_array_size, int _max_array_size);

  static property String^ schema_property_object_type
  {
	  String^ get();
  }
  static property String^ schema_property_object_name
  {
	  String^ get();
  }
  static property String^ schema_property_core_revision
  {
	  String^ get();
  }

  // parameters ends.

  ///////////////////////////////
  // file io and management
  ///////////////////////////////
  void export_to_file(String^ _path);
  bool import_from_file(String^ _path);
  void purge();

internal:
  og::og_schema* og_schema_;

};

}//namespace net
}//namespace og
