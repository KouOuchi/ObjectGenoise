#pragma once

#include "og/og_session.h"
#include "og/og_schema_object.h"
#include "og/og_schema_relation.h"
#include "og/og_transaction.h"
#include "og/og_session_object.h"
#include "og/og_session_relation.h"

#include <boost/tuple/tuple.hpp>
#include <boost/test/unit_test.hpp>

namespace og
{
namespace core
{
using namespace boost;
using namespace std;

struct og_session_relation_comparer
{
  static bool compare(og::og_session* session, og::og_session_relation_ptr _o1,
                      og::og_session_relation_ptr _o2)
  {
    BOOST_REQUIRE_EQUAL(_o1->get_schema_relation_id(), _o2->get_schema_relation_id());

    BOOST_REQUIRE_EQUAL(_o1->get_schema_relation_type(),
                        _o2->get_schema_relation_type());

    optional<og::og_schema_relation_ptr> schm_rel = session->og_schema_->get_relation(
          _o1->get_schema_relation_id());

    if (!schm_rel.is_initialized())
    {
      BOOST_FAIL("unexpected.");
    }

    list<boost::tuple<string, og_schema_parameter_ptr>> params;
    schm_rel.get()->get_parameters(&params);

    for (list<boost::tuple<string, og_schema_parameter_ptr>>::iterator it =
           params.begin(); it != params.end(); it++)
    {
      string pname = it->get<0>();

      og::core::parameter_basetype_enum ptype= schm_rel.get()->
          get_parameter_basetype_enum_by_param_name(pname);

      switch (ptype)
      {
	  case parameter_basetype_enum::integer:
      {
        list<int> o1val, o2val;
        _o1->get_parameter_values<int>(pname, &o1val);
        _o2->get_parameter_values<int>(pname, &o2val);
        list<int>::iterator oit1, oit2;
        for (oit1 = o1val.begin(), oit2 = o2val.begin();
             oit1 != o1val.end(); oit1++, oit2++)
        {
          BOOST_REQUIRE_EQUAL(*oit1, *oit2);
        }
      }
      break;
      case parameter_basetype_enum::real:
      {
        list<double> o1val, o2val;
        _o1->get_parameter_values<double>(pname, &o1val);
        _o2->get_parameter_values<double>(pname, &o2val);
        list<double>::iterator oit1, oit2;
        for (oit1 = o1val.begin(), oit2 = o2val.begin();
             oit1 != o1val.end(); oit1++, oit2++)
        {
          BOOST_REQUIRE_EQUAL(*oit1, *oit2);
        }
      }
      break;
      case parameter_basetype_enum::text:
      {
        list<string> o1val, o2val;
        _o1->get_parameter_values<string>(pname, &o1val);
        _o2->get_parameter_values<string>(pname, &o2val);
        list<string>::iterator oit1, oit2;
        for (oit1 = o1val.begin(), oit2 = o2val.begin();
             oit1 != o1val.end(); oit1++, oit2++)
        {
          BOOST_REQUIRE_EQUAL(*oit1, *oit2);
        }
      }
      break;

      default:
        BOOST_FAIL("unexpected.");

      }
    }
  return true;
  }

};

struct og_session_object_comparer
{
  static bool compare(og::og_session* session, og::og_session_object_ptr _o1,
                      og::og_session_object_ptr _o2)
  {
    BOOST_REQUIRE_EQUAL(_o1->get_schema_object_id(), _o2->get_schema_object_id());

    BOOST_REQUIRE_EQUAL(_o1->get_schema_object_type(),
                        _o2->get_schema_object_type());

    optional<og::og_schema_object_ptr> schm_obj = session->og_schema_->get_object(
          _o1->get_schema_object_id());

    if (!schm_obj.is_initialized())
    {
      BOOST_FAIL("unexpected.");
    }

    list<boost::tuple<string, og_schema_parameter_ptr>> params;
    schm_obj.get()->get_parameters(&params);

    for (list<boost::tuple<string, og_schema_parameter_ptr>>::iterator it =
           params.begin(); it != params.end(); it++)
    {
      string pname = it->get<0>();

      og::core::parameter_basetype_enum ptype= schm_obj.get()->
          get_parameter_basetype_enum_by_param_name(pname);

      switch (ptype)
      {
	  case parameter_basetype_enum::integer:
      {
        list<int> o1val, o2val;
        _o1->get_parameter_values<int>(pname, &o1val);
        _o2->get_parameter_values<int>(pname, &o2val);
        list<int>::iterator oit1, oit2;
        for (oit1 = o1val.begin(), oit2 = o2val.begin();
             oit1 != o1val.end(); oit1++, oit2++)
        {
          BOOST_REQUIRE_EQUAL(*oit1, *oit2);
        }
      }
      break;
      case parameter_basetype_enum::real:
      {
        list<double> o1val, o2val;
        _o1->get_parameter_values<double>(pname, &o1val);
        _o2->get_parameter_values<double>(pname, &o2val);
        list<double>::iterator oit1, oit2;
        for (oit1 = o1val.begin(), oit2 = o2val.begin();
             oit1 != o1val.end(); oit1++, oit2++)
        {
          BOOST_REQUIRE_EQUAL(*oit1, *oit2);
        }
      }
      break;
      case parameter_basetype_enum::text:
      {
        list<string> o1val, o2val;
        _o1->get_parameter_values<string>(pname, &o1val);
        _o2->get_parameter_values<string>(pname, &o2val);
        list<string>::iterator oit1, oit2;
        for (oit1 = o1val.begin(), oit2 = o2val.begin();
             oit1 != o1val.end(); oit1++, oit2++)
        {
          BOOST_REQUIRE_EQUAL(*oit1, *oit2);
        }
      }
      break;

      default:
        BOOST_FAIL("unexpected.");

      }
    }
  return true;
  }

};


}//namespace core
}//namespace og


