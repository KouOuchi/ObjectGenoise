#include "og/core/session_object.h"

#include "og/core/schema.h"

#include "og/core/where_condition.h"
#include "og/core/exception.h"
#include "og/core/mapping.h"

#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <soci.h>

namespace og
{
namespace core
{
namespace pt = boost::posix_time;

session_object::session_object(session* _session)
  : session_(_session)
{ }

session_object::session_object(session* _session, string _id,
                               schema_object_ptr _schm_obj)
  : session_(_session), schema_object_ptr_(_schm_obj)
{
  id_ = _id;
}
/*
session_object::session_object(session* _session)
  : session_(_session)
{

}
*/
session_object::~session_object()
{
  session_parameter_.clear();
}

void session_object::delete_object()
{
  if (!session_)
  {
    BOOST_THROW_EXCEPTION(exception() <<
                          exception_message("unexpected. invalid_session."));
  }

  session_->delete_object(id_);
}


// relation ===>
session_relation_ptr session_object::connect_to(session_object_ptr _sesn_obj,
    string _rel_type)
{
  list<string> rel_type_list;
  rel_type_list.push_back(_rel_type);

  list<schema_relation_ptr> schm_rel_list;

  session_->schema_->get_relation_by_type(rel_type_list, &schm_rel_list);
  if(schm_rel_list.size() == 0)
  {
    BOOST_THROW_EXCEPTION(exception() <<
                          exception_message("not found schema_relation."));
  }

//  if(schm_rel_list.size() != 1)
//  { BOOST_THROW_EXCEPTION(og::core::exception(error_code::more_than_one_schema_relation)); }

  return session_->connect(id_, _sesn_obj->get_id(), *(schm_rel_list.begin()));
}
session_relation_ptr session_object::connect_from(session_object_ptr _sesn_obj,
    string _rel_type)
{
  list<string> rel_type_list;
  rel_type_list.push_back(_rel_type);

  list<schema_relation_ptr> schm_rel_list;

  session_->schema_->get_relation_by_type(rel_type_list, &schm_rel_list);
  if(schm_rel_list.size() == 0)
  {
    BOOST_THROW_EXCEPTION(exception() <<
                          exception_message("not found schema_relation."));
  }

  if (schm_rel_list.size() != 1)
  {
    BOOST_THROW_EXCEPTION(exception() <<
                          exception_message("found more than one schema_relation"));
  }

  return session_->connect(_sesn_obj->get_id(), id_, *(schm_rel_list.begin()));
}

void session_object::disconnect(session_object_ptr _sesn_obj)
{
  list<session_relation_ptr> sesn_rel_list1;
  session_->get_relation(id_, _sesn_obj->get_id(), &sesn_rel_list1);

  list<session_relation_ptr> sesn_rel_list2;
  session_->get_relation(_sesn_obj->get_id(), id_, &sesn_rel_list2);

  for(list<session_relation_ptr>::iterator it = sesn_rel_list1.begin();
      it != sesn_rel_list1.end(); ++it)
  {
    (*it)->delete_relation();
  }
  for(list<session_relation_ptr>::iterator it = sesn_rel_list2.begin();
      it != sesn_rel_list2.end(); ++it)
  {
    (*it)->delete_relation();
  }
}

void session_object::get_connected_object(
  list<session_object_ptr>* _sesn_obj_list)
{
  list<string> rel_type_list;
  get_connected_object(rel_type_list, _sesn_obj_list);
}

void session_object::get_connected_object(list<string>& _rel_type_list,
    list<session_object_ptr>* _sesn_obj_list)
{
  list<session_object_ptr> sesn_obj_list1;
  session_->get_connected_object_from(id_, _rel_type_list, &sesn_obj_list1);
  list<session_object_ptr> sesn_obj_list2;
  session_->get_connected_object_to(id_, _rel_type_list, &sesn_obj_list2);

  _sesn_obj_list->merge(sesn_obj_list1);
  _sesn_obj_list->merge(sesn_obj_list2);
  _sesn_obj_list->unique();
}

void session_object::get_connected_object_from(
  list<session_object_ptr>* _sesn_obj_list)
{
  list<string> rel_type_list;
  session_->get_connected_object_from(id_, rel_type_list, _sesn_obj_list);
}

void session_object::get_connected_object_from(list<string>& _rel_type_list,
    list<session_object_ptr>* _sesn_obj_list)
{
  session_->get_connected_object_from(id_, _rel_type_list, _sesn_obj_list);
}

void session_object::get_connected_object_to(list<session_object_ptr>*
    _sesn_obj_list)
{
  list<string> rel_type_list;
  session_->get_connected_object_to(id_, rel_type_list, _sesn_obj_list);
}

void session_object::get_connected_object_to(list<string>& _rel_type_list,
    list<session_object_ptr>* _sesn_obj_list)
{
  session_->get_connected_object_to(id_, _rel_type_list, _sesn_obj_list);
}

bool session_object::validate_connect_to(session_object_ptr _sesn_obj_ptr)
{
  list<string> rel_type_list;

  return session_->validate_connect
         (id_, _sesn_obj_ptr->get_id(), rel_type_list);
}

bool session_object::validate_connect_to(session_object_ptr _sesn_obj_ptr,
    string _rel_type)
{
  list<string> rel_type_list;
  rel_type_list.push_back(_rel_type);

  return session_->validate_connect
         (id_, _sesn_obj_ptr->get_id(), rel_type_list);
}

bool session_object::validate_connect_from(session_object_ptr
    _sesn_obj_ptr)
{
  list<string> rel_type_list;

  return session_->validate_connect
         (_sesn_obj_ptr->get_id(), id_, rel_type_list);
}

bool session_object::validate_connect_from(session_object_ptr
    _sesn_obj_ptr,
    string _rel_type)
{
  list<string> rel_type_list;
  rel_type_list.push_back(_rel_type);

  return session_->validate_connect
         (_sesn_obj_ptr->get_id(), id_, rel_type_list);
}

void session_object::get_connectable_relation_type_to(list<string>*
    _rel_type_list)
{
  _rel_type_list->clear();
  list<string> all_rel_type;
  list<schema_relation_ptr> schm_rel_list;

  session_->schema_->get_connected_relation_to(
    get_schema_object_id(), all_rel_type, &schm_rel_list);

  for(list<schema_relation_ptr>::iterator it = schm_rel_list.begin();
      it != schm_rel_list.end(); ++it)
  {
    _rel_type_list->push_back((*it)->get_type());
  }

  _rel_type_list->unique();
}
void session_object::get_connectable_relation_type_from(
  list<string>* _rel_type_list)
{
  _rel_type_list->clear();
  list<string> all_rel_type;
  list<schema_relation_ptr> schm_rel_list;

  session_->schema_->get_connected_relation_from(
    get_schema_object_id(), all_rel_type, &schm_rel_list);

  for(list<schema_relation_ptr>::iterator it = schm_rel_list.begin();
      it != schm_rel_list.end(); ++it)
  {
    _rel_type_list->push_back((*it)->get_type());
  }

  _rel_type_list->unique();
}
void session_object::get_connectable_to(map<string, list<schema_object_ptr> >*
                                        _rel_type_map)
{
  _rel_type_map->clear();
  list<string> all_rel_type;
  list<schema_relation_ptr> schm_rel_list;

  session_->schema_->get_connected_relation_to(
    get_schema_object_id(), all_rel_type, &schm_rel_list);

  for(list<schema_relation_ptr>::iterator itl = schm_rel_list.begin();
      itl != schm_rel_list.end(); ++itl)
  {
    optional<schema_object_ptr> schm_obj(
      session_->schema_->get_object( (*itl)->get_to_id() ));

    map<string, list<schema_object_ptr> >::iterator itm =
      _rel_type_map->find((*itl)->get_type());
    if(itm == _rel_type_map->end())
    {
      list<schema_object_ptr> schm_obj_list;
      schm_obj_list.push_back(schm_obj.get());
      _rel_type_map->insert(std::make_pair(string((*itl)->get_type()),
                                           schm_obj_list));
    }
    else
    {
      itm->second.push_back(schm_obj.get());
    }
  }
}
void  session_object::get_connectable_from(map<string, list<schema_object_ptr> >*
    _rel_type_map)
{
  _rel_type_map->clear();
  list<string> all_rel_type;
  list<schema_relation_ptr> schm_rel_list;

  session_->schema_->get_connected_relation_from(
    get_schema_object_id(), all_rel_type, &schm_rel_list);

  for(list<schema_relation_ptr>::iterator itl = schm_rel_list.begin();
      itl != schm_rel_list.end(); ++itl)
  {
    optional<schema_object_ptr> schm_obj(
      session_->schema_->get_object( (*itl)->get_from_id() ));

    map<string, list<schema_object_ptr> >::iterator itm =
      _rel_type_map->find((*itl)->get_type());
    if(itm == _rel_type_map->end())
    {
      list<schema_object_ptr> schm_obj_list;
      schm_obj_list.push_back(schm_obj.get());
      _rel_type_map->insert(std::make_pair(string((*itl)->get_type()),
                                           schm_obj_list));
    }
    else
    {
      itm->second.push_back(schm_obj.get());
    }
  }
}

// <=== relation

// schema ===>
string session_object::get_schema_object_id()
{
  return schema_object_ptr_->get_id();
}
schema_object_ptr session_object::get_schema_object()
{
  return schema_object_ptr_;
}
void session_object::set_schema_object_by_id(string _id)
{
  optional<schema_object_ptr> o(session_->schema_->get_object(_id));
  if(o.is_initialized())
  {
    schema_object_ptr_.swap(o.get());
  }
}
void session_object::set_schema_object(schema_object_ptr _schm_obj)
{
  schema_object_ptr_.swap(_schm_obj);
}

string session_object::get_type()
{
  return schema_object_ptr_->get_type();
}

void session_object::sync()
{
//  if(!updated_) { return; }

  // update record
  session_->sync_object(this);

//  set_updated(false);
}
// <=== schema

optional<session_parameter_ptr> session_object::get_parameter(
  std::string _param_name)
{
  map<string, session_parameter_ptr>::iterator it =
    session_parameter_.find(_param_name);

  if (it == session_parameter_.end())
  {
    return optional<session_parameter_ptr>();
  }
  else
  {
    return optional<session_parameter_ptr>(it->second);
  }
}
void session_object::set_parameter(string _param_name,
                                     list<parameter_value_variant>& _values)
{
  optional<session_parameter_ptr> p( get_parameter(_param_name) );

  if (!p.is_initialized()) { throw og::core::exception() << exception_message("unexpected."); }

  p.get()->values_.clear();
  p.get()->values_.assign(_values.begin(), _values.end());
}




} //namespace core;
} //namespace og;
