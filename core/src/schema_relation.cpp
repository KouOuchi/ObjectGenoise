#include "og/core/schema_relation.h"
#include "og/core/schema.h"

#include <soci.h>
#include <sqlite3/soci-sqlite3.h>

namespace og
{
namespace core
{

schema_relation::schema_relation(schema* _schema)
  : from_multiplicity_(1, 1), to_multiplicity_(1, 0), schema_base(_schema)
{
}

schema_relation::schema_relation(
  schema* _schema, string _rel_id, string _rel_type,
  string _rel_name, string _from_o_id, string _to_o_id)
  : from_multiplicity_(1, 1), to_multiplicity_(1, 0), schema_base(_schema)
{
  id_ = _rel_id;
  type_ = _rel_type;
  name_ = _rel_name;
  schema_ = _schema;
  from_id_ = _from_o_id;
  to_id_ = _to_o_id;
}

schema_relation::~schema_relation()
{

}

void schema_relation::delete_relation()
{
  schema_->disconnect(id_);
}

void schema_relation::sync()
{
//  if(!updated_) { return; }

  // update record
  schema_->sync_relation(this);

  //map<string, schema_relation_parameter>::iterator it = parameters_.begin();
  //for (; it != parameters_.end(); it++)
  //{
  //  optional<schema_parameter_ptr> p = schema_->get_parameter(it->second.pid_);
  //  if (p.is_initialized())
  //  {
  //    schema_->add_relation_parameter_definition(id_, it->first, p.get());
  //  }
  //}
}

void schema_relation::get_parameters(
  list<boost::tuple<string, schema_parameter_ptr>>* _param_name_types)
{
  _param_name_types->clear();
  schema_->get_relation_parameters(id_, _param_name_types);

  //map<string, schema_relation_parameter>::iterator it = parameters_.begin();
  //for (; it != parameters_.end(); it++)
  //{
  //  optional<schema_parameter_ptr> p = schema_->get_parameter(it->second.pid_);
  //  if (p.is_initialized())
  //  {
  //    _param_name_types->push_back(
  //      boost::make_tuple<string, schema_parameter_ptr>(it->first, p.get()));
  //  }
  //}
}

void schema_relation::add_parameter_definition(string _param_name,
    schema_parameter_ptr _schm_par)
{
  schema_->add_relation_parameter_definition(id_, _param_name, _schm_par);
  //parameters_.insert(
  //  std::make_pair<string, schema_relation_parameter>(
  //    string(_param_name),
  //    schema_relation_parameter(id_, _schm_par->get_id(), _param_name)));
}
//void schema_relation::insert_parameter(string _param_name, string _pid)
//{
//  parameters_.insert(
//    std::make_pair<string, schema_relation_parameter>(string(_param_name),
//        schema_relation_parameter(id_, _pid, _param_name)));
//}

parameter_basetype_enum
schema_relation::get_parameter_basetype_enum_by_param_name(
  string _param_name)
{
  return schema_->get_object_parameter_basetype_enum_by_param_name(id_,
         _param_name);
}

} //namespace core
} //namespace og

