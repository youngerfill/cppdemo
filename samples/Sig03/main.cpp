#include <iostream>
#include <stdlib.h>

#include "clock.hpp"
#include "display.hpp"

using namespace std;

int main(int argc, char *argv[])
{
  Clock theClock(100);
  Display theDisplay;

  CONNECT_EVENT(&theClock, TimeEvent, &theDisplay, Display::OnTime)
  CONNECT_EVENT(&theClock, MessageEvent, &theDisplay, Display::OnMessage)

  theClock.Go();

  system("PAUSE");
  return 0;
}
