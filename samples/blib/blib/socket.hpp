/*! \file socket.hpp
** \date 22 mar 2003
** \author Bert Godderis
** \copyright (C) 2003 Bert Godderis
** \license wxWindows Library License : http://opensource.org/licenses/wxwindows
** 
** \brief Declares classes for socket communication
**  
**/

#if !defined SOCKET_HPP
#define SOCKET_HPP

#if defined WIN32
#pragma warning(disable:4786) 
#endif

#include <string>

#include "definitions.hpp"

namespace blib {

class ErrorStack;

enum
{
  SOCKET_ERR_NO_ERROR = 0,
  SOCKET_ERR_WINSOCK,
  SOCKET_ERR_WINSOCKSTARTUP,
  SOCKET_ERR_CLOSESOCKETHANDLE,
  SOCKET_ERR_READ,
  SOCKET_ERR_READ_ARGNULL,
  SOCKET_ERR_READ_NOTCONNECTED,
  SOCKET_ERR_WRITE,
  SOCKET_ERR_WRITE_ARGNULL,
  SOCKET_ERR_WRITE_NOTCONNECTED,
  SOCKET_ERR_READALL_NOTCONNECTED,
  SOCKET_ERR_READALL_ARGNULL,
  SOCKET_ERR_READALL_READ,
  SOCKET_ERR_WRITEALL_NOTCONNECTED,
  SOCKET_ERR_WRITEALL_ARGNULL,
  SOCKET_ERR_WRITEALL_WRITE,
  SOCKET_ERR_READMESSAGE_NOTCONNECTED,
  SOCKET_ERR_READMESSAGE_ARGNULL,
  SOCKET_ERR_READMESSAGE_READMSGLENGTH,
  SOCKET_ERR_READMESSAGE_BUFFERTOOSMALL,
  SOCKET_ERR_READMESSAGE_READMSGBODY,
  SOCKET_ERR_WRITEMESSAGE_NOTCONNECTED,
  SOCKET_ERR_WRITEMESSAGE_ARGNULL,
  SOCKET_ERR_WRITEMESSAGE_WRITEMSGLENGTH,
  SOCKET_ERR_WRITEMESSAGE_WRITEMSGBODY,
  SOCKET_ERR_WRITEMESSAGE_WRITEMESSAGE,
  SOCKET_ERR_CLOSE,
  SOCKET_ERR_CLOSE_NOTCONNECTED,
  SOCKET_ERR_GETINFO_NOTCONNECTED,
  SOCKET_ERR_LISTEN_CONNECTED,
  SOCKET_ERR_LISTEN_SOCKET,
  SOCKET_ERR_LISTEN_SETSOCKOPT_REUSE,
  SOCKET_ERR_LISTEN_BIND,
  SOCKET_ERR_LISTEN_LISTEN,
  SOCKET_ERR_LISTEN_ACCEPT,
  SOCKET_ERR_LISTEN_CLOSESOCKETHANDLE,
  SOCKET_ERR_LISTEN_RETRIEVEIPADDRESSES,
  SOCKET_ERR_CONNECT_SOCKET,
  SOCKET_ERR_CONNECT_CONNECT,
  SOCKET_ERR_CONNECT_RETRIEVEIPADDRESSES,
  SOCKET_ERR_RETRIEVEIPADDRESSES_GETSOCKNAME,
  SOCKET_ERR_RETRIEVEIPADDRESSES_GETPEERNAME,
};

ErrorStack* GetSocketErrorStack();


/* SocketBase */ 
class SocketBase
{
public:
  SocketBase(bool isBlocking=true);
  ~SocketBase();

  bool Read(char* buffer, int numBytes, int* numBytesRead);
  bool Write(const char* buffer, int numBytes, int* numBytesWritten);

  bool ReadAll(char* buffer, unsigned long numBytes);
  bool WriteAll(const char* buffer, unsigned long numBytes);

  bool ReadMessage(char* buffer, unsigned long bufferLength, unsigned long* messageLength);
  bool WriteMessage(const char* buffer, unsigned long messageLength);

  bool ReadMessage(std::string& message);
  bool WriteMessage(const std::string& message);

  bool Close();

  bool GetInfo(std::string* ipAddressLocal, std::string* ipAddressPeer, unsigned short* portNumber);
  bool IsConnected();
  bool IsBlocking();

protected:
  int socketHandle_;
  bool isBlocking_;
  bool isConnected_;
  std::string ipAddressLocal_;
  std::string ipAddressPeer_;
  unsigned short port_;
};


/* SocketServer */ 
class SocketServer : public SocketBase
{
public:
  SocketServer(bool isBlocking=true);
  bool Listen(unsigned short portNumber);
};


/* SocketClient */ 
class SocketClient : public SocketBase
{
public:
  SocketClient(bool isBlocking=true);
  bool Connect(const std::string& ipAddress, unsigned short portNumber);
};

} /* namespace blib  */ 

#endif /* SOCKET_HPP */ 
