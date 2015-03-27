#pragma once
#include "og/core/base.h"
#include <boost/tuple/tuple.hpp>

#include <soci.h>

namespace og
{
namespace core
{

template <typename T>
struct fetcher
{
  static void fetch(soci::session& _session, string _query, list<T>* _list)
  {
    vector<T> result_vec(BATCH_SIZE);
    soci::statement st =
      (
        _session.prepare <<
        _query
        , soci::into(result_vec)
      );

    fetch_body(st, result_vec, _list);
  }

  static void fetch_use1(soci::session& _session, string _query,
                         soci::details::use_type_ptr& _use,
                         list<T>* _list)
  {
    vector<T> result_vec(BATCH_SIZE);
    soci::statement st =
      (
        _session.prepare <<
        _query
        , _use
        , soci::into(result_vec)
      );

    fetch_body(st, result_vec, _list);
  }
  static void fetch_use2(soci::session& _session, string _query,
                         soci::details::use_type_ptr& _use,
                         soci::details::use_type_ptr& _use2, list<T>* _list)
  {
    vector<T> result_vec(BATCH_SIZE);
    soci::statement st =
      (
        _session.prepare <<
        _query
        , _use
        , _use2
        , soci::into(result_vec)
      );

    fetch_body(st, result_vec, _list);
  }

  static void fetch_body(soci::statement& _stmt, vector<T>& _result_vec,
                         list<T>* _list)
  {
    _stmt.execute();
    while (_stmt.fetch())
    {
      vector<T>::iterator pos;
      for(pos = _result_vec.begin(); pos != _result_vec.end(); ++pos)
      {
        _list->push_back(*pos);
      }

      _result_vec.resize(BATCH_SIZE);
    }
  }

};


// tuple version fetcher
template <typename T1, typename T2>
struct tuple_fetcher
{
  static void fetch_use1
  (soci::session& _session,
   string _query, soci::details::use_type_ptr& _use,
   list<boost::tuple<T1, T2>>* _list)
  {
    vector<T1> result_vec1(BATCH_SIZE);
    vector<T2> result_vec2(BATCH_SIZE);

    soci::statement st =
      (
        _session.prepare <<
        _query
        , _use
        , soci::into(result_vec1)
        , soci::into(result_vec2)
      );

    fetch_body(st, result_vec1, result_vec2, _list);
  }

  static void fetch_body(soci::statement& _stmt,
                         vector<T1>& _result_vec1, vector<T2>& _result_vec2,
                         list<boost::tuple<T1, T2>>* _list)
  {
    _stmt.execute();
    while (_stmt.fetch())
    {
      vector<T1>::iterator pos1 = _result_vec1.begin();
      vector<T2>::iterator pos2 = _result_vec2.begin();
      for(; pos1 != _result_vec1.end() && pos2 != _result_vec2.end(); ++pos1, ++pos2)
      {
        _list->push_back(boost::make_tuple<T1, T2>(*pos1, *pos2));
      }

      _result_vec1.resize(BATCH_SIZE);
      _result_vec2.resize(BATCH_SIZE);
    }
  }

};
}//namespace core
}//namespace og


