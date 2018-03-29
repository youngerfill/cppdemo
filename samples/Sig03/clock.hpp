#if !defined CLOCK_HPP
#define CLOCK_HPP

#include <string>


#include "event.hpp"
#include "timeevents.hpp"


class Clock
{

public:

  Clock(unsigned long numIterations=10);
  void Go();
  unsigned long GetCounter();
  
  ENABLE_EVENT(TimeEvent)
  ENABLE_EVENT(MessageEvent)

private:
  unsigned long counter_;
  unsigned long numIterations_;
  
  std::string theMessage_;
};

#endif /* CLOCK_HPP */ 


