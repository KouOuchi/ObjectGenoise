#include "og/core/session_relation.h"

#include "og/core/schema.h"

#include "og/core/where_condition.h"
#include "og/core/exception.h"
#include "og/core/mapping.h"

#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <soci/soci.h>

namespace og
{
namespace core
{
namespace pt = boost::posix_time;

session_relation::session_relation(session* _session)
  : session_(_session)
{ }

session_relation::session_relation(session* _session, string _id,
                                   schema_relation_ptr _schm_rel)
  : session_(_session), schema_relation_ptr_(_schm_rel)
{
  id_ = _id;
}

session_relation::~session_relation()
{

}

// schema ===>
string session_relation::get_schema_relation_id()
{
  return schema_relation_ptr_->get_id();
}
schema_relation_ptr session_relation::get_schema_relation()
{
  return schema_relation_ptr_;
}
void session_relation::set_schema_relation_by_id(string id_)
{
  optional<schema_relation_ptr> o(session_->schema_->get_relation(id_));
  if(o.is_initialized())
  {
    schema_relation_ptr_.swap( o.get() );
  }
}
void session_relation::set_schema_relation(schema_relation_ptr _schm_rel)
{
  schema_relation_ptr_.swap( _schm_rel );
}

string session_relation::get_type()
{
  return schema_relation_ptr_->get_type();
}

void session_relation::sync()
{
  //if(!updated_) { return; }

  // update record
  session_->sync_relation(this);

  //set_updated(false);
}

void session_relation::delete_relation()
{
  map<string, session_parameter_ptr>* param_map(get_parameters());
  session_->disconnect(id_, param_map);
}

// <=== schema

optional<session_parameter_ptr> session_relation::get_parameter(
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

void session_relation::set_parameter(string _param_name,
                                     list<parameter_value_variant>& _values)
{
  optional<session_parameter_ptr> p( get_parameter(_param_name) );

  if (!p.is_initialized()) { throw og::core::exception() << exception_message("unexpected."); }

  p.get()->values_.clear();
  p.get()->values_.assign(_values.begin(), _values.end());
}

} // namespace core;
} // namespace og;
