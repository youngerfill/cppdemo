
#include <stdlib.h>
#include <iostream>

#include "clock.hpp"

using namespace std;

Clock::Clock(unsigned long numIterations)
{
  counter_ = 0;
  numIterations_ = numIterations;
  
  theMessage_ = "Hey! The time is a multiple of 7.";
}

unsigned long Clock::GetCounter()
{
  return counter_;
}

void Clock::Go()
{
  TimeEvent timeEvent;
  MessageEvent messageEvent;
  messageEvent.message_ = theMessage_;

  for (counter_=1; counter_<=numIterations_; counter_++)
  {
    _sleep(1000);
    
    timeEvent.counter_ = counter_;
    SendEvent(timeEvent);
    
    if (!(counter_%7))
        SendEvent(messageEvent);
  }
}

