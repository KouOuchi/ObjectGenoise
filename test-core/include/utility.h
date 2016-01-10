#pragma once

#ifdef WINDOWS
#include <boost/date_time.hpp>
#include <crtdbg.h>
#include <boost/test/unit_test.hpp>

namespace og
{
namespace core
{
namespace pt = boost::posix_time;
using namespace boost;
using namespace std;

struct AvoidSomethingLeaks
{
  AvoidSomethingLeaks()
  {
    // time_from_string() leaks static
    pt::ptime t2(pt::time_from_string(pt::to_simple_string(
                                        pt::microsec_clock::local_time())));

  }
};

struct CrtCheckMemory
{
  AvoidSomethingLeaks l;

  _CrtMemState state1;
  _CrtMemState state2;
  _CrtMemState state3;
  CrtCheckMemory()
  {
    _CrtMemCheckpoint(&state1);
  }
  ~CrtCheckMemory()
  {
    _CrtMemCheckpoint(&state2);

    // else just do this to dump the leaked blocks to stdout.
    if( _CrtMemDifference( &state3, &state1, &state2) )
    {
      _CrtMemDumpStatistics( &state3 );
	  BOOST_FAIL("!!! Detected Memory Leak !!!");
    }
  }
};

}//namespace core
}//namespace og

#endif

