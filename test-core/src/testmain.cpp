#define BOOST_TEST_MODULE testmain

#include "fixtures.h"

struct TestCoreConfig
{
  TestCoreConfig()
  {
	  fixture_once();
  }
  ~TestCoreConfig()
  {

#ifdef _WINDOWS
	  ::system("pause");
#endif

  }
};

BOOST_GLOBAL_FIXTURE( TestCoreConfig );

