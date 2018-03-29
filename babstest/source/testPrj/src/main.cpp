#include <stdio.h>

#include "testfun1.h"
#include "testfun2.h"
#include "testfun3.hpp"

int main()
{
  printf("Hello from main()\n");

  testfun1();
  testfun2();
  testfun3();

  return 0;
}

