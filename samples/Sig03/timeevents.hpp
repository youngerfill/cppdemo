#if !defined TIMEEVENTS_HPP
#define TIMEEVENTS_HPP

#include <string>

struct TimeEvent
{
    unsigned long counter_;
};


struct MessageEvent
{
    std::string message_;
};

#endif /* TIMEEVENTS_HPP */

