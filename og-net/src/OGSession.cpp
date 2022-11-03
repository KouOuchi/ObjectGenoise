#include "og/net/stdafx.h"
#include "og/net/OGSession.h"

#include "og/net/OGConverter.h"
#include "og/og_session.h"

namespace og
{
namespace net
{


OGSession::OGSession() : og_session_(new og::og_session()),
  schema_(gcnew OGSchema(og_session_->schema()))
{
}

OGSession::~OGSession()
{
}

OGSchema^ OGSession::schema()
{
  return schema_;
}

void OGSession::open(String^ _connection_string)
{
  Encoding^ u8 = Encoding::UTF8;
  cli::array<unsigned char>^ bytes = u8->GetBytes(_connection_string);
  pin_ptr<unsigned char> pinnedPtr = &bytes[0];
  og_session_->open(std::string((char*)pinnedPtr));
}

void OGSession::close()
{
  og_session_->close();
}

bool OGSession::import_from_file(String^ _path)
{
  Encoding^ u8 = Encoding::UTF8;
  cli::array<unsigned char>^ bytes = u8->GetBytes(_path);
  pin_ptr<unsigned char> pinnedPtr = &bytes[0];

  return og_session_->import_from_file(std::string((char*)pinnedPtr));
}
void OGSession::export_to_file(String^ _path)
{
  Encoding^ u8 = Encoding::UTF8;
  cli::array<unsigned char>^ bytes = u8->GetBytes(_path);
  pin_ptr<unsigned char> pinnedPtr = &bytes[0];

  og_session_->export_to_file(std::string((char*)pinnedPtr));
}

void OGSession::purge()
{
  og_session_->purge();
}
void OGSession::build()
{
  og_session_->build();
}

OGSessionObject^ OGSession::create_object(OGSchemaObject^ _schm_obj)
{
  return gcnew OGSessionObject(
           OGSharedPtr<og::og_session_object>(
             og_session_->create_object(_schm_obj->og_schema_object_)));
}

//void OGSession::delete_object(String^ _id)
//{
//  std::string str;
//  OGConverter::convert_clr_to_std<String, std::string>(_id, &str);
//
//  og_session_->session_->delete_object(str);
//}

/*Nullable*/OGSessionObject^ OGSession::get_object(String^ _id)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_id, &str);

  boost::optional<og::og_session_object_ptr> o = og_session_->get_object(str);

  if (!o.is_initialized())
  {
    return nullptr;
  }
  else
  {
    return gcnew OGSessionObject(
             OGSharedPtr<og::og_session_object>(o.get()));
  }
}
List<OGSessionObject^>^ OGSession::get_object()
{
  std::list<og::og_session_object_ptr> objs;
  og_session_->get_object(&objs);

  List<OGSessionObject^>^ sesn_obj_list = gcnew List<OGSessionObject^>();
  OGConverter::convert_std_to_clr
  <std::list<og::og_session_object_ptr>, List<OGSessionObject^>>
      (&objs, sesn_obj_list);

  return  sesn_obj_list;
}
List<OGSessionObject^>^ OGSession::get_object(List<String^>^ _oid_list)
{
  std::list<std::string> strl;
  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
      (_oid_list, &strl);

  std::list<og::og_session_object_ptr> objs;
  og_session_->get_object(strl, &objs);

  List<OGSessionObject^>^ sesn_obj_list = gcnew List<OGSessionObject^>();
  OGConverter::convert_std_to_clr
  <std::list<og::og_session_object_ptr>, List<OGSessionObject^>>
      (&objs, sesn_obj_list);

  return  sesn_obj_list;
}
List<OGSessionObject^>^ OGSession::get_object_by_type(List<String^>^
    _otype_list)
{
  std::list<std::string> strl;
  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
      (_otype_list, &strl);

  std::list<og::og_session_object_ptr> objs;
  og_session_->get_object_by_type(strl, &objs);

  List<OGSessionObject^>^ sesn_obj_list = gcnew List<OGSessionObject^>();
  OGConverter::convert_std_to_clr
  <std::list<og::og_session_object_ptr>, List<OGSessionObject^>>
      (&objs, sesn_obj_list);

  return sesn_obj_list;
}

List<OGSessionObject^>^ OGSession::get_object_by_name(List<String^>^
    _oname_list)
{
  std::list<std::string> strl;
  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
      (_oname_list, &strl);

  std::list<og::og_session_object_ptr> objs;
  og_session_->get_object_by_type(strl, &objs);

  List<OGSessionObject^>^ sesn_obj_list = gcnew List<OGSessionObject^>();
  OGConverter::convert_std_to_clr
  <std::list<og::og_session_object_ptr>, List<OGSessionObject^>>
      (&objs, sesn_obj_list);

  return sesn_obj_list;
}

List<OGSessionObject^>^ OGSession::get_object_by_schema_object_name(
  List<String^>^
  _schm_obj_oname_list)
{
  std::list<std::string> strl;
  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
      (_schm_obj_oname_list, &strl);

  std::list<og::og_session_object_ptr> objs;
  og_session_->get_object_by_schema_object_name(strl, &objs);

  List<OGSessionObject^>^ sesn_obj_list = gcnew List<OGSessionObject^>();

  OGConverter::convert_std_to_clr
  <std::list<og::og_session_object_ptr>, List<OGSessionObject^>>
      (&objs, sesn_obj_list);

  return sesn_obj_list;
}

//List<OGSessionObject^>^ OGSession::get_object_by_schema_object(List<OGSchemaObject^>^
//    _schm_obj_list)
//{
//}

List<OGSessionObject^>^ OGSession::get_connected_object_from(String^ _to_id,
    List<String^>^ _rel_type_list
                                                            )
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_to_id, &str);

  std::list<std::string> strl;
  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
      (_rel_type_list, &strl);

  std::list<og::og_session_object_ptr> objs;
  og_session_->get_connected_object_from(str, strl, &objs);

  List<OGSessionObject^>^ sesn_obj_list = gcnew List<OGSessionObject^>();
  OGConverter::convert_std_to_clr
  <std::list<og::og_session_object_ptr>, List<OGSessionObject^>>
      (&objs, sesn_obj_list);

  return sesn_obj_list;
}
List<OGSessionObject^>^ OGSession::get_connected_object_to(String^ _from_id,
    List<String^>^ _rel_type_list)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_from_id, &str);

  std::list<std::string> strl;
  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
      (_rel_type_list, &strl);

  std::list<og::og_session_object_ptr> objs;
  og_session_->get_connected_object_to(str, strl, &objs);

  List<OGSessionObject^>^ sesn_obj_list = gcnew List<OGSessionObject^>();
  OGConverter::convert_std_to_clr
  <std::list<og::og_session_object_ptr>, List<OGSessionObject^>>
      (&objs, sesn_obj_list);

  return sesn_obj_list;
}

//bool OGSession::validate_connect(String^ _from_id, String^ _to_id,
//                                 List<String^>^ _rel_type_list)
//{
//  std::string str1;
//  OGConverter::convert_clr_to_std<String, std::string>
//  (_from_id, &str1);
//
//  std::string str2;
//  OGConverter::convert_clr_to_std<String, std::string>
//  (_to_id, &str2);
//
//  std::list<std::string> strl;
//  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
//      (_rel_type_list, &strl);
//
//  return og_session_->session_->validate_connect(str1, str2, strl);
//}

/*Nullable*/OGSessionRelation^ OGSession::get_relation(String^ _rel_id)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_rel_id, &str);

  boost::optional<og::og_session_relation_ptr>o =
    og_session_->get_relation(str);

  if (!o.is_initialized())
  {
    return nullptr;
  }
  else
  {
    return gcnew OGSessionRelation(OGSharedPtr<og::og_session_relation>(o.get()));
  }
}

List<OGSessionRelation^>^ OGSession::get_relation()
{
  std::list<og::og_session_relation_ptr> rels;
  og_session_->get_relation(&rels);

  List<OGSessionRelation^>^ sesn_rel_list = gcnew List<OGSessionRelation^>();
  OGConverter::convert_std_to_clr
  <std::list<og::og_session_relation_ptr>, List<OGSessionRelation^>>
      (&rels, sesn_rel_list);

  return sesn_rel_list;
}

List<OGSessionRelation^>^ OGSession::get_relation(List<String^>^ _rel_id_list)
{
  std::list<std::string> strl;
  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
      (_rel_id_list, &strl);

  std::list<og::og_session_relation_ptr> rels;
  og_session_->get_relation(strl, &rels);

  List<OGSessionRelation^>^ sesn_rel_list = gcnew List<OGSessionRelation^>();
  OGConverter::convert_std_to_clr
  <std::list<og::og_session_relation_ptr>, List<OGSessionRelation^>>
      (&rels, sesn_rel_list);

  return sesn_rel_list;
}
List<OGSessionRelation^>^ OGSession::get_relation(String^ _from_id,
    String^ _to_id)
{
  std::string str1;
  OGConverter::convert_clr_to_std<String, std::string>
  (_from_id, &str1);

  std::string str2;
  OGConverter::convert_clr_to_std<String, std::string>
  (_to_id, &str2);

  std::list<og::og_session_relation_ptr> rels;
  og_session_->get_relation(str1, str2, &rels);

  List<OGSessionRelation^>^ sesn_rel_list = gcnew List<OGSessionRelation^>();
  OGConverter::convert_std_to_clr
  <std::list<og::og_session_relation_ptr>, List<OGSessionRelation^>>
      (&rels, sesn_rel_list);

  return sesn_rel_list;
}
List<OGSessionRelation^>^ OGSession::get_relation_by_type(
  List<String^>^ _rel_type_list)
{
  std::list<std::string> strl;
  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
      (_rel_type_list, &strl);

  std::list<og::og_session_relation_ptr> rels;
  og_session_->get_relation_by_type(strl, &rels);

  List<OGSessionRelation^>^ sesn_rel_list = gcnew List<OGSessionRelation^>();
  OGConverter::convert_std_to_clr
  <std::list<og::og_session_relation_ptr>, List<OGSessionRelation^>>
      (&rels, sesn_rel_list);

  return sesn_rel_list;
}

List<OGSessionRelation^>^ OGSession::get_relation_by_name(
  List<String^>^ _rel_name_list)
{
  std::list<std::string> strl;
  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
      (_rel_name_list, &strl);

  std::list<og::og_session_relation_ptr> rels;
  og_session_->get_relation_by_name(strl, &rels);

  List<OGSessionRelation^>^ sesn_rel_list = gcnew List<OGSessionRelation^>();
  OGConverter::convert_std_to_clr
  <std::list<og::og_session_relation_ptr>, List<OGSessionRelation^>>
      (&rels, sesn_rel_list);

  return sesn_rel_list;
}

void OGSession::disconnect(String^ _rel_id)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_rel_id, &str);

  og_session_->disconnect(str);
}

bool OGSession::catchup_schema(String^ _path)
{
  Encoding^ u8 = Encoding::UTF8;
  cli::array<unsigned char>^ bytes_path = u8->GetBytes(_path);
  pin_ptr<unsigned char> file_path = &bytes_path[0];

  String^ tempdir = System::IO::Path::GetTempPath();
  cli::array<unsigned char>^ bytes_temp_dir = u8->GetBytes(tempdir);
  pin_ptr<unsigned char> tempdir_str = &bytes_temp_dir[0];

  return og_session_->catchup_schema((const char*)file_path, (const char*)tempdir_str);
}

OGSessionObject^ OGSession::get_property_object()
{
  og::og_session_object_ptr sesn_obj = og_session_->get_property_object();

  if (sesn_obj == nullptr)
  {
    return nullptr;
  }
  else
  {
    return gcnew OGSessionObject(OGSharedPtr<og::og_session_object>(sesn_obj));
  }
}

OGSessionObject^ OGSession::import_object_from_file(String^ _path)
{
  Encoding^ u8 = Encoding::UTF8;
  cli::array<unsigned char>^ bytes = u8->GetBytes(_path);
  pin_ptr<unsigned char> pinnedPtr = &bytes[0];

  boost::optional<og::og_session_object_ptr> ret =
    og_session_->import_object_from_file(std::string((char*)pinnedPtr));
  if (ret.is_initialized())
  {
    return gcnew OGSessionObject(OGSharedPtr<og::og_session_object>(ret.get()));
  }
  else
  {
    return nullptr;
  }
}


} // namespace net
} // namespace og


