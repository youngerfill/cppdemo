#include <stdio.h>
#include <stdlib.h>

#include "ant/ant.h"
#include "bee/bee.h"
#include "cat/cat.h"
#include "mouse/mouse.h"

#include "math/math.h"

int main()
{
  printf("Hello from main()\n");

  ant();
  bee();
  cat();
  mouse();

  doMath2();

  return 0;
}

