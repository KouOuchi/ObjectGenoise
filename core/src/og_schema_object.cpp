#include "og/og_schema_object.h"
#include "og/og_schema_parameter.h"
#include "og/og_converter.h"

#include "og/core/schema_object.h"
#include "og/core/schema.h"

namespace og
{

og_schema_object::og_schema_object(og::core::schema_object_ptr _o) :
  schema_object_ptr_(_o)
{
}

og_schema_object::~og_schema_object()
{}

og_schema_relation_ptr og_schema_object::connect_from(og_schema_object_ptr
    _from, string _otype)
{
  return og_schema_relation_ptr(new og_schema_relation(
                                  schema_object_ptr_->connect_to(_from->schema_object_ptr_, _otype)));
}
og_schema_relation_ptr og_schema_object::connect_to(og_schema_object_ptr _to,
    string _otype)
{
  return og_schema_relation_ptr(new og_schema_relation(
                                  schema_object_ptr_->connect_to(_to->schema_object_ptr_, _otype)));
}
//void og_schema_object::disconnect()
//{
//  schema_object_ptr_->disconnect();
//}
//void og_schema_object::disconnect(og_schema_object_ptr _object)
//{
//  schema_object_ptr_->disconnect(_object->schema_object_ptr_);
//}

void og_schema_object::get_parameters(
  list<boost::tuple<string, og_schema_parameter_ptr>>*
  _param_name_types)
{
  _param_name_types->clear();

  list<boost::tuple<string, og::core::schema_parameter_ptr>> types;
  schema_object_ptr_->get_parameters(&types);

  list<boost::tuple<string, og::core::schema_parameter_ptr>>::iterator it =
        types.begin();

  for (; it != types.end(); it++)
  {
    _param_name_types->push_back(
      boost::make_tuple(it->get<0>(), og_schema_parameter_ptr(
                          new og_schema_parameter(it->get<1>()))));
  }
}

void og_schema_object::add_parameter_definition(string _param_name,
    const og_schema_parameter_ptr _schm_par)
{
  schema_object_ptr_->add_parameter_definition(_param_name,
      _schm_par->schema_parameter_ptr_);
}

og::core::parameter_basetype_enum
og_schema_object::get_parameter_basetype_enum_by_param_name(
  string _param_name)
{
  return schema_object_ptr_->get_parameter_basetype_enum_by_param_name(
           _param_name);
}
void og_schema_object::get_connected_object(list<og_schema_object_ptr>*
    _schm_obj_list)
{
  list<og::core::schema_object_ptr> objs;
  schema_object_ptr_->get_connected_object(&objs);

  og_converter::convert
  <list<og::core::schema_object_ptr>&, list<og_schema_object_ptr>*>
  (objs, _schm_obj_list);
}
void og_schema_object::get_connected_object(list<string>& _rel_type_list,
    list<og_schema_object_ptr>* _schm_obj_list)
{
  list<og::core::schema_object_ptr> objs;
  schema_object_ptr_->get_connected_object(_rel_type_list, &objs);

  og_converter::convert
  <list<og::core::schema_object_ptr>&, list<og_schema_object_ptr>*>
  (objs, _schm_obj_list);
}

void og_schema_object::get_connected_object_to(list<og_schema_object_ptr>*
    _schm_obj_list)
{
  list<og::core::schema_object_ptr> objs;
  schema_object_ptr_->get_connected_object_to(&objs);

  og_converter::convert
  <list<og::core::schema_object_ptr>&, list<og_schema_object_ptr>*>
  (objs, _schm_obj_list);

}
void og_schema_object::get_connected_object_to(list<string>& _rel_type_list,
    list<og_schema_object_ptr>* _schm_obj_list)
{
  list<og::core::schema_object_ptr> objs;
  schema_object_ptr_->get_connected_object_to(_rel_type_list, &objs);

  og_converter::convert
  <list<og::core::schema_object_ptr>&, list<og_schema_object_ptr>*>
  (objs, _schm_obj_list);
}

void og_schema_object::get_connected_object_from(list<og_schema_object_ptr>*
    _schm_obj_list)
{
  list<og::core::schema_object_ptr> objs;
  schema_object_ptr_->get_connected_object_from(&objs);

  og_converter::convert
  <list<og::core::schema_object_ptr>&, list<og_schema_object_ptr>*>
  (objs, _schm_obj_list);
}
void og_schema_object::get_connected_object_from(list<string>& _rel_type_list,
    list<og_schema_object_ptr>* _schm_obj_list)
{
  list<og::core::schema_object_ptr> objs;
  schema_object_ptr_->get_connected_object_from(_rel_type_list, &objs);

  og_converter::convert
  <list<og::core::schema_object_ptr>&, list<og_schema_object_ptr>*>
  (objs, _schm_obj_list);
}

string og_schema_object::get_id()
{
  return schema_object_ptr_->get_id();
}
string og_schema_object::get_type()
{
  return schema_object_ptr_->get_type();
}
string og_schema_object::get_name()
{
  return schema_object_ptr_->get_name();
}
void og_schema_object::set_name(string _name)
{
  schema_object_ptr_->set_name(_name);
  schema_object_ptr_->sync();
}
string og_schema_object::get_comment()
{
  return schema_object_ptr_->get_comment();
}
void og_schema_object::set_comment(string _comment)
{
  schema_object_ptr_->set_comment(_comment);
  schema_object_ptr_->sync();
}
string og_schema_object::get_revision()
{
  return schema_object_ptr_->get_revision();
}
string og_schema_object::get_create_date()
{
  return schema_object_ptr_->get_create_date();
}
string og_schema_object::get_update_date()
{
  return schema_object_ptr_->get_update_date();
}
void og_schema_object::delete_object()
{
  return schema_object_ptr_->delete_object();
}

} // namespace og;

