#pragma once

#include "og/core/schema.h"
#include "og/core/schema_object.h"
#include <sstream>

namespace og
{
namespace core
{

using std::stringstream;

template <class T>
struct where_condition : std::unary_function<string, string>
{
  where_condition(const list<T>& _arg_list, string _arg_name)
    : arg_list_(_arg_list), arg_name_(_arg_name) { }

  result_type operator()() const
  {
    // NOTE: if no arg_list, condition is true
    if (arg_list_.size() == 0) { return ""; }

    stringstream ss;
    ss << " AND (" << arg_name_ << " IN (";

    for(list<T>::const_iterator it = arg_list_.begin();
        it != arg_list_.end(); ++it)
    {
      ss << "'" << get_value(*it) << "'";

      list<T>::const_iterator it2(it);

      if( ++it2 != arg_list_.end() )
      { ss << ","; }
    }

    ss << "))";

#ifdef DEBUG
    std::cout << ss.str() << std::endl;
#endif
    return ss.str();
  }

  template <class T>
  string get_value(const T& _v) const { return _v; }

  //specialized for schema_object_ref
  template <>
  string get_value<schema_object_ptr>(const schema_object_ptr& _v) const
  {
    return _v->get_id();
  }

  list<T> arg_list_;
  string arg_name_;
};


template <class T>
struct where_clause : std::unary_function<string, string>
{
  where_clause(string _clause) : clause_(_clause)
  { }

  result_type operator()() const
  {
    stringstream ss;
    ss << " AND " << clause_;

#ifdef DEBUG
    std::cout << ss.str() << std::endl;
#endif
    return ss.str();
  }
  string clause_;
};

} //namespace core;
} //namespace og;
