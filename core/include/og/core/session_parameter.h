#pragma once
#include "og/core/session_base.h"
#include "og/core/schema_parameter.h"

namespace og
{
namespace core
{

class session;

/*template <typename T>*/
class session_parameter
{
public:
  //session_parameter(session* _session);
  session_parameter(list<parameter_value_variant> _values,
                    schema_parameter_ptr _schm_par);
  virtual ~session_parameter();

  /*list<T> values_;*/
  list<parameter_value_variant> values_;

  schema_parameter_ptr schema_parameter_;
  //session* session_;

  static void dump_parameter(map<string, session_parameter_ptr> &_param_map)
  {
    for (map<string, session_parameter_ptr>::iterator mip = _param_map.begin();
         mip != _param_map.end(); mip++)
    {
      OG_LOG << mip->first;

      for (list<parameter_value_variant>::iterator vip = mip->second->values_.begin();
           vip != mip->second->values_.end(); vip++)
      {
        if (vip->type() == typeid(string))
        {
          OG_LOG << boost::get<string>(*vip);
        }
        if (vip->type() == typeid(double))
        {
          OG_LOG << boost::get<double>(*vip);
        }
        if (vip->type() == typeid(int))
        {
          OG_LOG << boost::get<int>(*vip);
        }
      }
    }
  }


};

} //namespace core;
} //namespace og;
