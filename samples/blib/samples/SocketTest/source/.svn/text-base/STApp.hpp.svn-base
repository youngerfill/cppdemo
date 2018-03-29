/*! \file STApp.hpp
** \date 22 may 2003
** \author Bert Godderis
** 
** \brief Declares the application class for the SocketTest program
**  
**/

#if !defined STAPP_HPP
#define STAPP_HPP

#ifdef WIN32
#pragma warning(disable:4786) 
#endif

#include "app.hpp"

namespace blib {

class ErrorStack;

enum
{
  ST_ERR_NO_ERROR = 0,
  ST_ERR_BADARGUMENTS,
  ST_ERR_RUNSERVER_LISTEN,
  ST_ERR_RUNSERVER_GETINFO,
  ST_ERR_RUNSERVER_READMESSAGE,
  ST_ERR_RUNSERVER_CLOSESOCKET,
  ST_ERR_RUNCLIENT_CONNECT,
  ST_ERR_RUNCLIENT_GETINFO,
  ST_ERR_RUNCLIENT_WRITEMESSAGE,
  ST_ERR_RUNCLIENT_CLOSESOCKET,
};

/*  */ 
class STApp : public App
{
public:
  STApp();
  ~STApp();

protected:
	virtual bool Enter();
	virtual bool Run();
	virtual int Leave();

  ErrorStack* errorStack_;
  void ShowHelp();
  bool RunServer();
  bool RunClient();

  void ProcessMessage(const std::string& message, bool& stopSession, bool& stopServer);

  bool helpShown_;
  bool isServer_;

  std::string ipAddress_;
  unsigned short portNumber_;
};

} /* namespace blib */ 

#endif /* STAPP_HPP */ 
