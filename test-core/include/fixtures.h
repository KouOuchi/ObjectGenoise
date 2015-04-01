#pragma once

#define TEST_ALL 1

#ifdef TEST_ALL
#define TEST_OG_BASIC 1
#define TEST_OG_SCHEMA 1
#define TEST_OG_SESSION 1
#define TEST_OG_TRAN 1
#define TEST_OG_ETC 1
#define TEST_OG_LARGE_DATA 1
//#define TEST_REQUIRE_THROW 1
#endif

#include <boost/test/unit_test.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/foreach.hpp>

#include "soci.h"
#include "sqlite3/soci-sqlite3.h"

#include "og/og_session.h"
#include "og/og_schema_object.h"
#include "og/og_schema_relation.h"
#include "og/og_transaction.h"
#include "og/og_session_object.h"
#include "og/og_session_relation.h"

#include <string>
#include <exception>

#define DBPATH "../sql/og.db"

namespace pt = boost::posix_time;
using namespace std;
using namespace boost;

struct fixture_clean_session
{
  fixture_clean_session()
  {
    BOOST_TEST_MESSAGE("test-core session setup");
  }

  ~fixture_clean_session() {}

};

struct fixture_once
{
  fixture_once()
  {
    BOOST_TEST_MESSAGE("test-core global setup");
    og::og_session cleaned_session_;
    cleaned_session_.connect(DBPATH);
    cleaned_session_.build();
  }

  ~fixture_once() {}
};

