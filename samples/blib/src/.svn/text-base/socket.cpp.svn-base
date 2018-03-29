/*! \file socket.cpp
** \date 22 may 2003
** \author Bert Godderis
** \copyright (C) 2003 Bert Godderis
** \license wxWindows Library License : http://opensource.org/licenses/wxwindows
** 
** \brief Implements classes for socket communication
**  
**/

#if !defined SOCKET_CPP
#define SOCKET_CPP

#if defined WIN32
#pragma warning(disable:4786) 
#include <winsock2.h>
#include <ws2tcpip.h>
#define socklen_t int
#else
#include <cstdio>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#endif

#include <sstream>
#include <string>

#include "error.hpp"
#include "socket.hpp"

using namespace std;
using namespace blib;

#define BLIB_ULONGSIZE sizeof(unsigned long)

static ErrorStack _socketErrorStack_;

/* Define socket.cpp error strings */ 
BLIB_GLOBALINIT_BEGIN(SOCKET_CPP_SOCKETERRORSTACK)
  _socketErrorStack_.DefineError(SOCKET_ERR_NO_ERROR, "socket.cpp: No error occurred. (SOCKET_ERR_NO_ERROR)");
  _socketErrorStack_.DefineError(SOCKET_ERR_WINSOCK, "socket.cpp: A WinSock error occurred. (SOCKET_ERR_WINSOCK)");
  _socketErrorStack_.DefineError(SOCKET_ERR_WINSOCKSTARTUP, "socket.cpp: Error while initialising WinSock library (SOCKET_ERR_WINSOCKSTARTUP)");
  _socketErrorStack_.DefineError(SOCKET_ERR_CLOSESOCKETHANDLE, "socket.cpp: CloseSocketHandle() : Could not close socket handle (SOCKET_ERR_CLOSESOCKETHANDLE)");
  _socketErrorStack_.DefineError(SOCKET_ERR_READ, "SocketBase::Read(): Error while reading from socket with recv() (SOCKET_ERR_READ)");
  _socketErrorStack_.DefineError(SOCKET_ERR_READ_ARGNULL, "SocketBase::Read(): No null-arguments allowed (SOCKET_ERR_READ_ARGNULL)");
  _socketErrorStack_.DefineError(SOCKET_ERR_READ_NOTCONNECTED, "SocketBase::Read(): The socket is not connected (SOCKET_ERR_READ_NOTCONNECTED)");
  _socketErrorStack_.DefineError(SOCKET_ERR_WRITE, "SocketBase::Write(): Error while writing to socket with send() (SOCKET_ERR_WRITE)");
  _socketErrorStack_.DefineError(SOCKET_ERR_WRITE_ARGNULL, "SocketBase::Write(): No null-arguments allowed (SOCKET_ERR_WRITE_ARGNULL)");
  _socketErrorStack_.DefineError(SOCKET_ERR_WRITE_NOTCONNECTED, "SocketBase::Write(): The socket is not connected (SOCKET_ERR_WRITE_NOTCONNECTED)");
  _socketErrorStack_.DefineError(SOCKET_ERR_READALL_NOTCONNECTED, "SocketBase::ReadAll(): The socket is not connected (SOCKET_ERR_READALL_NOTCONNECTED)");
  _socketErrorStack_.DefineError(SOCKET_ERR_READALL_ARGNULL, "SocketBase::ReadAll(): No null-arguments allowed (SOCKET_ERR_READALL_ARGNULL)");
  _socketErrorStack_.DefineError(SOCKET_ERR_READALL_READ, "SocketBase::ReadAll(): Error while reading from socket with SocketBase::Read() (SOCKET_ERR_READALL_READ)");
  _socketErrorStack_.DefineError(SOCKET_ERR_WRITEALL_NOTCONNECTED, "SocketBase::WriteAll(): The socket is not connected (SOCKET_ERR_WRITEALL_NOTCONNECTED)");
  _socketErrorStack_.DefineError(SOCKET_ERR_WRITEALL_ARGNULL, "SocketBase::WriteAll(): No null-arguments allowed (SOCKET_ERR_WRITEALL_ARGNULL)");
  _socketErrorStack_.DefineError(SOCKET_ERR_WRITEALL_WRITE, "SocketBase::WriteAll(): Error while writing to socket with SocketBase::Write() (SOCKET_ERR_WRITEALL_WRITE)");
  _socketErrorStack_.DefineError(SOCKET_ERR_READMESSAGE_NOTCONNECTED, "SocketBase::ReadMessage(): The socket is not connected (SOCKET_ERR_READMESSAGE_NOTCONNECTED)");
  _socketErrorStack_.DefineError(SOCKET_ERR_READMESSAGE_ARGNULL, "SocketBase::ReadMessage(): No null-arguments allowed (SOCKET_ERR_READMESSAGE_ARGNULL)");
  _socketErrorStack_.DefineError(SOCKET_ERR_READMESSAGE_READMSGLENGTH, "SocketBase::ReadMessage(): Error while reading message length (SOCKET_ERR_READMESSAGE_READMSGLENGTH)");
  _socketErrorStack_.DefineError(SOCKET_ERR_READMESSAGE_BUFFERTOOSMALL, "SocketBase::ReadMessage(): The read buffer given in the argument is too small (SOCKET_ERR_READMESSAGE_BUFFERTOOSMALL)");
  _socketErrorStack_.DefineError(SOCKET_ERR_READMESSAGE_READMSGBODY, "SocketBase::ReadMessage(): Error while reading the message body (SOCKET_ERR_READMESSAGE_READMSGBODY)");
  _socketErrorStack_.DefineError(SOCKET_ERR_WRITEMESSAGE_NOTCONNECTED, "SocketBase::WriteMessage(): The socket is not connected (SOCKET_ERR_WRITEMESSAGE_NOTCONNECTED)");
  _socketErrorStack_.DefineError(SOCKET_ERR_WRITEMESSAGE_ARGNULL, "SocketBase::WriteMessage(): No null-arguments allowed (SOCKET_ERR_WRITEMESSAGE_ARGNULL)");
  _socketErrorStack_.DefineError(SOCKET_ERR_WRITEMESSAGE_WRITEMSGLENGTH, "SocketBase::WriteMessage(): Error while writing message length (SOCKET_ERR_WRITEMESSAGE_WRITEMSGLENGTH)");
  _socketErrorStack_.DefineError(SOCKET_ERR_WRITEMESSAGE_WRITEMSGBODY, "SocketBase::WriteMessage(): Error while writing message body (SOCKET_ERR_WRITEMESSAGE_WRITEMSGBODY)");
  _socketErrorStack_.DefineError(SOCKET_ERR_WRITEMESSAGE_WRITEMESSAGE, "SocketBase::WriteMessage(): Error while writing message (SOCKET_ERR_WRITEMESSAGE_WRITEMESSAGE)");
  _socketErrorStack_.DefineError(SOCKET_ERR_CLOSE, "SocketBase::Close(): Error closing socket(SOCKET_ERR_CLOSE)");
  _socketErrorStack_.DefineError(SOCKET_ERR_CLOSE_NOTCONNECTED, "SocketBase::Close(): The socket is not connected (SOCKET_ERR_CLOSE_NOTCONNECTED)");
  _socketErrorStack_.DefineError(SOCKET_ERR_GETINFO_NOTCONNECTED, "SocketBase::GetInfo(): The socket is not connected (SOCKET_ERR_GETINFO_NOTCONNECTED)");
  _socketErrorStack_.DefineError(SOCKET_ERR_LISTEN_CONNECTED, "SocketBase::Listen(): The socket is already connected (SOCKET_ERR_LISTEN_CONNECTED)");
  _socketErrorStack_.DefineError(SOCKET_ERR_LISTEN_SOCKET, "SocketBase::Listen(): Error while calling socket() (SOCKET_ERR_LISTEN_SOCKET)");
  _socketErrorStack_.DefineError(SOCKET_ERR_LISTEN_SETSOCKOPT_REUSE, "SocketBase::Listen(): Error while setting options for socket reuse (SOCKET_ERR_LISTEN_SETSOCKOPT_REUSE)");
  _socketErrorStack_.DefineError(SOCKET_ERR_LISTEN_BIND, "SocketBase::Listen(): Error while calling bind() (SOCKET_ERR_LISTEN_BIND)");
  _socketErrorStack_.DefineError(SOCKET_ERR_LISTEN_LISTEN, "SocketBase::Listen(): Error while calling listen() (SOCKET_ERR_LISTEN_LISTEN)");
  _socketErrorStack_.DefineError(SOCKET_ERR_LISTEN_ACCEPT, "SocketBase::Listen(): Error while calling accept() (SOCKET_ERR_LISTEN_ACCEPT)");
  _socketErrorStack_.DefineError(SOCKET_ERR_LISTEN_CLOSESOCKETHANDLE, "SocketBase::Listen(): Error while closing temporary socket handle (SOCKET_ERR_LISTEN_CLOSESOCKETHANDLE)");
  _socketErrorStack_.DefineError(SOCKET_ERR_LISTEN_RETRIEVEIPADDRESSES, "SocketBase::Listen(): Error while retrieving IP addresses (SOCKET_ERR_LISTEN_RETRIEVEIPADDRESSES)");
  _socketErrorStack_.DefineError(SOCKET_ERR_CONNECT_SOCKET, "SocketBase::Connect(): Error while calling socket() (SOCKET_ERR_CONNECT_SOCKET)");
  _socketErrorStack_.DefineError(SOCKET_ERR_CONNECT_CONNECT, "SocketBase::Connect(): Error while calling connect() (SOCKET_ERR_CONNECT_CONNECT)");
  _socketErrorStack_.DefineError(SOCKET_ERR_LISTEN_RETRIEVEIPADDRESSES, "SocketBase::Connect(): Error while retrieving IP addresses (SOCKET_ERR_CONNECT_RETRIEVEIPADDRESSES)");
  _socketErrorStack_.DefineError(SOCKET_ERR_RETRIEVEIPADDRESSES_GETSOCKNAME, "socket.cpp: RetrieveIPAddresses(): Error while calling getsockname() (SOCKET_ERR_RETRIEVEIPADDRESSES_GETSOCKNAME)");
  _socketErrorStack_.DefineError(SOCKET_ERR_RETRIEVEIPADDRESSES_GETPEERNAME, "socket.cpp: RetrieveIPAddresses(): Error while calling getpeername() (SOCKET_ERR_RETRIEVEIPADDRESSES_GETPEERNAME)");
BLIB_GLOBALINIT_END(SOCKET_CPP_SOCKETERRORSTACK)


#if defined WIN32

/* Define Winsock error strings */ 
static IntStringMap _winsockErrorMap_;
BLIB_GLOBALINIT_BEGIN(SOCKET_CPP_WINSOCKERRORMAP)
  _winsockErrorMap_[WSAEINTR] = "WSAEINTR";
  _winsockErrorMap_[WSAEBADF] = "WSAEBADF";
  _winsockErrorMap_[WSAEACCES] = "WSAEACCES";
  _winsockErrorMap_[WSAEFAULT] = "WSAEFAULT";
  _winsockErrorMap_[WSAEINVAL] = "WSAEINVAL";
  _winsockErrorMap_[WSAEMFILE] = "WSAEMFILE";
  _winsockErrorMap_[WSAEWOULDBLOCK] = "WSAEWOULDBLOCK";
  _winsockErrorMap_[WSAEINPROGRESS] = "WSAEINPROGRESS";
  _winsockErrorMap_[WSAEALREADY] = "WSAEALREADY";
  _winsockErrorMap_[WSAENOTSOCK] = "WSAENOTSOCK";
  _winsockErrorMap_[WSAEDESTADDRREQ] = "WSAEDESTADDRREQ";
  _winsockErrorMap_[WSAEMSGSIZE] = "WSAEMSGSIZE";
  _winsockErrorMap_[WSAEPROTOTYPE] = "WSAEPROTOTYPE";
  _winsockErrorMap_[WSAENOPROTOOPT] = "WSAENOPROTOOPT";
  _winsockErrorMap_[WSAEPROTONOSUPPORT] = "WSAEPROTONOSUPPORT";
  _winsockErrorMap_[WSAESOCKTNOSUPPORT] = "WSAESOCKTNOSUPPORT";
  _winsockErrorMap_[WSAEOPNOTSUPP] = "WSAEOPNOTSUPP";
  _winsockErrorMap_[WSAEPFNOSUPPORT] = "WSAEPFNOSUPPORT";
  _winsockErrorMap_[WSAEAFNOSUPPORT] = "WSAEAFNOSUPPORT";
  _winsockErrorMap_[WSAEADDRINUSE] = "WSAEADDRINUSE";
  _winsockErrorMap_[WSAEADDRNOTAVAIL] = "WSAEADDRNOTAVAIL";
  _winsockErrorMap_[WSAENETDOWN] = "WSAENETDOWN";
  _winsockErrorMap_[WSAENETUNREACH] = "WSAENETUNREACH";
  _winsockErrorMap_[WSAENETRESET] = "WSAENETRESET";
  _winsockErrorMap_[WSAECONNABORTED] = "WSAECONNABORTED";
  _winsockErrorMap_[WSAECONNRESET] = "WSAECONNRESET";
  _winsockErrorMap_[WSAENOBUFS] = "WSAENOBUFS";
  _winsockErrorMap_[WSAEISCONN] = "WSAEISCONN";
  _winsockErrorMap_[WSAENOTCONN] = "WSAENOTCONN";
  _winsockErrorMap_[WSAESHUTDOWN] = "WSAESHUTDOWN";
  _winsockErrorMap_[WSAETOOMANYREFS] = "WSAETOOMANYREFS";
  _winsockErrorMap_[WSAETIMEDOUT] = "WSAETIMEDOUT";
  _winsockErrorMap_[WSAECONNREFUSED] = "WSAECONNREFUSED";
  _winsockErrorMap_[WSAELOOP] = "WSAELOOP";
  _winsockErrorMap_[WSAENAMETOOLONG] = "WSAENAMETOOLONG";
  _winsockErrorMap_[WSAEHOSTDOWN] = "WSAEHOSTDOWN";
  _winsockErrorMap_[WSAEHOSTUNREACH] = "WSAEHOSTUNREACH";
  _winsockErrorMap_[WSAENOTEMPTY] = "WSAENOTEMPTY";
  _winsockErrorMap_[WSAEPROCLIM] = "WSAEPROCLIM";
  _winsockErrorMap_[WSAEUSERS] = "WSAEUSERS";
  _winsockErrorMap_[WSAEDQUOT] = "WSAEDQUOT";
  _winsockErrorMap_[WSAESTALE] = "WSAESTALE";
  _winsockErrorMap_[WSAEREMOTE] = "WSAEREMOTE";
  _winsockErrorMap_[WSASYSNOTREADY] = "WSASYSNOTREADY";
  _winsockErrorMap_[WSAVERNOTSUPPORTED] = "WSAVERNOTSUPPORTED";
  _winsockErrorMap_[WSANOTINITIALISED] = "WSANOTINITIALISED";
  _winsockErrorMap_[WSAEDISCON] = "WSAEDISCON";
  _winsockErrorMap_[WSAENOMORE] = "WSAENOMORE";
  _winsockErrorMap_[WSAECANCELLED] = "WSAECANCELLED";
  _winsockErrorMap_[WSAEINVALIDPROCTABLE] = "WSAEINVALIDPROCTABLE";
  _winsockErrorMap_[WSAEINVALIDPROVIDER] = "WSAEINVALIDPROVIDER";
  _winsockErrorMap_[WSAEPROVIDERFAILEDINIT] = "WSAEPROVIDERFAILEDINIT";
  _winsockErrorMap_[WSASYSCALLFAILURE] = "WSASYSCALLFAILURE";
  _winsockErrorMap_[WSASERVICE_NOT_FOUND] = "WSASERVICE_NOT_FOUND";
  _winsockErrorMap_[WSATYPE_NOT_FOUND] = "WSATYPE_NOT_FOUND";
  _winsockErrorMap_[WSA_E_NO_MORE] = "WSA_E_NO_MORE";
  _winsockErrorMap_[WSA_E_CANCELLED] = "WSA_E_CANCELLED";
  _winsockErrorMap_[WSAEREFUSED] = "WSAEREFUSED";
  _winsockErrorMap_[WSAHOST_NOT_FOUND] = "WSAHOST_NOT_FOUND";
  _winsockErrorMap_[WSATRY_AGAIN] = "WSATRY_AGAIN";
  _winsockErrorMap_[WSANO_RECOVERY] = "WSANO_RECOVERY";
  _winsockErrorMap_[WSANO_DATA] = "WSANO_DATA";
  _winsockErrorMap_[WSANO_ADDRESS] = "WSANO_ADDRESS";
  _winsockErrorMap_[WSA_QOS_RECEIVERS] = "WSA_QOS_RECEIVERS";
  _winsockErrorMap_[WSA_QOS_SENDERS] = "WSA_QOS_SENDERS";
  _winsockErrorMap_[WSA_QOS_NO_SENDERS] = "WSA_QOS_NO_SENDERS ";
  _winsockErrorMap_[WSA_QOS_NO_RECEIVERS] = "WSA_QOS_NO_RECEIVERS";
  _winsockErrorMap_[WSA_QOS_REQUEST_CONFIRMED] = "WSA_QOS_REQUEST_CONFIRMED";
  _winsockErrorMap_[WSA_QOS_ADMISSION_FAILURE] = "WSA_QOS_ADMISSION_FAILURE";
  _winsockErrorMap_[WSA_QOS_POLICY_FAILURE] = "WSA_QOS_POLICY_FAILURE";
  _winsockErrorMap_[WSA_QOS_BAD_STYLE] = "WSA_QOS_BAD_STYLE";
  _winsockErrorMap_[WSA_QOS_BAD_OBJECT] = "WSA_QOS_BAD_OBJECT";
  _winsockErrorMap_[WSA_QOS_TRAFFIC_CTRL_ERROR] = "WSA_QOS_TRAFFIC_CTRL_ERROR";
  _winsockErrorMap_[WSA_QOS_GENERIC_ERROR] = "WSA_QOS_GENERIC_ERROR";
BLIB_GLOBALINIT_END(SOCKET_CPP_WINSOCKERRORMAP)


/* Initialise Winsock library on startup */ 
BLIB_GLOBALINIT_BEGIN(SOCKET_CPP_WINSOCKINIT)

  WSADATA wsaData;
  if (WSAStartup(MAKEWORD(2, 2), &wsaData))
    _socketErrorStack_.RootError(SOCKET_ERR_WINSOCKSTARTUP);

BLIB_GLOBALINIT_END(SOCKET_CPP_WINSOCKINIT)



/* Cleanup Winsock library on exit */ 
BLIB_GLOBALEXIT_BEGIN(SOCKET_CPP_WINSOCKEXIT)

  WSACleanup();

BLIB_GLOBALEXIT_END(SOCKET_CPP_WINSOCKEXIT)

#endif /* WIN32 */ 


/*  */ 
ErrorStack* blib::GetSocketErrorStack()
{
  return &_socketErrorStack_;
}

/*  */ 
void OnSocketSystemError()
{
#if defined WIN32
  ostringstream auxStream;
  auxStream << "socket.cpp: The following WinSock error has occurred: " << _winsockErrorMap_[WSAGetLastError()];
  _socketErrorStack_.RootError(SOCKET_ERR_WINSOCK, auxStream.str());
#else
  _socketErrorStack_.RootSystemError();
#endif
}

/*  */ 
string RetrieveIPAddress(struct sockaddr_in& addressInfo)
{
  char ipAddress[20];

  sprintf(ipAddress, "%u.%u.%u.%u",
    (unsigned char)( ( (int)addressInfo.sin_addr.s_addr       ) % 0x100),
    (unsigned char)( ( (int)addressInfo.sin_addr.s_addr >>  8 ) % 0x100),
    (unsigned char)( ( (int)addressInfo.sin_addr.s_addr >> 16 ) % 0x100),
    (unsigned char)( ( (int)addressInfo.sin_addr.s_addr >> 24 ) % 0x100)
  );

  return ipAddress;
}

/*  */ 
bool RetrieveIPAddresses(int socketHandle, string* ipAddressLocal, string* ipAddressPeer)
{
  struct sockaddr_in addressInfo;
  socklen_t addressInfoLength;

  if (ipAddressLocal)
  {
    addressInfo.sin_family = AF_INET;
    addressInfo.sin_port = 0;
    addressInfo.sin_addr.s_addr = INADDR_ANY;
    memset(&addressInfo.sin_zero, 0, 8);
    addressInfoLength = sizeof(addressInfo);

    /* Get local IP address */ 
    if ( getsockname(socketHandle, (struct sockaddr*)&addressInfo, &addressInfoLength) == SOCKET_ERROR )
    {
      OnSocketSystemError();
      return _socketErrorStack_.PushError(SOCKET_ERR_RETRIEVEIPADDRESSES_GETSOCKNAME);
    }
    *ipAddressLocal = RetrieveIPAddress(addressInfo);
  }
  
  if (ipAddressPeer)
  {
    addressInfo.sin_family = AF_INET;
    addressInfo.sin_port = 0;
    addressInfo.sin_addr.s_addr = INADDR_ANY;
    memset(&addressInfo.sin_zero, 0, 8);
    addressInfoLength = sizeof(addressInfo);

    /* Get peer IP address */ 
    if ( getpeername(socketHandle, (struct sockaddr*)&addressInfo, &addressInfoLength) == SOCKET_ERROR )
    {
      OnSocketSystemError();
      return _socketErrorStack_.PushError(SOCKET_ERR_RETRIEVEIPADDRESSES_GETPEERNAME);
    }
    *ipAddressPeer = RetrieveIPAddress(addressInfo);
  }

  return true;
}

/*  */ 
bool CloseSocketHandle(int socketHandle)
{
#if defined WIN32
    if ( closesocket(socketHandle) == SOCKET_ERROR ) 
#else 
    if ( close(socketHandle) == SOCKET_ERROR ) 
#endif
    {
      OnSocketSystemError();
      return _socketErrorStack_.PushError(SOCKET_ERR_CLOSESOCKETHANDLE);
    }

  return true;
}

/**************/ 
/* SocketBase */ 
/**************/ 
SocketBase::SocketBase(bool isBlocking)
{
  isBlocking_ = isBlocking;

  socketHandle_ = 0;
  isConnected_ = false;
  ipAddressLocal_ = "undefined";
  ipAddressPeer_ = "undefined";
  port_ = 0;
}

/*  */ 
SocketBase::~SocketBase()
{
  if (isConnected_)
    Close();
}

/*  */ 
bool SocketBase::Read(char* buffer, int numBytes, int* numBytesRead)
{
  if (!isConnected_)
    return _socketErrorStack_.RootError(SOCKET_ERR_READ_NOTCONNECTED);

  if ( (!buffer) || (!numBytesRead) )
    return _socketErrorStack_.RootError(SOCKET_ERR_READ_ARGNULL);

  int numberOfBytesRead = recv(socketHandle_, buffer, numBytes, 0);
  if (numberOfBytesRead==SOCKET_ERROR)
  {
    OnSocketSystemError();
    return _socketErrorStack_.PushError(SOCKET_ERR_READ);
  }

  *numBytesRead = numberOfBytesRead;
  return true;
}

/*  */ 
bool SocketBase::Write(const char* buffer, int numBytes, int* numBytesWritten)
{
  if (!isConnected_)
    return _socketErrorStack_.RootError(SOCKET_ERR_WRITE_NOTCONNECTED);

  if ( (!buffer) || (!numBytesWritten) )
    return _socketErrorStack_.RootError(SOCKET_ERR_WRITE_ARGNULL);

  int numberOfBytesWritten = send(socketHandle_, buffer, numBytes, 0);
  if (numberOfBytesWritten==SOCKET_ERROR)
  {
    OnSocketSystemError();
    return _socketErrorStack_.PushError(SOCKET_ERR_WRITE);
  }

  *numBytesWritten = numberOfBytesWritten;
  return true;
}

/*  */ 
bool SocketBase::ReadAll(char* buffer, unsigned long numBytes)
{
  if (!isConnected_)
    return _socketErrorStack_.RootError(SOCKET_ERR_READALL_NOTCONNECTED);

  if (!buffer)
    return _socketErrorStack_.RootError(SOCKET_ERR_READALL_ARGNULL);

  unsigned long totalNumBytesRead=0;
  unsigned long numBytesToRead=numBytes;
  int numBytesRead=0;
  while (totalNumBytesRead<numBytes)
  {
    if (!Read(buffer, numBytesToRead, &numBytesRead))
      return _socketErrorStack_.PushError(SOCKET_ERR_READALL_READ);

    totalNumBytesRead += (unsigned long) numBytesRead;
    numBytesToRead = numBytes - totalNumBytesRead;
  }

  return true;
}

/*  */ 
bool SocketBase::WriteAll(const char* buffer, unsigned long numBytes)
{
  if (!isConnected_)
    return _socketErrorStack_.RootError(SOCKET_ERR_WRITEALL_NOTCONNECTED);

  if (!buffer)
    return _socketErrorStack_.RootError(SOCKET_ERR_WRITEALL_ARGNULL);

  unsigned long totalNumBytesWritten=0;
  unsigned long numBytesToWrite=numBytes;
  int numBytesWritten=0;
  while (totalNumBytesWritten<numBytes)
  {
    if (!Write(buffer, numBytesToWrite, &numBytesWritten))
      return _socketErrorStack_.PushError(SOCKET_ERR_WRITEALL_WRITE);

    totalNumBytesWritten += (unsigned long) numBytesWritten;
    numBytesToWrite = numBytes - totalNumBytesWritten;
  }

  return true;
}

/*  */ 
bool SocketBase::ReadMessage(char* buffer, unsigned long bufferLength, unsigned long* messageLength)
{
  if (!isConnected_)
    return _socketErrorStack_.RootError(SOCKET_ERR_READMESSAGE_NOTCONNECTED);

  if (!buffer)
    return _socketErrorStack_.RootError(SOCKET_ERR_READMESSAGE_ARGNULL);

  char msgLengthBuffer[BLIB_ULONGSIZE];
  if (!ReadAll(msgLengthBuffer, BLIB_ULONGSIZE))
    return _socketErrorStack_.PushError(SOCKET_ERR_READMESSAGE_READMSGLENGTH);

  unsigned long theMessageLength;
  memcpy(&theMessageLength, msgLengthBuffer, BLIB_ULONGSIZE);
  theMessageLength = ntohl(theMessageLength);

  if (theMessageLength>bufferLength)
    return _socketErrorStack_.RootError(SOCKET_ERR_READMESSAGE_BUFFERTOOSMALL);

  *messageLength = theMessageLength;
  
  if (!ReadAll(buffer, theMessageLength))
    return _socketErrorStack_.RootError(SOCKET_ERR_READMESSAGE_READMSGBODY);

  return true;
}

/*  */ 
bool SocketBase::WriteMessage(const char* buffer, unsigned long messageLength)
{
  if (!isConnected_)
    return _socketErrorStack_.RootError(SOCKET_ERR_WRITEMESSAGE_NOTCONNECTED);

  if (!buffer)
    return _socketErrorStack_.RootError(SOCKET_ERR_WRITEMESSAGE_ARGNULL);

  char msgLengthBuffer[BLIB_ULONGSIZE];
  unsigned long theMessageLength = htonl(messageLength);
  memcpy(msgLengthBuffer, &theMessageLength, BLIB_ULONGSIZE);

  if (!WriteAll(msgLengthBuffer, BLIB_ULONGSIZE))
    return _socketErrorStack_.PushError(SOCKET_ERR_WRITEMESSAGE_WRITEMSGLENGTH);

  if (!WriteAll(buffer, messageLength))
    return _socketErrorStack_.PushError(SOCKET_ERR_WRITEMESSAGE_WRITEMSGBODY);

  return true;
}

/*  */ 
bool SocketBase::ReadMessage(std::string& message)
{
  if (!isConnected_)
    return _socketErrorStack_.RootError(SOCKET_ERR_READMESSAGE_NOTCONNECTED);

  char msgLengthBuffer[BLIB_ULONGSIZE];
  if (!ReadAll(msgLengthBuffer, BLIB_ULONGSIZE))
    return _socketErrorStack_.PushError(SOCKET_ERR_READMESSAGE_READMSGLENGTH);

  unsigned long theMessageLength;
  memcpy(&theMessageLength, msgLengthBuffer, BLIB_ULONGSIZE);
  theMessageLength = ntohl(theMessageLength);

  message.resize(theMessageLength);
  if (!ReadAll((char*)message.c_str(), theMessageLength))
    return _socketErrorStack_.RootError(SOCKET_ERR_READMESSAGE_READMSGBODY);

  return true;
}

/*  */ 
bool SocketBase::WriteMessage(const std::string& message)
{
  if (!WriteMessage(message.c_str(), message.length()))
    return _socketErrorStack_.PushError(SOCKET_ERR_WRITEMESSAGE_WRITEMESSAGE);

  return true;
}

/*  */ 
bool SocketBase::Close()
{
  if (!isConnected_)
    return _socketErrorStack_.RootError(SOCKET_ERR_CLOSE_NOTCONNECTED);

  if (!CloseSocketHandle(socketHandle_))
    return _socketErrorStack_.PushError(SOCKET_ERR_CLOSE);

  socketHandle_ = 0;
  isConnected_ = false;
  return true;
}

/*  */ 
bool SocketBase::GetInfo(std::string* ipAddressLocal, std::string* ipAddressPeer, unsigned short* portNumber)
{
  if (!isConnected_)
    return _socketErrorStack_.RootError(SOCKET_ERR_GETINFO_NOTCONNECTED);

  if (ipAddressLocal)
    *ipAddressLocal = ipAddressLocal_;

  if (ipAddressPeer)
    *ipAddressPeer = ipAddressPeer_;

  if (portNumber)
    *portNumber = port_;

  return true;
}

/*  */ 
bool SocketBase::IsBlocking()
{
  return isBlocking_;
}

/*  */ 
bool SocketBase::IsConnected()
{
  return isConnected_;
}



/* SocketServer */ 
SocketServer::SocketServer(bool isBlocking)
:SocketBase(isBlocking)
{
}

/*  */ 
bool SocketServer::Listen(unsigned short portNumber)
{
  if (isConnected_)
    return _socketErrorStack_.RootError(SOCKET_ERR_LISTEN_CONNECTED);

  /* Create a socket */ 
  int socketHandle = socket(AF_INET, SOCK_STREAM, 0);
  if ( socketHandle == INVALID_SOCKET )
  {
    OnSocketSystemError();
    return _socketErrorStack_.PushError(SOCKET_ERR_LISTEN_SOCKET);
  }

#if !defined  WIN32
  int value = 1;
  socklen_t value_size = sizeof(value);
  if ( setsockopt(socketHandle, SOL_SOCKET, SO_REUSEADDR, &value, value_size) == SOCKET_ERROR )
  {
    CloseSocketHandle(socketHandle);
    OnSocketSystemError();
    return _socketErrorStack_.PushError(SOCKET_ERR_LISTEN_SETSOCKOPT_REUSE);
  }
#endif

  /* Create a structure to hold address info */ 
  struct sockaddr_in addressInfo;
  addressInfo.sin_family = AF_INET;
  addressInfo.sin_port = htons(portNumber);
  addressInfo.sin_addr.s_addr = INADDR_ANY;
  memset(&addressInfo.sin_zero, 0, 8);

  /* Associate the socket with the local address */ 
  if ( bind(socketHandle, (struct sockaddr*)&addressInfo, sizeof(struct sockaddr)) == SOCKET_ERROR )
  {
    CloseSocketHandle(socketHandle);
    OnSocketSystemError();
    return _socketErrorStack_.PushError(SOCKET_ERR_LISTEN_BIND);
  }

  /* Wait for incoming connection */ 
  if ( listen(socketHandle, 1) == SOCKET_ERROR )
  {
    CloseSocketHandle(socketHandle);
    OnSocketSystemError();
    return _socketErrorStack_.PushError(SOCKET_ERR_LISTEN_LISTEN);
  }

  struct sockaddr_in clientAddress;
  memset(&clientAddress.sin_zero, 0, 8);

#ifdef WIN32
  int clientAddressLength = sizeof (struct sockaddr_in);
#else
  socklen_t clientAddressLength = sizeof(struct sockaddr_in);
#endif

  /* Accept incoming connection */ 
  int retVal = accept(socketHandle, (struct sockaddr*)&clientAddress, &clientAddressLength);
  if ( retVal == INVALID_SOCKET )
  {
    CloseSocketHandle(socketHandle);
    OnSocketSystemError();
    return _socketErrorStack_.PushError(SOCKET_ERR_LISTEN_ACCEPT);
  }

  /* Assign some members now that the connection is made */ 
  socketHandle_ = retVal;

  /* Close the temporary handle */ 
  if (!CloseSocketHandle(socketHandle))
    return _socketErrorStack_.PushError(SOCKET_ERR_LISTEN_CLOSESOCKETHANDLE);

#if !defined WIN32
  /* Ignore broken pipe errors */ 
  if ( signal(SIGPIPE,SIG_IGN) == SIG_ERR ) 
    perror("signal"); 
#endif

  port_ = portNumber;
  isConnected_ = true;
  if ( !RetrieveIPAddresses(socketHandle_, &ipAddressLocal_, &ipAddressPeer_) )
    return _socketErrorStack_.PushError(SOCKET_ERR_LISTEN_RETRIEVEIPADDRESSES);

  return true;
}


/* SocketClient */ 
SocketClient::SocketClient(bool isBlocking)
:SocketBase(isBlocking)
{
}

/*  */ 
bool SocketClient::Connect(const std::string& ipAddress, unsigned short portNumber)
{
  struct sockaddr_in addressInfo;
  addressInfo.sin_family = AF_INET;
  addressInfo.sin_port = htons(portNumber);
  addressInfo.sin_addr.s_addr= inet_addr(ipAddress.c_str());
  memset(&addressInfo.sin_zero, 0, 8);

  /* Create socket */ 
  int socketHandle = socket(AF_INET,SOCK_STREAM,0);
  if ( socketHandle == INVALID_SOCKET )
  {
    OnSocketSystemError();
    return _socketErrorStack_.PushError(SOCKET_ERR_CONNECT_SOCKET);
  }

  /* Connect to server */ 
  if ( connect(socketHandle, (struct sockaddr *)&addressInfo, sizeof(struct sockaddr)) == SOCKET_ERROR)
  {
    CloseSocketHandle(socketHandle);
    OnSocketSystemError();
    return _socketErrorStack_.PushError(SOCKET_ERR_CONNECT_CONNECT);
  }

#if !defined WIN32
  /* Ignore broken pipe errors */ 
  if( signal(SIGPIPE, SIG_IGN) == SIG_ERR )
    perror("signal"); 
#endif
  
  socketHandle_ = socketHandle;
  port_ = portNumber;
  isConnected_ = true;
  if ( !RetrieveIPAddresses(socketHandle_, &ipAddressLocal_, &ipAddressPeer_) )
    return _socketErrorStack_.PushError(SOCKET_ERR_CONNECT_RETRIEVEIPADDRESSES);

  return true;
}


#endif /* SOCKET_CPP */ 
