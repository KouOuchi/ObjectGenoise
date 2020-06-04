#pragma once

#include "og/net/OGSchema.h"
#include "og/net/OGSharedPtr.h"

namespace og
{

class og_session;

namespace net
{

using namespace System;
using namespace System::Collections::Generic;

public ref class OGSession
{
public:
  OGSession();
  virtual ~OGSession();

  ///////////////////////////////
  // schema
  ///////////////////////////////
  OGSchema^ schema();
  void open(String^ _connection_string);
  void close();

  ///////////////////////////////
  // file io and management
  ///////////////////////////////
  bool import_from_file(String^ _path);
  void export_to_file(String^ _path);
  void purge();
  void build();
  /*Nullable*/OGSessionObject^ import_object_from_file(String^ _path);

  // file io and management ends.

  ///////////////////////////////
  // object
  ///////////////////////////////
  OGSessionObject^ create_object(OGSchemaObject^ _schm_obj);
  //void delete_object(String^ _id);

  /*Nullable*/OGSessionObject^ get_object(String^ _id);
  List<OGSessionObject^>^ get_object();
  List<OGSessionObject^>^ get_object(List<String^>^ _oid_list);

  List<OGSessionObject^>^ get_object_by_type(List<String^>^ _otype_list);

  List<OGSessionObject^>^ get_object_by_name(List<String^>^ _oname_list);

  List<OGSessionObject^>^ get_object_by_schema_object_name(
    List<String^>^ _schm_obj_oname_list);

//  List<OGSessionObject^>^ get_object_by_schema_object(List<OGSchemaObject^>^ _schm_obj_list);

  ///////////////////////////////
  // relation
  ///////////////////////////////
  List<OGSessionObject^>^ get_connected_object_from(String^ _to_id,
      List<String^>^ _rel_type_list);
  List<OGSessionObject^>^ get_connected_object_to(String^ _from_id,
      List<String^>^ _rel_type_list);

  //bool validate_connect(String^ _from_id, String^ _to_id,
  //                      List<String^>^ _rel_type_list);

  /*Nullable*/OGSessionRelation^ get_relation(String^ _rel_id);
  List<OGSessionRelation^>^ get_relation();
  List<OGSessionRelation^>^ get_relation(List<String^>^ _rel_id_list);
  List<OGSessionRelation^>^ get_relation(String^ _from_id, String^ _to_id);
  List<OGSessionRelation^>^ get_relation_by_type(List<String^>^ _rel_type_list);
  List<OGSessionRelation^>^ get_relation_by_name(List<String^>^ _rel_name_list);

  void disconnect(String^ _rel_id);
  // relation ends.

  bool catchup_schema(String^ _path);

  OGSessionObject^ get_property_object();

internal:
  OGSharedPtr<og::og_session> og_session_;

private:
  OGSchema^ schema_;
};

}//namespace net
}//namespace og
