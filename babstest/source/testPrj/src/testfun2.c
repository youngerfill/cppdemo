#include <iostream>

#include "testfun2.h"

using namespace std;

void testfun2()
{
  cout << "Hello from testfun2()" << endl;
#if defined ACTIVATE_FEATURE
  cout << "Feature activated" << endl;
#endif

}

