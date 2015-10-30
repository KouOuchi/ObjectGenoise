#include "og/net/stdafx.h"

#include "og/net/OGSchema.h"
#include "og/net/OGConverter.h"
#include "og/net/OGSchemaParameter.h"

#include "og/og_schema.h"

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;

namespace og
{
namespace net
{

String^ OGSchema::schema_property_object_type::get()
{
  return gcnew String(og_schema::schema_property_object_type());
}
String^ OGSchema::schema_property_object_name::get()
{
  return gcnew String(og_schema::schema_property_object_name());
}
String^ OGSchema::schema_property_core_revision::get()
{
  return gcnew String(og_schema::schema_property_core_revision());
}

OGSchema::OGSchema(og::og_schema* _og_schema)
  : og_schema_(_og_schema)
{
  og_schema_ = _og_schema;
}

OGSchema::OGSchema()
{}

OGSchema::~OGSchema()
{
}

void OGSchema::export_to_file(String^ _path)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_path, &str);

  og_schema_->export_to_file(str);
}
bool OGSchema::import_from_file(String^ _path)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_path, &str);

  return og_schema_->import_from_file(str);
}

void OGSchema::purge()
{
  og_schema_->purge();
}

OGSchemaObject^ OGSchema::create_object(String^ _otype, String^ _oname)
{
  std::string str1;
  OGConverter::convert_clr_to_std<String, std::string>(_otype, &str1);

  std::string str2;
  OGConverter::convert_clr_to_std<String, std::string>(_oname, &str2);

  return gcnew OGSchemaObject(OGSharedPtr<og::og_schema_object>
                              (og_schema_->create_object(str1, str2)));
}

void OGSchema::get_object(List<String^>^ _oid_list,
                          List<OGSchemaObject^>^ _schm_obj_list)
{
  std::list<std::string> strl;
  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
      (_oid_list, &strl);

  std::list<og::og_schema_object_ptr> objs;
  og_schema_->get_object(strl, &objs);

  OGConverter::convert_std_to_clr
  <std::list<og::og_schema_object_ptr>, List<OGSchemaObject^>>
      (&objs, _schm_obj_list);
}
void OGSchema::get_object(List<OGSchemaObject^>^ _schm_obj_list)
{
  std::list<og::og_schema_object_ptr> objs;
  og_schema_->get_object(&objs);

  OGConverter::convert_std_to_clr
  <std::list<og::og_schema_object_ptr>, List<OGSchemaObject^>>
      (&objs, _schm_obj_list);
}
void OGSchema::delete_object(String^ _oid)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_oid, &str);

  og_schema_->delete_object(str);
}
/*Nullable*/OGSchemaObject^ OGSchema::get_object(String^ _oid)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_oid, &str);

  boost::optional<og::og_schema_object_ptr> o_ptr(og_schema_->get_object(str));

  if (!o_ptr.is_initialized())
  {
    return nullptr;
  }
  else
  {
    return gcnew OGSchemaObject(OGSharedPtr<og::og_schema_object>(o_ptr.get()));
  }
}
void OGSchema::get_object_by_type(List<String^>^ _otype_list,
                                  List<OGSchemaObject^>^ _schm_obj_list)
{
  std::list<std::string> strl;
  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
      (_otype_list, &strl);

  std::list<og::og_schema_object_ptr> objs;
  og_schema_->get_object_by_type(strl, &objs);

  OGConverter::convert_std_to_clr
  <std::list<og::og_schema_object_ptr>, List<OGSchemaObject^>>
      (&objs, _schm_obj_list);
}
void OGSchema::get_object_by_name(List<String^>^ _name_list,
                                  List<OGSchemaObject^>^ _schm_obj_list)
{
  std::list<std::string> strl;
  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
      (_name_list, &strl);

  std::list<og::og_schema_object_ptr> objs;
  og_schema_->get_object_by_name(strl, &objs);

  OGConverter::convert_std_to_clr
  <std::list<og::og_schema_object_ptr>, List<OGSchemaObject^>>
      (&objs, _schm_obj_list);
}

void OGSchema::get_object_type(List<String^>^ _type_list)
{
  std::list<std::string> strl;
  og_schema_->get_object_type(&strl);

  OGConverter::convert_std_to_clr
  <std::list<std::string>, List<String^>>
                                       (&strl, _type_list);
}

void OGSchema::get_relation(List<String^>^ _relid_list,
                            List<OGSchemaRelation^>^ _schm_rel_list)
{
  std::list<std::string> strl;
  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
      (_relid_list, &strl);

  std::list<og::og_schema_relation_ptr> rels;
  og_schema_->get_relation(strl, &rels);

  OGConverter::convert_std_to_clr
  <std::list<og::og_schema_relation_ptr>, List<OGSchemaRelation^>>
      (&rels, _schm_rel_list);
}

void OGSchema::get_relation(List<OGSchemaRelation^>^ _schm_rel_list)
{
  std::list<og::og_schema_relation_ptr> rels;
  og_schema_->get_relation(&rels);

  OGConverter::convert_std_to_clr
  <std::list<og::og_schema_relation_ptr>, List<OGSchemaRelation^>>
      (&rels, _schm_rel_list);
}
/*Nullable*/OGSchemaRelation^ OGSchema::get_relation(String^ _relid)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_relid, &str);

  boost::optional<og::og_schema_relation_ptr> rel_ptr(og_schema_->get_relation(
        str));

  if (!rel_ptr.is_initialized())
  {
    return nullptr;
  }
  else
  {
    return gcnew OGSchemaRelation(OGSharedPtr<og::og_schema_relation>
                                  (rel_ptr.get()));
  }
}
void OGSchema::get_relation_type(List<String^>^ _rel_type_list)
{
  std::list<std::string> strl;
  og_schema_->get_relation_type(&strl);

  OGConverter::convert_std_to_clr
  <std::list<std::string>, List<String^>>
                                       (&strl, _rel_type_list);
}

void OGSchema::get_relation_by_type(List<String^>^ _rel_type_list,
                                    List<OGSchemaRelation^>^ _schm_rel_list)
{
  std::list<std::string> strl;
  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
      (_rel_type_list, &strl);

  std::list<og::og_schema_relation_ptr> rels;
  og_schema_->get_relation_by_type(strl, &rels);

  OGConverter::convert_std_to_clr
  <std::list<og::og_schema_relation_ptr>, List<OGSchemaRelation^>>
      (&rels, _schm_rel_list);
}

void OGSchema::get_connected_object_to(String^ _from_o_id,
                                       List<String^>^ _rel_type_list,
                                       List<OGSchemaObject^>^ _schm_obj_list)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_from_o_id, &str);

  std::list<std::string> strl;
  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
      (_rel_type_list, &strl);

  std::list<og::og_schema_object_ptr> objs;
  og_schema_->get_connected_object_to(str, strl, &objs);

  OGConverter::convert_std_to_clr
  <std::list<og::og_schema_object_ptr>, List<OGSchemaObject^>>
      (&objs, _schm_obj_list);
}
void OGSchema::get_connected_object_from(String^ _to_o_id,
    List<String^>^ _rel_type_list,
    List<OGSchemaObject^>^ _schm_obj_list)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_to_o_id, &str);

  std::list<std::string> strl;
  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
      (_rel_type_list, &strl);

  std::list<og::og_schema_object_ptr> objs;
  og_schema_->get_connected_object_from(str, strl, &objs);

  OGConverter::convert_std_to_clr
  <std::list<og::og_schema_object_ptr>, List<OGSchemaObject^>>
      (&objs, _schm_obj_list);
}

void OGSchema::get_connected_relation_to(String^ _from_o_id,
    List<String^>^ _rel_type_list,
    List<OGSchemaRelation^>^ _schm_rel_list)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_from_o_id, &str);

  std::list<std::string> strl;
  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
      (_rel_type_list, &strl);

  std::list<og::og_schema_relation_ptr> rels;
  og_schema_->get_connected_relation_to(str, strl, &rels);

  OGConverter::convert_std_to_clr
  <std::list<og::og_schema_relation_ptr>, List<OGSchemaRelation^>>
      (&rels, _schm_rel_list);
}
void OGSchema::get_connected_relation_from(String^ _to_o_id,
    List<String^>^ _rel_type_list,
    List<OGSchemaRelation^>^ _schm_rel_list)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_to_o_id, &str);

  std::list<std::string> strl;
  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
      (_rel_type_list, &strl);

  std::list<og::og_schema_relation_ptr> rels;
  og_schema_->get_connected_relation_from(str, strl, &rels);

  OGConverter::convert_std_to_clr
  <std::list<og::og_schema_relation_ptr>, List<OGSchemaRelation^>>
      (&rels, _schm_rel_list);
}

bool OGSchema::validate_connect_by_type(String^ _otype_from, String^ _otype_to)
{
  std::string str1;
  OGConverter::convert_clr_to_std<String, std::string>(_otype_from, &str1);
  std::string str2;
  OGConverter::convert_clr_to_std<String, std::string>(_otype_to, &str2);

  return og_schema_->validate_connect_by_type(str1, str2);
}
bool OGSchema::validate_connect_by_type_id(String^ _otype, String^ _oid_from,
    String^ _oid_to)
{
  std::string str1;
  OGConverter::convert_clr_to_std<String, std::string>(_otype, &str1);
  std::string str2;
  OGConverter::convert_clr_to_std<String, std::string>(_oid_from, &str2);
  std::string str3;
  OGConverter::convert_clr_to_std<String, std::string>(_oid_to, &str3);

  return og_schema_->validate_connect_by_type_id(str1, str2, str3);
}


OGSchemaParameter^ OGSchema::create_parameter_interger(String^ _param_type,
    String^ _comment, OGInteger^ _basetype)
{
  return create_parameter_interger(_param_type, _comment, _basetype, 1, 1, 1);
}
OGSchemaParameter^ OGSchema::create_parameter_real(String^ _param_type,
    String^ _comment, OGReal^ _basetype)
{
  return create_parameter_real(_param_type, _comment, _basetype, 1, 1, 1);
}
OGSchemaParameter^ OGSchema::create_parameter_text(String^ _param_type,
    String^ _comment, OGText^ _basetype)
{
  return create_parameter_text(_param_type, _comment, _basetype, 1, 1, 1);
}
OGSchemaParameter^ OGSchema::create_parameter_bool(String^ _param_type,
    String^ _comment, OGBool^ _basetype)
{
  return create_parameter_bool(_param_type, _comment, _basetype, 1, 1, 1);
}

OGSchemaParameter^ OGSchema::create_parameter_interger(String^ _param_type,
    String^ _comment, OGInteger^ _basetype, int _default_array_size,
    int _min_array_size, int _max_array_size)
{
  std::string str1;
  OGConverter::convert_clr_to_std<String, std::string>(_param_type, &str1);

  std::string str2;
  OGConverter::convert_clr_to_std<String, std::string>(_comment, &str2);

  og::og_schema_parameter_ptr p(
    og_schema_->create_parameter<typename og::core::parameter_basetype_integer>
    (str1, str2, _basetype->og_basetype_integer_.get(), _default_array_size,
     _min_array_size, _max_array_size));

  return gcnew OGSchemaParameter(
           OGSharedPtr<og::og_schema_parameter>(p));
}

OGSchemaParameter^ OGSchema::create_parameter_real(String^ _param_type,
    String^ _comment, OGReal^ _basetype, int _default_array_size,
    int _min_array_size, int _max_array_size)
{
  std::string str1;
  OGConverter::convert_clr_to_std<String, std::string>(_param_type, &str1);

  std::string str2;
  OGConverter::convert_clr_to_std<String, std::string>(_comment, &str2);

  og::og_schema_parameter_ptr p(
    og_schema_->create_parameter<typename og::core::parameter_basetype_real>
    (str1, str2, _basetype->og_basetype_real_.get(), _default_array_size,
     _min_array_size, _max_array_size ));

  return gcnew OGSchemaParameter(
           OGSharedPtr<og::og_schema_parameter>(p));
}
OGSchemaParameter^ OGSchema::create_parameter_text(String^ _param_type,
    String^ _comment, OGText^ _basetype, int _default_array_size,
    int _min_array_size, int _max_array_size)
{
  std::string str1;
  OGConverter::convert_clr_to_std<String, std::string>(_param_type, &str1);

  std::string str2;
  OGConverter::convert_clr_to_std<String, std::string>(_comment, &str2);

  og::og_schema_parameter_ptr p(
    og_schema_->create_parameter<typename og::core::parameter_basetype_text>
    (str1, str2, _basetype->og_basetype_text_.get(), _default_array_size,
     _min_array_size, _max_array_size ));

  return gcnew OGSchemaParameter(
           OGSharedPtr<og::og_schema_parameter>(p));
}

OGSchemaParameter^ OGSchema::create_parameter_bool(String^ _param_type,
    String^ _comment, OGBool^ _basetype, int _default_array_size,
    int _min_array_size, int _max_array_size)
{
  std::string str1;
  OGConverter::convert_clr_to_std<String, std::string>(_param_type, &str1);

  std::string str2;
  OGConverter::convert_clr_to_std<String, std::string>(_comment, &str2);

  og::og_schema_parameter_ptr p(
    og_schema_->create_parameter<typename og::core::parameter_basetype_integer>
    (str1, str2, _basetype->og_basetype_integer_.get(), _default_array_size,
     _min_array_size, _max_array_size));

  return gcnew OGSchemaParameter(
           OGSharedPtr<og::og_schema_parameter>(p));
}

}
}

