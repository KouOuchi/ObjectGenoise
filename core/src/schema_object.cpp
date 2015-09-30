#include "og/stdafx.h"

#include "og/core/schema_object.h"
#include "og/core/schema.h"

#include <soci.h>
#include <algorithm>

namespace og
{
namespace core
{

schema_object::schema_object(schema* _schema) : schema_base()
{
  schema_ = _schema;
}

schema_object::schema_object(schema* _schema, string _oid, string _otype,
                             string _oname) : schema_base()
{
  id_ = _oid;
  type_ = _otype;
  name_ = _oname;
  schema_ = _schema;
}

schema_object::~schema_object()
{

}

schema_relation_ptr schema_object::connect_from(schema_object_ptr _from,
    string _otype)
{
  return  schema_relation_ptr();
}

schema_relation_ptr schema_object::connect_to(schema_object_ptr _to,
    string _rel_type)
{
  return schema_->create_relation(_rel_type,  "", id_, _to->get_id());
}

void schema_object::disconnect()
{
  list<schema_relation_ptr> connected_rel_to;
  list<schema_relation_ptr> connected_rel_from;

  schema_->get_connected_relation_from(
    get_id(),
    list<string>(),
    &connected_rel_from);

  schema_->get_connected_relation_to(
    get_id(),
    list<string>(),
    &connected_rel_to);

  for (list<schema_relation_ptr>::iterator it = connected_rel_from.begin();
       it != connected_rel_from.end(); it++)
  {
    (*it)->delete_relation();
  }

  for (list<schema_relation_ptr>::iterator it = connected_rel_to.begin();
       it != connected_rel_to.end(); it++)
  {
    (*it)->delete_relation();
  }
}

void schema_object::disconnect(schema_object_ptr _schm_obj)
{
  list<schema_relation_ptr> connected_rel_to;
  list<schema_relation_ptr> connected_rel_from;

  schema_->get_connected_relation_from(
    get_id(),
    list<string>(),
    &connected_rel_from);

  schema_->get_connected_relation_to(
    get_id(),
    list<string>(),
    &connected_rel_to);

  for (list<schema_relation_ptr>::iterator it = connected_rel_from.begin();
       it != connected_rel_from.end(); it++)
  {
    if (it->get()->get_id().compare(_schm_obj->get_id()) == 0)
    {
      (*it)->delete_relation();
    }
  }

  for (list<schema_relation_ptr>::iterator it = connected_rel_to.begin();
       it != connected_rel_to.end(); it++)
  {
    if (it->get()->get_id().compare(_schm_obj->get_id()) == 0)
    {
      (*it)->delete_relation();
    }
  }
}

//schema_object_ptr schema_object::copy_object()
//{
//
//}

void schema_object::delete_object()
{
  if (!schema_)
  {
    BOOST_THROW_EXCEPTION(exception() <<
                          exception_message("unexpected. invalid schema"));
  }

  disconnect();

  list<boost::tuple<string, schema_parameter_ptr>> param_name_types;
  get_parameters(&param_name_types);
  schema_->delete_object(id_, &param_name_types);
}


void schema_object::sync()
{
  //  if(!updated_) { return; }

  // update record
  schema_->sync_object(this);

  //map<string, schema_object_parameter>::iterator it = parameters_.begin();
  //for (; it != parameters_.end(); it++)
  //{
  //  optional<schema_parameter_ptr> p = schema_->get_parameter(it->second.pid_);
  //  if (p.is_initialized())
  //  {
  //    schema_->add_object_parameter_definition(id_, it->first, p.get());
  //  }
  //}
}


void schema_object::get_connected_object(list<schema_object_ptr>*
    _schm_object_list)
{
  list<string> _rel_type_list;
  get_connected_object(_rel_type_list, _schm_object_list);
}

bool schema_object::equals(const schema_object_ptr& _x,
                           const schema_object_ptr& _y)
{
  return _x->get_id().compare(_y->get_id()) == 0;
}

void schema_object::get_connected_object(list<string>& _rel_type_list,
    list<schema_object_ptr>* _schm_object_list)
{
  _schm_object_list->clear();

  list<schema_object_ptr> list1;
  list<schema_object_ptr> list2;
  schema_->get_connected_object_to(id_, _rel_type_list, &list1);
  schema_->get_connected_object_from(id_, _rel_type_list, &list2);

  _schm_object_list->clear();

  _schm_object_list->splice(_schm_object_list->end(), list1);
  _schm_object_list->splice(_schm_object_list->end(), list2);
  std::unique(_schm_object_list->begin(), _schm_object_list->end(), &equals);
}

void schema_object::get_connected_object_to(list<schema_object_ptr>*
    _schm_object_list)
{
  list<string> _rel_type_list;
  schema_->get_connected_object_to(id_, _rel_type_list, _schm_object_list);
}
void schema_object::get_connected_object_to(list<string>& _rel_type_list,
    list<schema_object_ptr>* _schm_object_list)
{
  schema_->get_connected_object_to(id_, _rel_type_list, _schm_object_list);
}

void schema_object::get_connected_object_from(list<schema_object_ptr>*
    _schm_object_list)
{
  list<string> _rel_type_list;
  schema_->get_connected_object_from(id_, _rel_type_list, _schm_object_list);
}

void schema_object::get_connected_object_from(list<string>& _rel_type_list,
    list<schema_object_ptr>* _schm_object_list)
{
  schema_->get_connected_object_from(id_, _rel_type_list, _schm_object_list);
}

/**
 * TODO
 */
//string schema_object::get_parameter_definition(string _pname,
//    param_attrib_enum _p)
//{
//  return"";
//}


//string schema_object::get_parameter_type(string _pname)
//{
//
//  return  NULL;
//}


void schema_object::get_parameters(
  list<boost::tuple<string, schema_parameter_ptr>>* _param_name_types)
{
  _param_name_types->clear();
  schema_->get_object_parameters(id_, _param_name_types);
}

void schema_object::add_parameter_definition(string _param_name,
    schema_parameter_ptr _schm_par)
{
  schema_->add_object_parameter_definition(id_, _param_name, _schm_par);

  // set update date and revision
  set_updated();
  if(auto_revision_up_) { revision_up(); }
}

void schema_object::delete_parameter_definition(string _param_name,
    schema_parameter_ptr _schm_par)
{
  schema_->delete_object_parameter_definition(id_, _param_name, _schm_par);

  // set update date and revision
  set_updated();
  if(auto_revision_up_) { revision_up(); }
}

parameter_basetype_enum
schema_object::get_parameter_basetype_enum_by_param_name(
  string _param_name)
{
  return schema_->get_object_parameter_basetype_enum_by_param_name(id_,
         _param_name);
}


bool schema_object::validate_connect_From(string _otype)
{

  return false;
}

bool schema_object::validate_connect_from(schema_object_ptr _obj, string _otype)
{

  return false;
}

bool schema_object::validate_connect_to(string _otype)
{

  return false;
}

bool schema_object::validate_connect_to(schema_object_ptr _obj, string _otype)
{

  return false;
}

// return error_bit
bool schema_object::validate_parameter_valuerange()
{

  return false;
}

void schema_object::revision_up()
{
  schema_->revision_up_object(this);
}

} // namespace core
} // namespace og