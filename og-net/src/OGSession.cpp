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

void OGSession::connect(String^ _connection_string)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_connection_string, &str);

  og_session_->session_->open(str);
}


bool OGSession::import_from_file(String^ _path)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_path, &str);

  return og_session_->session_->import_from_file(str);
}
void OGSession::export_to_file(String^ _path)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_path, &str);

  og_session_->session_->export_to_file(str);
}

void OGSession::purge()
{
  og_session_->session_->purge(false);
}
void OGSession::build()
{
  og_session_->session_->build();
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
void OGSession::get_object(List<OGSessionObject^>^ _sesn_obj_list)
{
  std::list<og::og_session_object_ptr> objs;
  og_session_->get_object(&objs);

  OGConverter::convert_std_to_clr
  <std::list<og::og_session_object_ptr>, List<OGSessionObject^>>
      (&objs, _sesn_obj_list);

}
void OGSession::get_object(List<String^>^ _oid_list,
                           List<OGSessionObject^>^ _sesn_obj_list)
{
  std::list<std::string> strl;
  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
      (_oid_list, &strl);

  std::list<og::og_session_object_ptr> objs;
  og_session_->get_object(strl, &objs);

  OGConverter::convert_std_to_clr
  <std::list<og::og_session_object_ptr>, List<OGSessionObject^>>
      (&objs, _sesn_obj_list);
}
void OGSession::get_object_by_type(List<String^>^ _otype_list,
                                   List<OGSessionObject^>^ _sesn_obj_list)
{
  std::list<std::string> strl;
  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
      (_otype_list, &strl);

  std::list<og::og_session_object_ptr> objs;
  og_session_->get_object_by_type(strl, &objs);

  OGConverter::convert_std_to_clr
  <std::list<og::og_session_object_ptr>, List<OGSessionObject^>>
      (&objs, _sesn_obj_list);
}

void OGSession::get_object_by_name(List<String^>^ _oname_list,
                                   List<OGSessionObject^>^ _sesn_obj_list)
{
  std::list<std::string> strl;
  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
      (_oname_list, &strl);

  std::list<og::og_session_object_ptr> objs;
  og_session_->get_object_by_type(strl, &objs);

  OGConverter::convert_std_to_clr
  <std::list<og::og_session_object_ptr>, List<OGSessionObject^>>
      (&objs, _sesn_obj_list);
}

void OGSession::get_object_by_schema_object_name(List<String^>^
    _schm_obj_oname_list,
    List<OGSessionObject^>^ _sesn_obj_list)
{
  std::list<std::string> strl;
  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
      (_schm_obj_oname_list, &strl);

  std::list<og::og_session_object_ptr> objs;
  og_session_->get_object_by_schema_object_name(strl, &objs);

  OGConverter::convert_std_to_clr
  <std::list<og::og_session_object_ptr>, List<OGSessionObject^>>
      (&objs, _sesn_obj_list);
}

void OGSession::get_object_by_schema_object(List<OGSchemaObject^>^
    _schm_obj_list,
    List<OGSessionObject^>^ _sesn_obj_list)
{
}

void OGSession::get_connected_object_from(String^ _to_id,
    List<String^>^ _rel_type_list,
    List<OGSessionObject^>^ _sesn_obj_list)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_to_id, &str);

  std::list<std::string> strl;
  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
      (_rel_type_list, &strl);

  std::list<og::og_session_object_ptr> objs;
  og_session_->get_connected_object_from(str, strl, &objs);

  OGConverter::convert_std_to_clr
  <std::list<og::og_session_object_ptr>, List<OGSessionObject^>>
      (&objs, _sesn_obj_list);
}
void OGSession::get_connected_object_to(String^ _from_id,
                                        List<String^>^ _rel_type_list,
                                        List<OGSessionObject^>^ _sesn_obj_list)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_from_id, &str);

  std::list<std::string> strl;
  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
      (_rel_type_list, &strl);

  std::list<og::og_session_object_ptr> objs;
  og_session_->get_connected_object_to(str, strl, &objs);

  OGConverter::convert_std_to_clr
  <std::list<og::og_session_object_ptr>, List<OGSessionObject^>>
      (&objs, _sesn_obj_list);
}

bool OGSession::validate_connect(String^ _from_id, String^ _to_id,
                                 List<String^>^ _rel_type_list)
{
  std::string str1;
  OGConverter::convert_clr_to_std<String, std::string>
  (_from_id, &str1);

  std::string str2;
  OGConverter::convert_clr_to_std<String, std::string>
  (_to_id, &str2);

  std::list<std::string> strl;
  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
      (_rel_type_list, &strl);

  return og_session_->session_->validate_connect(str1, str2, strl);
}

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

void OGSession::get_relation(List<OGSessionRelation^>^ _sesn_rel_list)
{
  std::list<og::og_session_relation_ptr> rels;
  og_session_->get_relation(&rels);

  OGConverter::convert_std_to_clr
  <std::list<og::og_session_relation_ptr>, List<OGSessionRelation^>>
      (&rels, _sesn_rel_list);
}

void OGSession::get_relation(List<String^>^ _rel_id_list,
                             List<OGSessionRelation^>^ _sesn_rel_list)
{
  std::list<std::string> strl;
  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
      (_rel_id_list, &strl);

  std::list<og::og_session_relation_ptr> rels;
  og_session_->get_relation(strl, &rels);

  OGConverter::convert_std_to_clr
  <std::list<og::og_session_relation_ptr>, List<OGSessionRelation^>>
      (&rels, _sesn_rel_list);
}
void OGSession::get_relation(String^ _from_id, String^ _to_id,
                             List<OGSessionRelation^>^ _sesn_rel_list)
{
  std::string str1;
  OGConverter::convert_clr_to_std<String, std::string>
  (_from_id, &str1);

  std::string str2;
  OGConverter::convert_clr_to_std<String, std::string>
  (_to_id, &str2);

  std::list<og::og_session_relation_ptr> rels;
  og_session_->get_relation(str1, str2, &rels);

  OGConverter::convert_std_to_clr
  <std::list<og::og_session_relation_ptr>, List<OGSessionRelation^>>
      (&rels, _sesn_rel_list);
}
void OGSession::get_relation_by_type(List<String^>^ _rel_type_list,
                                     List<OGSessionRelation^>^ _sesn_rel_list)
{
  std::list<std::string> strl;
  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
      (_rel_type_list, &strl);

  std::list<og::og_session_relation_ptr> rels;
  og_session_->get_relation_by_type(strl, &rels);

  OGConverter::convert_std_to_clr
  <std::list<og::og_session_relation_ptr>, List<OGSessionRelation^>>
      (&rels, _sesn_rel_list);
}
void OGSession::get_relation_by_name(List<String^>^ _rel_name_list,
                                     List<OGSessionRelation^>^ _sesn_rel_list)
{
  std::list<std::string> strl;
  OGConverter::convert_clr_to_std<List<String^>, std::list<std::string>>
      (_rel_name_list, &strl);

  std::list<og::og_session_relation_ptr> rels;
  og_session_->get_relation_by_name(strl, &rels);

  OGConverter::convert_std_to_clr
  <std::list<og::og_session_relation_ptr>, List<OGSessionRelation^>>
      (&rels, _sesn_rel_list);
}

void OGSession::disconnect(String^ _rel_id)
{
  std::string str;
  OGConverter::convert_clr_to_std<String, std::string>(_rel_id, &str);

  og_session_->disconnect(str);
}

} // namespace net
} // namespace og


