#include "og/og_schema.h"
#include "og/og_schema_object.h"
#include "og/core/schema.h"
#include "og/og_converter.h"

namespace og
{

og_schema::og_schema(og::core::schema* _schema) :
  schema_(_schema)
{
}

og_schema::~og_schema()
{}

void og_schema::purge()
{
  schema_->purge();
}

og_schema_object_ptr og_schema::create_object(string _otype, string _oname)
{
  og::core::schema_object_ptr o = schema_->create_object(_otype, _oname);

  return og_schema_object_ptr(new og_schema_object(o));
}

void og_schema::export_to_file(string _path)
{
  schema_->export_to_file(_path);
}
bool og_schema::import_from_file(string _path)
{
  return schema_->import_from_file(_path);
}

void og_schema::get_object(list<string>& _oid_list,
                           list<og_schema_object_ptr>* _schm_obj_list)
{
  list<og::core::schema_object_ptr> objs;
  schema_->get_object(_oid_list, &objs);

  og_converter::convert
  <list<og::core::schema_object_ptr>&, list<og_schema_object_ptr>*>
  (objs, _schm_obj_list);
}
void og_schema::get_object(list<og_schema_object_ptr>* _schm_obj_list)
{
  list<og::core::schema_object_ptr> objs;
  schema_->get_object(&objs);

  og_converter::convert
  <list<og::core::schema_object_ptr>&, list<og_schema_object_ptr>*>
  (objs, _schm_obj_list);
}
boost::optional<og_schema_object_ptr> og_schema::get_object(string _oid)
{
  boost::optional<og::core::schema_object_ptr> obj = schema_->get_object(_oid);

  if (!obj.is_initialized())
  {
    return boost::optional<og_schema_object_ptr>();
  }
  else
  {
    return boost::optional<og_schema_object_ptr>(og_schema_object_ptr(
             new og_schema_object(obj.get())));
  }
}
void og_schema::get_object_by_type(list<string>& _otype_list,
                                   list<og_schema_object_ptr>* _schm_obj_list)
{
  list<og::core::schema_object_ptr> objs;
  schema_->get_object_by_type(_otype_list, &objs);

  og_converter::convert
  <list<og::core::schema_object_ptr>&, list<og_schema_object_ptr>*>
  (objs, _schm_obj_list);
}
void og_schema::get_object_by_name(list<string>& _name_list,
                                   list<og_schema_object_ptr>* _schm_obj_list)
{
  list<og::core::schema_object_ptr> objs;
  schema_->get_object_by_name(_name_list, &objs);

  og_converter::convert
  <list<og::core::schema_object_ptr>&, list<og_schema_object_ptr>*>
  (objs, _schm_obj_list);
}
void og_schema::get_object_type(list<string>* _type_list)
{
  schema_->get_object_type(_type_list);
}

void og_schema::get_relation_by_type(list<string>& _otype_list,
                                     list<og_schema_relation_ptr>* _schm_rel_list)
{
  list<og::core::schema_relation_ptr> rels;
  schema_->get_relation_by_type(_otype_list, &rels);

  og_converter::convert
  <list<og::core::schema_relation_ptr>&, list<og_schema_relation_ptr>*>
  (rels, _schm_rel_list);
}

void og_schema::get_relation(list<string>& _relid_list,
                             list<og_schema_relation_ptr>* _schm_rel_list)
{
  list<og::core::schema_relation_ptr> rels;
  schema_->get_relation(_relid_list, &rels);

  og_converter::convert
  <list<og::core::schema_relation_ptr>&, list<og_schema_relation_ptr>*>
  (rels, _schm_rel_list);
}

void og_schema::get_relation(list<og_schema_relation_ptr>* _schm_rel_list)
{
  list<og::core::schema_relation_ptr> rels;
  schema_->get_relation(&rels);

  og_converter::convert
  <list<og::core::schema_relation_ptr>&, list<og_schema_relation_ptr>*>
  (rels, _schm_rel_list);
}

optional<og_schema_relation_ptr> og_schema::get_relation(string _relid)
{
  optional<og::core::schema_relation_ptr> r =
    schema_->get_relation(_relid);

  if (!r.is_initialized())
  {
    return optional<og_schema_relation_ptr>();
  }
  else
  {
    return optional<og_schema_relation_ptr>(
             og_schema_relation_ptr(
               new og_schema_relation(r.get())
             ));
  }
}

void og_schema::get_relation_type(list<string>* _rel_type_list)
{
  schema_->get_relation_type(_rel_type_list);
}

void og_schema::get_connected_object_to(string _from_o_id,
                                        list<string>& _rel_type_list,
                                        list<og_schema_object_ptr>* _schm_obj_list)
{
  list<og::core::schema_object_ptr> objs;
  schema_->get_connected_object_to(_from_o_id, _rel_type_list, &objs);

  og_converter::convert
  <list<og::core::schema_object_ptr>&, list<og_schema_object_ptr>*>
  (objs, _schm_obj_list);
}

void og_schema::get_connected_object_from(string _to_o_id,
    list<string>& _rel_type_list,
    list<og_schema_object_ptr>* _schm_obj_list)
{
  list<og::core::schema_object_ptr> objs;
  schema_->get_connected_object_from(_to_o_id, _rel_type_list, &objs);

  og_converter::convert
  <list<og::core::schema_object_ptr>&, list<og_schema_object_ptr>*>
  (objs, _schm_obj_list);
}

void og_schema::get_connected_relation_to(string _from_o_id,
    list<string>& _rel_type_list,
    list<og_schema_relation_ptr>* _schm_rel_list)
{
  list<og::core::schema_relation_ptr> rels;
  schema_->get_connected_relation_to(_from_o_id, _rel_type_list, &rels);

  og_converter::convert
  <list<og::core::schema_relation_ptr>&, list<og_schema_relation_ptr>*>
  (rels, _schm_rel_list);
}

void og_schema::get_connected_relation_from(string _to_o_id,
    list<string>& _rel_type_list,
    list<og_schema_relation_ptr>* _schm_rel_list)
{
  list<og::core::schema_relation_ptr> rels;
  schema_->get_connected_relation_from(_to_o_id, _rel_type_list, &rels);

  og_converter::convert
  <list<og::core::schema_relation_ptr>&, list<og_schema_relation_ptr>*>
  (rels, _schm_rel_list);
}

bool og_schema::validate_connect_by_type(string _otype_from, string _otype_to)
{
  return schema_->validate_connect_by_type(_otype_from, _otype_to);
}
bool og_schema::validate_connect_by_type_id(string _otype, string _oid_from,
    string _oid_to)
{
  return schema_->validate_connect_by_type_id(_otype, _oid_from, _oid_to);
}

void og_schema::delete_object(string _oid)
{
  schema_->delete_object(_oid);
}

} // namespace og;

