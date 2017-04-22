#include "og/og_session.h"
#include "og/og_schema_object.h"
#include "og/og_session_object.h"
#include "og/og_converter.h"

#include "og/core/schema.h"
#include "og/core/session.h"
#include "og/core/session_object.h"

namespace og
{
og_session::og_session() :
  session_(new og::core::session()),
  og_schema_(new og_schema(session_->schema_.get()))
{
}

og_session::~og_session()
{}

void og_session::open(string _connection_string)
{
  session_->open(_connection_string);
}

void og_session::close()
{
  session_->close();
}

void og_session::purge()
{
  session_->purge(true);
}

og_schema* og_session::schema()
{
  return og_schema_.get();
}
void og_session::build()
{
  return session_->build();
}

og_session_object_ptr og_session::create_object(og_schema_object_ptr _schm_obj)
{
  og::core::session_object_ptr o = session_->create_object(
                                     _schm_obj->schema_object_ptr_);
  return og_session_object_ptr(new og_session_object(o));
}

void og_session::delete_object(string _id)
{
  get_object(_id)->get()->delete_object();
}

bool og_session::import_from_file(string _path)
{
  return session_->import_from_file(_path);
}
void og_session::export_to_file(string _path)
{
  session_->export_to_file(_path);
}
bool og_session::catchup_schema(string _path)
{
  return session_->catchup_schema(_path);
}

optional<og_session_object_ptr> og_session::get_object(string _id)
{
  optional<og::core::session_object_ptr> o = session_->get_object(_id);
  if (!o.is_initialized()) { return optional<og_session_object_ptr>(); }
  else	{ return  optional<og_session_object_ptr>(new og_session_object(o.get())); }
}

void og_session::get_object(list<string>& _oid_list,
                            list<og_session_object_ptr>* _sesn_obj_list)
{
  list<og::core::session_object_ptr> objs;
  session_->get_object(_oid_list, &objs);

  og_converter::convert
  <list<og::core::session_object_ptr>&, list<og_session_object_ptr>*>
  (objs, _sesn_obj_list);
}

void og_session::get_object_by_type(list<string>& _otype_list,
                                    list<og_session_object_ptr>* _sesn_obj_list)
{
  list<og::core::session_object_ptr> objs;
  session_->get_object_by_type(_otype_list, &objs);

  og_converter::convert
  <list<og::core::session_object_ptr>&, list<og_session_object_ptr>*>
  (objs, _sesn_obj_list);
}

void og_session::get_object_by_name(list<string>& _oname_list,
                                    list<og_session_object_ptr>* _sesn_obj_list)
{
  list<og::core::session_object_ptr> objs;
  session_->get_object_by_name(_oname_list, &objs);

  og_converter::convert
  <list<og::core::session_object_ptr>&, list<og_session_object_ptr>*>
  (objs, _sesn_obj_list);
}

void og_session::get_object_by_schema_object_name(list<string>&
    _schm_obj_oname_list,
    list<og_session_object_ptr>* _sesn_obj_list)
{
  list<og::core::session_object_ptr> objs;
  session_->get_object_by_schema_object_name(_schm_obj_oname_list, &objs);

  og_converter::convert
  <list<og::core::session_object_ptr>&, list<og_session_object_ptr>*>
  (objs, _sesn_obj_list);
}

void og_session::get_object_by_schema_object(list<og_schema_object_ptr>&
    _schm_obj_list,
    list<og_session_object_ptr>* _sesn_obj_list)
{
  list<og::core::schema_object_ptr> so;
  og_converter::convert
  <list<og_schema_object_ptr>&, list<og::core::schema_object_ptr>*>
  (_schm_obj_list, &so);

  list<og::core::session_object_ptr> objs;
  session_->get_object_by_schema_object(so, &objs);

  og_converter::convert
  <list<og::core::session_object_ptr>&, list<og_session_object_ptr>*>
  (objs, _sesn_obj_list);
}

void og_session::get_connected_object_from(string _to_id,
    list<string> _rel_type_list,
    list<og_session_object_ptr>* _sesn_obj_list)
{
  list<og::core::session_object_ptr> objs;
  session_->get_connected_object_from(_to_id, _rel_type_list, &objs);

  og_converter::convert
  <list<og::core::session_object_ptr>&, list<og_session_object_ptr>*>
  (objs, _sesn_obj_list);
}
void og_session::get_connected_object_to(string _from_id,
    list<string> _rel_type_list,
    list<og_session_object_ptr>* _sesn_obj_list)
{
  list<og::core::session_object_ptr> objs;
  session_->get_connected_object_to(_from_id, _rel_type_list, &objs);

  og_converter::convert
  <list<og::core::session_object_ptr>&, list<og_session_object_ptr>*>
  (objs, _sesn_obj_list);
}

boost::optional<og_session_relation_ptr> og_session::get_relation(
  string _rel_id)
{
  boost::optional<og::core::session_relation_ptr> r =
    session_->get_relation(_rel_id);

  if (!r.is_initialized())
  {
    return boost::optional<og_session_relation_ptr>();
  }
  else
  {
    return boost::optional<og_session_relation_ptr>
           (
             og_session_relation_ptr(
               new og_session_relation(r.get())));
  }
}
void og_session::get_relation(list<og_session_relation_ptr>* _sesn_rel_list)
{
  list<og::core::session_relation_ptr> rels;
  session_->get_relation(&rels);

  og_converter::convert
  <list<og::core::session_relation_ptr>&, list<og_session_relation_ptr>*>
  (rels, _sesn_rel_list);
}
void og_session::get_relation(list<string> _rel_id_list,
                              list<og_session_relation_ptr>* _sesn_rel_list)
{
  list<og::core::session_relation_ptr> rels;
  session_->get_relation(_rel_id_list, &rels);

  og_converter::convert
  <list<og::core::session_relation_ptr>&, list<og_session_relation_ptr>*>
  (rels, _sesn_rel_list);
}
void og_session::get_relation(string _from_id, string _to_id,
                              list<og_session_relation_ptr>* _sesn_rel_list)
{
  list<og::core::session_relation_ptr> rels;
  session_->get_relation(_from_id, _to_id, &rels);

  og_converter::convert
  <list<og::core::session_relation_ptr>&, list<og_session_relation_ptr>*>
  (rels, _sesn_rel_list);
}
void og_session::get_relation_by_type(list<string> _rel_type_list,
                                      list<og_session_relation_ptr>* _sesn_rel_list)
{
  list<og::core::session_relation_ptr> rels;
  session_->get_relation_by_type(_rel_type_list, &rels);

  og_converter::convert
  <list<og::core::session_relation_ptr>&, list<og_session_relation_ptr>*>
  (rels, _sesn_rel_list);
}
void og_session::get_relation_by_name(list<string> _rel_name_list,
                                      list<og_session_relation_ptr>* _sesn_rel_list)
{
  list<og::core::session_relation_ptr> rels;
  session_->get_relation_by_name(_rel_name_list, &rels);

  og_converter::convert
  <list<og::core::session_relation_ptr>&, list<og_session_relation_ptr>*>
  (rels, _sesn_rel_list);
}
void og_session::disconnect(string _rel_id)
{
  session_->get_relation(_rel_id).get()->delete_relation();
}

void og_session::get_object(list<og_session_object_ptr>* _sesn_obj_list)
{
  list<og::core::session_object_ptr> objs;
  session_->get_object(&objs);

  og_converter::convert
  <list<og::core::session_object_ptr>&, list<og_session_object_ptr>*>
  (objs, _sesn_obj_list);
}
og_session_object_ptr og_session::get_property_object()
{
  og::core::session_object_ptr sesn_obj = session_->get_property_object();
  if (sesn_obj == nullptr)
  {
    return nullptr;
  }
  else
  {
    return og_session_object_ptr(new og_session_object(sesn_obj));
  }
}

} // namespace og;

