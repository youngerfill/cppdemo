#if !defined DISPLAY_HPP
#define DISPLAY_HPP

class TimeEvent;
class MessageEvent;

class Display
{
public:
  void OnTime(TimeEvent& event);
  void OnMessage(MessageEvent& event);
};

#endif /* DISPLAY_HPP */ 


