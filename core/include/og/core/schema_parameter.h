#pragma once
#include "og/core/base.h"
#include "og/core/schema_base.h"
#include "og/core/schema.h"

#include <boost/variant.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/blank.hpp>

namespace og
{
namespace core
{

class schema_object_parameter
{
public:
  OG_COREAPI virtual ~schema_object_parameter() {}
  OG_COREAPI schema_object_parameter() {}
  OG_COREAPI schema_object_parameter(string _schm_id, string _pid,
                                     string _param_name)
    : schema_object_id_(_schm_id), pid_(_pid), param_name_(_param_name)
  {}
  string schema_object_id_;
  string pid_;
  string param_name_;
//  string comment_;

protected:
  void revision_up();
};

class schema_relation_parameter
{
public:
  OG_COREAPI virtual ~schema_relation_parameter() {}
  OG_COREAPI schema_relation_parameter() {}
  OG_COREAPI schema_relation_parameter(string _schm_id, string _pid,
                                       string _param_name)
    : schema_relation_id_(_schm_id), pid_(_pid), param_name_(_param_name)
  {}
  string schema_relation_id_;
  string pid_;
  string param_name_;
//  string comment_;

protected:
  void revision_up();
};

template <typename T>
struct parameter_value_converter
{
  //template <typename T>
  void convert_from_variant_list_to_list(
    list<parameter_value_variant>& _list1,
    list<T>* _list2)
  {
    _list2->clear();

    for (list<parameter_value_variant>::iterator it = _list1.begin();
         it != _list1.end(); it++)
    {
      _list2->push_back(boost::get<T>(*it));
    }
  }

  //template <typename T>
  void convert_from_list_to_variant_list(
    list<T>& _list1,
    list<parameter_value_variant>* _list2)
  {
    _list2->clear();

    for (list<T>::iterator it = _list1.begin();
         it != _list1.end(); it++)
    {
      parameter_value_variant val(*it);
      parameter_check(val);

      _list2->push_back(val);
    }
  }

  void parameter_check(parameter_value_variant _val)
  {
    if (_val.type() == typeid(double))
    {
      double v = boost::get<double>(_val);
      if (std::isnan(v) || std::isinf(v))
      {
        throw og::core::exception() <<
                                    exception_message("nan or inf value(double) is not allowed.");
      }
    }
  }
};

class parameter_basetype_visitor : public boost::static_visitor<void>
{
public:
  virtual ~parameter_basetype_visitor() {}

//  void operator()(parameter_basetype_boolean& _p)
//  {}
  void operator()(parameter_basetype_integer& _p)
  {}
  void operator()(parameter_basetype_real& _p)
  {}
  void operator()(parameter_basetype_text& _p)
  {}
//  void operator()(parameter_basetype_select& _p)
//  {}
};

class schema_parameter : public schema_base
{

public:
  OG_COREAPI schema_parameter(schema* _schema);
  OG_COREAPI schema_parameter(schema* _schema,
                              /*                   parameter_basetype_enum _basetype,*/
                              string _param_id, string _param_type,
                              string _comment, int _default_array_size, int _min_array_size,
                              int _max_array_size);
  OG_COREAPI schema_parameter(schema* _schema, schema_parameter& _s);

  OG_COREAPI virtual ~schema_parameter();

  parameter_basetype_variant basetype_variant_;

  OG_COREAPI void initialize_basetype(parameter_basetype_enum _bid, string _id)
  {
    switch (_bid)
    {
    case parameter_basetype_enum::integer:
      basetype_variant_ =
        parameter_basetype_integer(_id);
      break;
    case parameter_basetype_enum::real:
      basetype_variant_ =
        parameter_basetype_real(_id);
      break;
    case parameter_basetype_enum::text:
      basetype_variant_ =
        parameter_basetype_text(_id);
      break;
    }
  }
//  {
//    basetype_ = _bid;
  /*
    switch (_bid)
    {
    case parameter_basetype_enum::integer:
      basetype_variant_ =
        parameter_basetype_integer();
      break;
    case parameter_basetype_enum::real:
      basetype_variant_ =
        parameter_basetype_real();
      break;
    case parameter_basetype_enum::text:
      basetype_variant_ =
        parameter_basetype_text();
      break;
    }
  */
//  }

  /*
  virtual void set_id(string _oid)
  {
    switch (basetype_)
    {
    case parameter_basetype_enum::integer:
      boost::get<parameter_basetype_integer>(basetype_variant_).id_ = _oid;
      break;
    case parameter_basetype_enum::real:
      boost::get<parameter_basetype_real>(basetype_variant_).id_ = _oid;
      break;
    case parameter_basetype_enum::text:
      boost::get<parameter_basetype_text>(basetype_variant_).id_ = _oid;
      break;
    }

    base::set_id(_oid);
  }
  */

  OG_COREAPI int get_basetype()
  {
    if (basetype_variant_.which() == (int)parameter_basetype_enum::blank)
    { throw og::core::exception() << exception_message("empty variant."); }

    return basetype_variant_.which();
  }

  OG_COREAPI int get_default_array_size()
  {
    return default_array_size_;
  }
  OG_COREAPI int get_min_array_size()
  {
    return min_array_size_;
  }
  OG_COREAPI int get_max_array_size()
  {
    return max_array_size_;
  }

  OG_COREAPI void set_default_array_size(int _size)
  {
    default_array_size_ = _size;
  }
  OG_COREAPI void set_min_array_size(int _size)
  {
    min_array_size_ = _size;
  }
  OG_COREAPI void set_max_array_size(int _size)
  {
    max_array_size_ = _size;
  }

  //void get_parameter_names(list<string>* _names);

  OG_COREAPI parameter_basetype_enum get_parameter_basetype_by_parameter_name(
    string _name);

  OG_COREAPI virtual string get_name()
  {
    BOOST_THROW_EXCEPTION(og::core::exception() <<
                          exception_message("not have get_name() method."));
  }

  /*
  void get_parameter_definition(string _param_name,
                                parameter_basetypes* _param_basetype);
  */

  static parameter_basetype_enum convert_to_parameter_basetype_enum(int _e)
  {
    switch (_e)
    {
    case (int)parameter_basetype_enum::blank:
      return parameter_basetype_enum::blank;
    case (int)parameter_basetype_enum::integer:
      return parameter_basetype_enum::integer;
    case (int)parameter_basetype_enum::real:
      return parameter_basetype_enum::real;
    case (int)parameter_basetype_enum::text:
      return parameter_basetype_enum::text;
    case (int)parameter_basetype_enum::select:
      return parameter_basetype_enum::select;
    default:
      BOOST_THROW_EXCEPTION(exception() <<
                            exception_message("unexpected type while convertion."));

    }
  };

  OG_COREAPI virtual void sync();
  OG_COREAPI virtual void revision_up() {};

private:
//  parameter_basetype_enum basetype_;
  int default_array_size_;
  int min_array_size_;
  int max_array_size_;
};

} //namespace core;
} //namespace og;
