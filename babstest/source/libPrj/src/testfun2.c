#include <stdio.h>

#include "testfun2.h"

void testfun2()
{
  printf("Hello from testfun2()\n");
#if defined ACTIVATE_FEATURE
  printf("Feature activated\n");
#endif

}

