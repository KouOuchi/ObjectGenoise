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
  void connect(String^ _connection_string);

  ///////////////////////////////
  // file io and management
  ///////////////////////////////
  bool import_from_file(String^ _path);
  void export_to_file(String^ _path);
  void purge();
  void build();

  // file io and management ends.

  ///////////////////////////////
  // object
  ///////////////////////////////
  OGSessionObject^ create_object(OGSchemaObject^ _schm_obj);
  void delete_object(String^ _id);

  /*Nullable*/OGSessionObject^ get_object(String^ _id);
  void get_object(List<OGSessionObject^>^ _sesn_obj_list);
  void get_object(List<String^>^ _oid_list,
                  List<OGSessionObject^>^ _sesn_obj_list);

  void get_object_by_type(List<String^>^ _otype_list,
                          List<OGSessionObject^>^ _sesn_obj_list);

  void get_object_by_name(List<String^>^ _oname_list,
                          List<OGSessionObject^>^ _sesn_obj_list);

  void get_object_by_schema_object_name(List<String^>^ _schm_obj_oname_list,
                                        List<OGSessionObject^>^ _sesn_obj_list);

  void get_object_by_schema_object(List<OGSchemaObject^>^ _schm_obj_list,
                                   List<OGSessionObject^>^ _sesn_obj_list);

  ///////////////////////////////
  // relation
  ///////////////////////////////
  void get_connected_object_from(String^ _to_id, List<String^>^ _rel_type_list,
                                 List<OGSessionObject^>^ _sesn_obj_list);
  void get_connected_object_to(String^ _from_id, List<String^>^ _rel_type_list,
                               List<OGSessionObject^>^ _sesn_obj_list);
  bool validate_connect(String^ _from_id, String^ _to_id,
                        List<String^>^ _rel_type_list);

  /*Nullable*/OGSessionRelation^ get_relation(String^ _rel_id);
  void get_relation(List<OGSessionRelation^>^ _sesn_rel_list);
  void get_relation(List<String^>^ _rel_id_list,
                    List<OGSessionRelation^>^ _sesn_rel_list);
  void get_relation(String^ _from_id, String^ _to_id,
                    List<OGSessionRelation^>^ _sesn_rel_list);
  void get_relation_by_type(List<String^>^ _rel_type_list,
                            List<OGSessionRelation^>^ _sesn_rel_list);
  void get_relation_by_name(List<String^>^ _rel_name_list,
                            List<OGSessionRelation^>^ _sesn_rel_list);

  void disconnect(String^ _rel_id);
  // relation ends.

internal:
  OGSharedPtr<og::og_session> og_session_;

private:
  OGSchema^ schema_;
};

}//namespace net
}//namespace og
