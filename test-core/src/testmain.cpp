#include "gtest/gtest.h"

//__declspec(dllexport) int PullInMyLibrary() { return 0; }
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  // from gtest FAQ.
  //int PullInMyLibrary();
  //static int dummy = PullInMyLibrary();


  int result = RUN_ALL_TESTS();
  return result;
}
