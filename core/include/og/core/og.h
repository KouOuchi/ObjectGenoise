#pragma once

#include <string>
#include <list>
#include <vector>
#include <map>

#include <boost/foreach.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>

#include <boost/variant.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/optional.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#ifndef OG_LOG_STDERR
#include <boost/log/trivial.hpp>
#define OG_LOG BOOST_LOG_TRIVIAL(debug)
#else
#define OG_LOG std::cerr
#endif

//#if defined(WIN32)
//#if defined(CORE_LIBRARY_EXPORT)
//#define COREAPI  __declspec( dllexport )
//#else
//#define COREAPI  __declspec( dllimport )
//#endif
//#else //WIN32
//#define COREAPI
//#endif

namespace og
{
namespace core
{

namespace pt = boost::posix_time;

using boost::shared_ptr;
using boost::scoped_ptr;
using boost::optional;

using std::vector;

using std::string;
using std::list;
using std::tm;
using std::pair;
using std::map;


// parameter_basetype_variant order
enum parameter_basetype_enum
{
  blank = 0,
  integer = 1,
  real = 2,
  text = 3,
  select = 4
};

typedef boost::variant <
boost::blank,
      int, //interger
      double, //real
      std::string, //text
      boost::tuple<int, std::string> //select
      > parameter_value_variant;

class parameter_basetype_base
{
public:
  parameter_basetype_base() {};
  parameter_basetype_base(const parameter_basetype_base& _t) : id_(_t.id_) {}
  parameter_basetype_base(string _id) : id_(id_) {}
  virtual ~parameter_basetype_base() {}
  string id_;
  virtual parameter_basetype_enum basetype_id() = 0;
};

class parameter_basetype_integer : public parameter_basetype_base
{
public:
  virtual ~parameter_basetype_integer() {}
  parameter_basetype_integer() {}
  parameter_basetype_integer(string _id)
  {
    id_ = _id;
  }

  parameter_basetype_integer(const parameter_basetype_integer& _t)
    : default_value_(_t.default_value_), system_min_(_t.system_min_),
      system_max_(_t.system_max_),
      warn_min_(_t.warn_min_), warn_max_(_t.warn_max_)
  {
    id_ = _t.id_;
  }
  parameter_basetype_integer(int _def, int _sys_min, int _sys_max,
                             int _war_min, int _war_max)
    :default_value_(_def), system_min_(_sys_min), system_max_(_sys_max),
     warn_min_(_war_min), warn_max_(_war_max)
  {}
  int default_value_;
  int system_min_;
  int system_max_;
  int warn_min_;
  int warn_max_;

  virtual parameter_basetype_enum basetype_id()
  {
    return parameter_basetype_enum::integer;
  }
};

class parameter_basetype_real : public parameter_basetype_base
{
public:
  virtual ~parameter_basetype_real() {}
  parameter_basetype_real() {}
  parameter_basetype_real(string _id)
  {
    id_ = _id;
  }
  parameter_basetype_real(const parameter_basetype_real& _t)
    : default_value_(_t.default_value_), system_min_(_t.system_min_),
      system_max_(_t.system_max_),
      warn_min_(_t.warn_min_), warn_max_(_t.warn_max_)
  {
    id_ = _t.id_;
  }
  parameter_basetype_real(double _def, double _sys_min, double _sys_max,
                          double _war_min, double _war_max)
    :default_value_(_def), system_min_(_sys_min), system_max_(_sys_min),
     warn_min_(_war_min), warn_max_(_war_max)
  {}
  double default_value_;
  double system_min_;
  double system_max_;
  double warn_min_;
  double warn_max_;
  virtual parameter_basetype_enum basetype_id()
  {
    return parameter_basetype_enum::real;
  }
};

class parameter_basetype_text : public parameter_basetype_base
{
public:
  virtual ~parameter_basetype_text() {}
  parameter_basetype_text() {}
  parameter_basetype_text(string _id)
  {
    id_ = _id;
  }
  parameter_basetype_text(const parameter_basetype_text& _t)
    : default_value_(_t.default_value_), system_min_(_t.system_min_),
      system_max_(_t.system_max_),
      warn_min_(_t.warn_min_), warn_max_(_t.warn_max_)
  {
    id_ = _t.id_;
  }
  parameter_basetype_text(string _def, int _sys_min, int _sys_max,
                          int _war_min, int _war_max)
    :default_value_(_def), system_min_(_sys_min), system_max_(_sys_min),
     warn_min_(_war_min), warn_max_(_war_max)
  {}
  string default_value_;
  int system_min_;
  int system_max_;
  int warn_min_;
  int warn_max_;
  virtual parameter_basetype_enum basetype_id()
  {
    return parameter_basetype_enum::text;
  }
};

class parameter_basetype_select : public parameter_basetype_base
{
public:
  virtual ~parameter_basetype_select() {}
  long defult_item_index_;
  bool enable_multi_select_;
  virtual parameter_basetype_enum basetype_id()
  {
    return parameter_basetype_enum::select;
  }
};

class parameter_basetype_select_item
{
public:
  virtual ~parameter_basetype_select_item() {}
  unsigned long item_index_;
  string item_name_;
  string comment_;
};
/*
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
      _list2->push_back(*it);
    }
  }
};
*/
} //namespace core;
} //namespace og;
