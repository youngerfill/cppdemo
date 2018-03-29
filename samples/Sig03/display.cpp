#include <iostream>


#include "timeevents.hpp"
#include "display.hpp"

using namespace std;


void Display::OnTime(TimeEvent& event)
{
  cout << "time == " << event.counter_ << endl;
}

void Display::OnMessage(MessageEvent& event)
{
  cout << event.message_ << endl;
}


