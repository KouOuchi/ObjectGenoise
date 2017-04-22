#include "og/og_session_object.h"
#include "og/og_session_relation.h"
#include "og/og_converter.h"
#include "og/og_session_parameter.h"

#include "og/core/session_object.h"
#include "og/core/session_parameter.h"

namespace og
{

og_session_object::og_session_object(og::core::session_object_ptr _o) :
  session_object_ptr_(_o)
{
}

og_session_object::~og_session_object()
{}

void og_session_object::disconnect(og_session_object_ptr _obj)
{
  session_object_ptr_->disconnect(_obj->session_object_ptr_);
}

og_session_relation_ptr og_session_object::connect_to(og_session_object_ptr
    _sesn_obj, string _rel_type)
{
  og::core::session_relation_ptr r = session_object_ptr_->connect_to(
                                       _sesn_obj->session_object_ptr_, _rel_type);
  return og_session_relation_ptr(
           new og_session_relation(r));
}
og_session_relation_ptr og_session_object::connect_from(
  og_session_object_ptr _sesn_obj,
  string _rel_type)
{
  og::core::session_relation_ptr r = session_object_ptr_->connect_from(
                                       _sesn_obj->session_object_ptr_, _rel_type);
  return og_session_relation_ptr(
           new og_session_relation(r));
}

void og_session_object::get_connected_object(list<og_session_object_ptr>*
    _sesn_obj_list)
{
  list<og::core::session_object_ptr> objs;
  session_object_ptr_->get_connected_object(&objs);

  og_converter::convert
  <list<og::core::session_object_ptr>&, list<og_session_object_ptr>*>
  (objs, _sesn_obj_list);
}
void og_session_object::get_connected_object(list<string>& _rel_type_list,
    list<og_session_object_ptr>* _sesn_obj_list)
{
  list<og::core::session_object_ptr> objs;
  session_object_ptr_->get_connected_object(_rel_type_list, &objs);

  og_converter::convert
  <list<og::core::session_object_ptr>&, list<og_session_object_ptr>*>
  (objs, _sesn_obj_list);
}
void og_session_object::get_connected_object_from(list<og_session_object_ptr>*
    _sesn_obj_list)
{
  list<og::core::session_object_ptr> objs;
  session_object_ptr_->get_connected_object_from(&objs);

  og_converter::convert
  <list<og::core::session_object_ptr>&, list<og_session_object_ptr>*>
  (objs, _sesn_obj_list);
}

void og_session_object::get_connected_object_from(list<string>& _rel_type_list,
    list<og_session_object_ptr>* _sesn_obj_list)
{
  list<og::core::session_object_ptr> objs;
  session_object_ptr_->get_connected_object_from(_rel_type_list, &objs);

  og_converter::convert
  <list<og::core::session_object_ptr>&, list<og_session_object_ptr>*>
  (objs, _sesn_obj_list);
}

void og_session_object::get_connected_object_to(list<og_session_object_ptr>*
    _sesn_obj_list)
{
  list<og::core::session_object_ptr> objs;
  session_object_ptr_->get_connected_object_to(&objs);

  og_converter::convert
  <list<og::core::session_object_ptr>&, list<og_session_object_ptr>*>
  (objs, _sesn_obj_list);
}

void og_session_object::get_connected_object_to(list<string>& _rel_type_list,
    list<og_session_object_ptr>* _sesn_obj_list)
{
  list<og::core::session_object_ptr> objs;
  session_object_ptr_->get_connected_object_to(_rel_type_list, &objs);

  og_converter::convert
  <list<og::core::session_object_ptr>&, list<og_session_object_ptr>*>
  (objs, _sesn_obj_list);
}

//connection_validation_result_enum og_session_object::validate_connect_to(og_session_object_ptr
//    _sesn_obj_ptr)
//{
//  return session_object_ptr_->validate_connect_to(
//           _sesn_obj_ptr->session_object_ptr_);
//}

og::core::connection_validation_result_enum og_session_object::validate_connect_to(og_session_object_ptr _sesn_obj_ptr,
    string _rel_type)
{
  return session_object_ptr_->validate_connect_to(
           _sesn_obj_ptr->session_object_ptr_, _rel_type);
}
//connection_validation_result_enum og_session_object::validate_connect_from(og_session_object_ptr
//    _sesn_obj_ptr)
//{
//  return session_object_ptr_->validate_connect_from(
//           _sesn_obj_ptr->session_object_ptr_);
//}
og::core::connection_validation_result_enum og_session_object::validate_connect_from(og_session_object_ptr
    _sesn_obj_ptr, string _rel_type)
{
  return session_object_ptr_->validate_connect_from(
           _sesn_obj_ptr->session_object_ptr_, _rel_type);
}

void og_session_object::get_connectable_relation_type_to(
  list<string>* _rel_type_list)
{
  session_object_ptr_->get_connectable_relation_type_to(_rel_type_list);
}
void og_session_object::get_connectable_relation_type_from(
  list<string>* _rel_type_list)
{
  session_object_ptr_->get_connectable_relation_type_from(_rel_type_list);
}

void og_session_object::get_connectable_to(
  map<string, list<og_schema_object_ptr> >* _rel_type_map)
{
  map < string, list<og::core::schema_object_ptr> > objmap;
  session_object_ptr_->get_connectable_to(&objmap);

  og_converter::convert
  <map<string, list<og::core::schema_object_ptr>>&, map<string,list<og_schema_object_ptr>>*>
      (objmap, _rel_type_map);
}

void og_session_object::get_connectable_from(
  map<string, list<og_schema_object_ptr> >* _rel_type_map)
{
  map<string, list<og::core::schema_object_ptr>> objmap;
  session_object_ptr_->get_connectable_from(&objmap);

  og_converter::convert
  <map<string, list<og::core::schema_object_ptr>>&, map<string, list<og_schema_object_ptr>>*>
      (objmap, _rel_type_map);
}

string og_session_object::get_id()
{
  return session_object_ptr_->get_id();
}
string og_session_object::get_instance_name()
{
  return session_object_ptr_->get_name();
}
void og_session_object::set_instance_name(string _name)
{
  session_object_ptr_->set_name(_name);
  session_object_ptr_->sync();
}
string og_session_object::get_comment()
{
  return session_object_ptr_->get_comment();
}
void og_session_object::set_comment(string _comment)
{
  session_object_ptr_->set_comment(_comment);
  session_object_ptr_->sync();
}
string og_session_object::get_revision()
{
  return session_object_ptr_->get_revision();
}
void og_session_object::set_revision(string _rev)
{
  session_object_ptr_->set_revision(_rev);
  session_object_ptr_->sync();
}
string og_session_object::get_create_date()
{
  return session_object_ptr_->get_create_date();
}
string og_session_object::get_update_date()
{
  return session_object_ptr_->get_update_date();
}

string og_session_object::get_schema_object_id()
{
  return session_object_ptr_->get_schema_object()->get_id();
}
string og_session_object::get_schema_object_type()
{
  return session_object_ptr_->get_schema_object()->get_type();
}
string og_session_object::get_schema_object_name()
{
  return session_object_ptr_->get_schema_object()->get_name();
}
string og_session_object::get_schema_object_comment()
{
  return session_object_ptr_->get_schema_object()->get_comment();
}
string og_session_object::get_schema_object_create_date()
{
  return session_object_ptr_->get_schema_object()->get_create_date();
}
string og_session_object::get_schema_object_update_date()
{
  return session_object_ptr_->get_schema_object()->get_update_date();
}
boost::optional<og_session_parameter_ptr> og_session_object::get_parameter(
  std::string _param_name)
{
  boost::optional<og::core::session_parameter_ptr> sp =
    session_object_ptr_->get_parameter(_param_name);

  if (!sp.is_initialized())
  {
    return boost::optional<og_session_parameter_ptr>();
  }
  else
  {
    return boost::optional<og_session_parameter_ptr>
           (og_session_parameter_ptr
            (new og_session_parameter
             (sp.get()->values_,
              og_schema_parameter_ptr
              (new og_schema_parameter(
                 sp.get()->schema_parameter_)))));
  }
}

void og_session_object::set_parameter(string _param_name,
                                      list<og::core::parameter_value_variant>& _values)
{
  session_object_ptr_->set_parameter(_param_name, _values);
}
void og_session_object::delete_object()
{
  session_object_ptr_->delete_object();
}

og_session_object_ptr og_session_object::copy_object()
{
  return copy_object(
           og::core::connection_direction_enum::direction_none);
}

void og_session_object::delete_object(
  og::core::connection_direction_enum _direction)
{
  session_object_ptr_->delete_object(_direction);
}

og_session_object_ptr og_session_object::copy_object(
  og::core::connection_direction_enum _direction)
{
  og::core::session_object_ptr o(session_object_ptr_->copy_object(_direction));
  og_session_object_ptr ret;

  og_converter::convert
  <og::core::session_object_ptr&, og_session_object_ptr*>
  (o, &ret);

  return ret;
}

void og_session_object::get_connected(
  list<boost::tuple<og_session_object_ptr, og_session_relation_ptr>>*
  _sesn_obj_rel_list)
{
  list<boost::tuple<og::core::session_object_ptr, og::core::session_relation_ptr>> obj_rels;
  session_object_ptr_->get_connected(&obj_rels);

  og_converter::convert
  <list<boost::tuple<og::core::session_object_ptr, og::core::session_relation_ptr>>&, list<boost::tuple<og_session_object_ptr, og_session_relation_ptr>>*>
  (obj_rels, _sesn_obj_rel_list);
}

void og_session_object::get_connected(list<string>& _rel_type_list,
                                      list<boost::tuple<og_session_object_ptr, og_session_relation_ptr>>*
                                      _sesn_obj_rel_list)
{
  list<boost::tuple<og::core::session_object_ptr, og::core::session_relation_ptr>> obj_rels;
  session_object_ptr_->get_connected(_rel_type_list, &obj_rels);

  og_converter::convert
  <list<boost::tuple<og::core::session_object_ptr, og::core::session_relation_ptr>>&, list<boost::tuple<og_session_object_ptr, og_session_relation_ptr>>*>
  (obj_rels, _sesn_obj_rel_list);
}

void og_session_object::get_connected_from(
  list<boost::tuple<og_session_object_ptr, og_session_relation_ptr>>*
  _sesn_obj_rel_list)
{
  list<boost::tuple<og::core::session_object_ptr, og::core::session_relation_ptr>> obj_rels;
  session_object_ptr_->get_connected_from(&obj_rels);

  og_converter::convert
  <list<boost::tuple<og::core::session_object_ptr, og::core::session_relation_ptr>>&, list<boost::tuple<og_session_object_ptr, og_session_relation_ptr>>*>
  (obj_rels, _sesn_obj_rel_list);
}
void og_session_object::get_connected_from(list<string>& _rel_type_list,
    list<boost::tuple<og_session_object_ptr, og_session_relation_ptr>>*
    _sesn_obj_rel_list)
{
  list<boost::tuple<og::core::session_object_ptr, og::core::session_relation_ptr>> obj_rels;
  session_object_ptr_->get_connected_from(_rel_type_list, &obj_rels);

  og_converter::convert
  <list<boost::tuple<og::core::session_object_ptr, og::core::session_relation_ptr>>&, list<boost::tuple<og_session_object_ptr, og_session_relation_ptr>>*>
  (obj_rels, _sesn_obj_rel_list);
}
void og_session_object::get_connected_to(
  list<boost::tuple<og_session_object_ptr, og_session_relation_ptr>>*
  _sesn_obj_rel_list)
{
  list<boost::tuple<og::core::session_object_ptr, og::core::session_relation_ptr>> obj_rels;
  session_object_ptr_->get_connected_to(&obj_rels);

  og_converter::convert
  <list<boost::tuple<og::core::session_object_ptr, og::core::session_relation_ptr>>&, list<boost::tuple<og_session_object_ptr, og_session_relation_ptr>>*>
  (obj_rels, _sesn_obj_rel_list);
}
void og_session_object::get_connected_to(list<string>& _rel_type_list,
    list<boost::tuple<og_session_object_ptr, og_session_relation_ptr>>*
    _sesn_obj_rel_list)
{
  list<boost::tuple<og::core::session_object_ptr, og::core::session_relation_ptr>> obj_rels;
  session_object_ptr_->get_connected_to(_rel_type_list, &obj_rels);

  og_converter::convert
  <list<boost::tuple<og::core::session_object_ptr, og::core::session_relation_ptr>>&, list<boost::tuple<og_session_object_ptr, og_session_relation_ptr>>*>
  (obj_rels, _sesn_obj_rel_list);
}

} // namespace og;