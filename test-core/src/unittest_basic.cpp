#include "fixtures.h"
#include "utility.h"
#include <gtest/gtest.h>

// test boost posix_time
TEST(basic, one)
{
#ifdef _WINDOWS
  og::core::CrtCheckMemory check;
#endif

  {
    pt::ptime current_date_microseconds = pt::microsec_clock::local_time();
    string t(pt::to_simple_string(current_date_microseconds));
    //std::cout << t << std::endl;

    pt::ptime t2(pt::time_from_string(t));

    EXPECT_TRUE(t2 == current_date_microseconds);
  }
}

/*
BOOST_AUTO_TEST_CASE( basic_002 )
{
#ifdef _WINDOWS
  CrtCheckMemory check;
#endif

  int *i = new int();
  //delete i;
}
*/



