#pragma once
#define TEST_ALL 1
//#undef TEST_ALL
//#define TEST_ALL
//#define TEST_OG_BASIC 1
//#define TEST_OG_SCHEMA 1
//#define TEST_OG_SESSION 1
//#define TEST_OG_TRAN 1
//#define TEST_OG_ETC 1
//#define TEST_OG_LARGE_DATA 1
//#define TEST_OG_IMPORT_EXPORT 1

#ifdef TEST_ALL
#define TEST_OG_BASIC 1
#define TEST_OG_SCHEMA 1
#define TEST_OG_SESSION 1
#define TEST_OG_TRAN 1
#define TEST_OG_ETC 1
#define TEST_OG_LARGE_DATA 1
#define TEST_OG_IMPORT_EXPORT 1
#endif

#include <boost/test/unit_test.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/foreach.hpp>

#include "soci/soci.h"
#include "soci/sqlite3/soci-sqlite3.h"

#include "og/og_session.h"
#include "og/og_schema_object.h"
#include "og/og_schema_relation.h"
#include "og/og_transaction.h"
#include "og/og_session_object.h"
#include "og/og_session_relation.h"

#include <string>
#include <exception>
#include <boost/filesystem.hpp>

#define DBPATH u8"../../sql/ドキュメント/社会og.db"
//#define DBPATH u8"D:/git/ObjectGenoise/sql/ドキュメント/og.db"
//#define DBPATH u8"D:\\git\\ObjectGenoise\\sql\\ドキュメント\\og.db"

#define SRC_DBPATH "../../sql/og_src.db"

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

namespace fs = boost::filesystem;

struct fixture_once
{
  fixture_once()
  {
    BOOST_TEST_MESSAGE("test-core global setup");
    og::og_session cleaned_session_;

    const fs::path path(SRC_DBPATH); // コピー元
    const fs::path dest(DBPATH); // コピー先

    try
    {
      fs::copy_file(path, dest, fs::copy_option::overwrite_if_exists);
    }
    catch (fs::filesystem_error& ex)
    {
      std::cout << ex.what() << std::endl;
      throw;
    }

    cleaned_session_.open(DBPATH);
    cleaned_session_.build();
  }

  ~fixture_once() {}
};

