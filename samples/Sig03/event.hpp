#if !defined EVENT_HPP
#define EVENT_HPP

#include "boost/signal.hpp"
#include "boost/bind.hpp"


#define ENABLE_EVENT(evtType)\
  public:\
    boost::signal<void(evtType&)> sig_##evtType##_;\
    void SendEvent(evtType& evt){ sig_##evtType##_(evt); }  
  

#define ACCEPT_EVENT(pSigObject, evtType, slot)\
 (pSigObject)->sig_##evtType##_.connect(boost::bind(&slot, this, _1));


#define CONNECT_EVENT(pSigObject, evtType, pSlotObject, slot)\
 (pSigObject)->sig_##evtType##_.connect(boost::bind(&slot, pSlotObject, _1));


#endif /* EVENT_HPP */ 

