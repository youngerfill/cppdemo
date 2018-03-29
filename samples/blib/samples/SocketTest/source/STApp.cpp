/*! \file STApp.cpp
** \date 22 may 2003
** \author Bert Godderis
** 
** \brief Implements the application class for the SocketTest program
**  
**/

#ifdef WIN32
#pragma warning(disable:4786) 
#endif

#include <iostream>

#include "error.hpp"
#include "socket.hpp"
#include "STApp.hpp"

using namespace blib;
using namespace std;

/*  */ 
STApp::STApp()
{
  errorStack_ = new ErrorStack;
  errorStack_->DefineError(ST_ERR_NO_ERROR, "STApp : No error occurred (ST_ERR_NO_ERROR)");
  errorStack_->DefineError(ST_ERR_BADARGUMENTS, "STApp : Bad arguments. Help shown. (ST_ERR_BADARGUMENTS)");
  errorStack_->DefineError(ST_ERR_RUNSERVER_LISTEN, "STApp::RunServer() : Error while listening for incoming connection (ST_ERR_RUNSERVER_LISTEN)");
  errorStack_->DefineError(ST_ERR_RUNSERVER_GETINFO, "STApp::RunServer() : Error while retrieving info about connection (ST_ERR_RUNSERVER_GETINFO)");
  errorStack_->DefineError(ST_ERR_RUNSERVER_READMESSAGE, "STApp::RunServer() : Error while reading message from client (ST_ERR_RUNSERVER_READMESSAGE)");
  errorStack_->DefineError(ST_ERR_RUNSERVER_CLOSESOCKET, "STApp::RunServer() : Error while closing socket (ST_ERR_RUNSERVER_CLOSESOCKET)");
  errorStack_->DefineError(ST_ERR_RUNCLIENT_CONNECT, "STApp::RunClient() : Error while connecting to server (ST_ERR_RUNCLIENT_CONNECT)");
  errorStack_->DefineError(ST_ERR_RUNCLIENT_GETINFO, "STApp::RunClient() : Error while retrieving info about connection (ST_ERR_RUNCLIENT_GETINFO)");
  errorStack_->DefineError(ST_ERR_RUNCLIENT_WRITEMESSAGE, "STApp::RunClient() : Error while writing message (ST_ERR_RUNCLIENT_WRITEMESSAGE)");
  errorStack_->DefineError(ST_ERR_RUNCLIENT_CLOSESOCKET, "STApp::RunClient() : Error while closing socket (ST_ERR_RUNCLIENT_CLOSESOCKET)");

  helpShown_ = false;
  isServer_ = false;
}

/*  */ 
STApp::~STApp()
{
  delete errorStack_;
}

/*  */ 
bool STApp::Enter()
{
  if (arguments_.size() >= 3)
  {
    if (arguments_[1] == "server")
      isServer_ = true;
    else
      ipAddress_ = arguments_[1];
    portNumber_ = (unsigned short)atol(arguments_[2].c_str());
    return true;
  }

  errorStack_->PushError(ST_ERR_BADARGUMENTS);
  ShowHelp();
  return false;
}

/*  */ 
bool STApp::Run()
{
  if (isServer_)
    return RunServer();
  else
    return RunClient();
}

/*  */ 
int STApp::Leave()
{
  int numErrors = errorStack_->GetNumErrorCodes();
  if (numErrors)
  {
    if (!helpShown_)
    {
      cout << "An error has occurred:" << endl << endl;
      errorStack_->PrintErrors(cout);
    }
    return errorStack_->GetErrorCode(numErrors-1);
  }

  return 0;
}

/*  */ 
void STApp::ShowHelp()
{
  cout << "SocketTest" << endl << "Bert Godderis" << endl << endl;
  cout << "Usage:" << endl;
  cout << "st server PORT" << endl;
  cout << "or:" << endl;
  cout << "st ADDRESS PORT" << endl << endl;
  cout << "'PORT' is a port number, e.g. 2000" << endl;
  cout << "'ADDRESS' is an IP number, e.g. 192.168.1.1" << endl << endl;
  cout << "The first form starts an st-server listening on PORT." << endl;
  cout << "The second form starts an st-client which will connect to" << endl;
  cout << "an st-server at ADDRESS listening on PORT." << endl;
  cout << "When the connection is made, the client shows a command prompt" << endl;
  cout << "and the user can type messages to be sent to the server." << endl;
  cout << "The message 'exit' will end the session. The client exits" << endl;
  cout << "and the server will wait for another incoming connection." << endl;
  cout << "The message 'stop' will stop the server." << endl;

  helpShown_ = true;
}

/*  */ 
bool STApp::RunServer()
{
  SocketServer socket;

  string theMessage;

  bool stopSession = false;
  bool stopServer = false;

  do /* while (!stopServer) */ 
  {
    stopSession = false;
    stopServer = false;

    cout << "Waiting for incoming connection on port " << portNumber_ << " ... ";

    if (!socket.Listen(portNumber_))
    {
      cout << "ERROR" << endl;
      errorStack_->Prepend(GetSocketErrorStack());
      return errorStack_->PushError(ST_ERR_RUNSERVER_LISTEN);
    }

    cout << "connected!" << endl;

    string localIP, peerIP;
    unsigned short port;
    if (!socket.GetInfo(&localIP, &peerIP, &port))
    {
      errorStack_->Prepend(GetSocketErrorStack());
      return errorStack_->PushError(ST_ERR_RUNSERVER_GETINFO);
    }

    cout << "Local IP: " << localIP << endl;
    cout << "Peer IP: " << peerIP << endl;
    cout << "Port #: " << port << endl;

    do /* while (!stopSession) */ 
    {
      cout << "Waiting for message from client ... ";
    
      if (!socket.ReadMessage(theMessage))
      {
        cout << "ERROR" << endl;
        errorStack_->Prepend(GetSocketErrorStack());
        return errorStack_->PushError(ST_ERR_RUNSERVER_READMESSAGE);
      }

      cout << "done." << endl;

      cout << "Message received from client: '" << theMessage << "'" << endl;

      ProcessMessage(theMessage, stopSession, stopServer);

    } while (!stopSession);

    cout << "Closing connection ... ";
    if (!socket.Close())
    {
      cout << "ERROR" << endl;
      errorStack_->Prepend(GetSocketErrorStack());
      return errorStack_->PushError(ST_ERR_RUNSERVER_CLOSESOCKET);
    }
    cout << "done." << endl;

  } while (!stopServer);

  cout << "Server stopped." << endl;
  return true;
}

/*  */ 
bool STApp::RunClient()
{
  SocketClient socket;
  char buffer[200];
  string theMessage;

  bool stopSession = false;
  bool stopServer = false;

  cout << "Connecting to " << ipAddress_ << " on port " << portNumber_ << " ... ";
  if (!socket.Connect(ipAddress_, portNumber_))
  {
    cout << "ERROR" << endl;
    errorStack_->Prepend(GetSocketErrorStack());
    return errorStack_->PushError(ST_ERR_RUNCLIENT_CONNECT);
    socket.Close();
  }
  cout << "connected!" << endl;

  string localIP, peerIP;
  unsigned short port;
  if (!socket.GetInfo(&localIP, &peerIP, &port))
  {
    errorStack_->Prepend(GetSocketErrorStack());
    return errorStack_->PushError(ST_ERR_RUNCLIENT_GETINFO);
  }

  cout << "Local IP: " << localIP << endl;
  cout << "Peer IP: " << peerIP << endl;
  cout << "Port #: " << port << endl;

  do /* while (!stopClient) */ 
  {
    cout << ">";
    cin.getline(buffer, 200);
    theMessage = buffer;

    ProcessMessage(buffer, stopSession, stopServer);

    cout << "Sending message '" << theMessage << "' to server ... ";
    if (!socket.WriteMessage(theMessage))
    {
      cout << "ERROR" << endl;
      errorStack_->Prepend(GetSocketErrorStack());
      return errorStack_->PushError(ST_ERR_RUNCLIENT_WRITEMESSAGE);
    }
    cout << "done." << endl;
  } while ( (!stopSession) && (!stopServer) );

  cout << "Closing socket ... ";
  if (!socket.Close())
  {
    cout << "ERROR" << endl;
    errorStack_->Prepend(GetSocketErrorStack());
    return errorStack_->PushError(ST_ERR_RUNCLIENT_CLOSESOCKET);
  }
  cout << "done." << endl;

  return true;
}

/*  */ 
void STApp::ProcessMessage(const string& message, bool& stopSession, bool& stopServer)
{
  if (message == "exit")
  {
    stopSession = true;
    return;
  }

  if (message == "stop")
  {
    stopSession = true;
    stopServer = true;
    return;
  }
}

/*  */ 
BLIB_MAIN(STApp)
