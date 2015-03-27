#include "fixtures.h"
#include "utility.h"

#ifdef TEST_OG_BASIC

// test boost posix_time
BOOST_AUTO_TEST_CASE( basic_001 )
{
#ifdef WINDOWS
  og::core::CrtCheckMemory check;
#endif

  {
    pt::ptime current_date_microseconds = pt::microsec_clock::local_time();
    string t(pt::to_simple_string(current_date_microseconds));
    //std::cout << t << std::endl;

    pt::ptime t2(pt::time_from_string(t));

    BOOST_REQUIRE(t2 == current_date_microseconds);
  }
}

/*
BOOST_AUTO_TEST_CASE( basic_002 )
{
#ifdef WINDOWS
  CrtCheckMemory check;
#endif

  int *i = new int();
  //delete i;
}
*/
#endif


