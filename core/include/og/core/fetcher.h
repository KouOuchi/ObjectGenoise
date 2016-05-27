#pragma once
#include "og/core/base.h"
#include <boost/tuple/tuple.hpp>

#include <soci.h>

namespace og
{
namespace core
{

template <typename S>
struct fetcher
{
  static void fetch(soci::session& _session, string _query, list<S>* _list)
  {
    vector<S> result_vec(BATCH_SIZE);
    soci::statement st =
      (
        _session.prepare <<
        _query
        , soci::into(result_vec)
      );

    fetch_body(st, result_vec, _list);
  }

  template <typename T>
  static void fetch_use1(soci::session& _session, string _query,
                         soci::details::use_container<T, soci::details::no_indicator>& _use,
                         list<S>* _list)
  {
    vector<S> result_vec(BATCH_SIZE);
    soci::statement st =
      (
        _session.prepare <<
        _query
        , _use
        , soci::into(result_vec)
      );

    fetch_body(st, result_vec, _list);
  }

  template <typename T>
  static void fetch_use1(soci::session& _session, string _query,
                         soci::details::use_container<T, soci::details::no_indicator>& _use,
                         list<S>* _list, list<S>* _list2)
  {
    vector<S> result_vec(BATCH_SIZE);
    vector<S> result_vec2(BATCH_SIZE);
    soci::statement st =
      (
        _session.prepare <<
        _query
        , _use
        , soci::into(result_vec)
        , soci::into(result_vec2)
      );

    fetch_body(st, result_vec, _list, result_vec2, _list2);
  }

  template <typename T>
  static void fetch_use2(soci::session& _session, string _query,
                         soci::details::use_container<T, soci::details::no_indicator>& _use,
                         soci::details::use_container<T, soci::details::no_indicator>& _use2,
                         list<S>* _list)
  {
    vector<S> result_vec(BATCH_SIZE);
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

  static void fetch_body(soci::statement& _stmt, vector<S>& _result_vec,
                         list<S>* _list)
  {
    _stmt.execute();
    while (_stmt.fetch())
    {
      vector<S>::iterator pos;
      for(pos = _result_vec.begin(); pos != _result_vec.end(); ++pos)
      {
        _list->push_back(*pos);
      }

      _result_vec.resize(BATCH_SIZE);
    }
  }

  static void fetch_body(soci::statement& _stmt, vector<S>& _result_vec,
                         list<S>* _list, vector<S>& _result_vec2,
                         list<S>* _list2)
  {
    _stmt.execute();
    while (_stmt.fetch())
    {
      vector<S>::iterator pos = _result_vec.begin();
      vector<S>::iterator pos2 = _result_vec2.begin();
      for(; pos != _result_vec.end(); ++pos, ++pos2)
      {
        _list->push_back(*pos);
        _list2->push_back(*pos2);
      }

      _result_vec.resize(BATCH_SIZE);
      _result_vec2.resize(BATCH_SIZE);
    }
  }

};


// tuple version fetcher
template <typename S1, typename S2>
struct tuple_fetcher
{

  template <typename T>
  static void fetch_use1
  (soci::session& _session,
   string _query, soci::details::use_container<T, soci::details::no_indicator>&
   _use,
   list<boost::tuple<S1, S2>>* _list)
  {
    vector<S1> result_vec1(BATCH_SIZE);
    vector<S2> result_vec2(BATCH_SIZE);

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
                         vector<S1>& _result_vec1, vector<S2>& _result_vec2,
                         list<boost::tuple<S1, S2>>* _list)
  {
    _stmt.execute();
    while (_stmt.fetch())
    {
      vector<S1>::iterator pos1 = _result_vec1.begin();
      vector<S2>::iterator pos2 = _result_vec2.begin();
      for(; pos1 != _result_vec1.end() && pos2 != _result_vec2.end(); ++pos1, ++pos2)
      {
        _list->push_back(boost::make_tuple<S1, S2>(*pos1, *pos2));
      }

      _result_vec1.resize(BATCH_SIZE);
      _result_vec2.resize(BATCH_SIZE);
    }
  }

};
}//namespace core
}//namespace og


