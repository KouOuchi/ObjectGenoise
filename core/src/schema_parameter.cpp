#include "og/core/schema_parameter.h"

namespace og
{
namespace core
{

schema_parameter::schema_parameter(schema* _schema) : schema_base(_schema)
{}

schema_parameter::schema_parameter(
  schema* _schema,
/*  parameter_basetype_enum _basetype,*/
  string _param_id, string _param_type,
  string _comment, int _default_array_size, int _min_array_size,
  int _max_array_size)
  : schema_base(_schema), 
    default_array_size_(_default_array_size), min_array_size_(_min_array_size),
    max_array_size_(_max_array_size)
{
  id_ = _param_id;
  type_ = _param_type;
//    name_ = _param_name;
//    schema_ = _schema;
  comment_ = _comment;
  //parameter_basetype_factory param_base_fac;
  //param_base_fac.create_parameter_base(_param_base, &basetypes_value_);
  /*set_basetype(_basetype);*/
}

schema_parameter::schema_parameter(schema* _schema, schema_parameter& _s)
  : schema_parameter(_schema)
{

/*  basetype_ = schema_parameter::convert_to_parameter_basetype_enum(
                _s.get_basetype());*/
  default_array_size_ = _s.get_default_array_size();
  min_array_size_ = _s.get_min_array_size();
  max_array_size_ = _s.get_max_array_size();
  id_ = _s.id_;
  type_ = _s.type_;
  comment_ = _s.comment_;
}

schema_parameter::~schema_parameter()
{}


//void schema_parameter::set_basetype(parameter_basetype_enum _bid) { basetype_ = _bid; }

/*
void schema_parameter::get_parameter_names(list<string>* _names)
{
  if (_names->size() != 0) { BOOST_THROW_EXCEPTION(og::core::exception() << exception_message("argument has list item.")); }

  schema_->get_parameter_names(id_, _names);
}
*/

// specified for bool
void schema_parameter::sync()
{
//  if(!updated_) { return; }

  // update record


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

} //namespace core;
} //namespace og;
