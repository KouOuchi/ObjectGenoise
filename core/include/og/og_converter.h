#pragma once

#include "og/og_session.h"
#include "og/og_schema_object.h"
#include "og/og_session_object.h"
#include "og/og_session_relation.h"

#include "og/core/exception.h"

namespace og
{

struct og_converter
{
  template <typename T, typename U>
  static void convert(T _source,
                      U _target)
  {
	  throw og::core::exception() << og::core::exception_message("unexpected conversion.");
  }

  template <>
  static void convert
  (list<og::core::session_object_ptr>& _source,
   list<og_session_object_ptr>* _target)
  {
    _target->clear();

    for (list<og::core::session_object_ptr>::iterator it = _source.begin();
         it != _source.end();
         it++)
    {
      _target->push_back(og_session_object_ptr(new og_session_object(*it)));
    }
  }

  template <>
  static void convert
  (list<og::core::session_relation_ptr>& _source,
   list<og_session_relation_ptr>* _target)
  {
    _target->clear();

    for (list<og::core::session_relation_ptr>::iterator it = _source.begin();
         it != _source.end();
         it++)
    {
      _target->push_back(og_session_relation_ptr(new og_session_relation(*it)));
    }
  }

  template <>
  static void convert(list<og_schema_object_ptr>& _source,
                      list<og::core::schema_object_ptr>* _target)
  {
    _target->clear();

    for (list<og_schema_object_ptr>::iterator it = _source.begin();
         it != _source.end();
         it++)
    {
      _target->push_back((*it)->schema_object_ptr_);
    }
  }
  template <>
  static void convert(list<og::core::schema_object_ptr>& _source,
                      list<og_schema_object_ptr>* _target)
  {
    _target->clear();

    for (list<og::core::schema_object_ptr>::iterator it = _source.begin();
         it != _source.end();
         it++)
    {
		_target->push_back(og_schema_object_ptr(new og_schema_object(*it)));
    }
  }
  template <>
  static void convert(list<og::core::schema_relation_ptr>& _source,
                      list<og_schema_relation_ptr>* _target)
  {
    _target->clear();

    for (list<og::core::schema_relation_ptr>::iterator it = _source.begin();
         it != _source.end();
         it++)
    {
		_target->push_back(og_schema_relation_ptr(new og_schema_relation(*it)));
    }
  }

  template <>
  static void convert(map<string, list<og::core::schema_object_ptr>>& _source,
                      map<string, list<og_schema_object_ptr>>* _target)
  {
    _target->clear();

    for (map<string, list<og::core::schema_object_ptr>>::iterator it =
           _source.begin();
         it != _source.end();
         it++)
    {
      list<og_schema_object_ptr> l;

	  convert
	  <list<og::core::schema_object_ptr>&, list<og_schema_object_ptr>*>
      (it->second, &l);

      _target->insert(std::make_pair(it->first, l));
    }
  }
};


}// namespace og;