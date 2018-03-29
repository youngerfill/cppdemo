/*! \file error.hpp
** \date 2 dec 2002
** \author Bert Godderis
** \copyright (C) 2003 Bert Godderis
** \license wxWindows Library License : http://opensource.org/licenses/wxwindows
** 
** \brief Implements an error stack
**  
**/

#ifdef WIN32
#pragma warning(disable:4786)
#include <windows.h>
#else
#include <errno.h>
#endif

#include <algorithm>
#include <sstream>

#include "definitions.hpp"
#include "error.hpp"

using namespace std;
using namespace blib;

/*  */ 
ErrorStack::ErrorStack()
{
  indexUnTranslated_ = 0;
//  rootIsSystemError_ = false;
}

/*  */ 
void ErrorStack::DefineError(int errorCode, const std::string& errorString)
{
  errorString_[errorCode] = errorString;
}

/*  */ 
void ErrorStack::Clear()
{
  errorCodeStack_.erase(errorCodeStack_.begin(), errorCodeStack_.end());
  errorStringStack_.erase(errorStringStack_.begin(), errorStringStack_.end());
  indexUnTranslated_ = 0;
//  rootIsSystemError_ = false;
}

/*  */ 
bool ErrorStack::RootSystemError()
{
  Clear();
//  rootIsSystemError_ = true;
  ostringstream auxStream;
  auxStream << "The following system error occurred: " << endl << strerror(errno) << endl;
  PushError(-1, auxStream.str());
  return false;
}

#if defined WIN32

/*  */ 
bool ErrorStack::RootMSWindowsError(int errorCode)
{
  LPVOID lpMsgBuf;
  FormatMessage( 
      FORMAT_MESSAGE_ALLOCATE_BUFFER | 
      FORMAT_MESSAGE_FROM_SYSTEM | 
      FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL,
      errorCode,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
      (LPTSTR) &lpMsgBuf,
      0,
      NULL 
  );

  ostringstream auxStream;
  auxStream << "The following Windows system error has occurred: " << endl << (LPCTSTR)lpMsgBuf;
  RootError(-1, auxStream.str());
  LocalFree(lpMsgBuf);

  return false;
}

/*  */ 
bool ErrorStack::RootMSWindowsError()
{
  return RootMSWindowsError( GetLastError() );
}
#endif

/*  */ 
bool ErrorStack::RootError(int errorCode, const std::string& customErrorString)
{
  Clear();
  PushError(errorCode, customErrorString);
  return false;
}

/*  */ 
bool ErrorStack::PushError(int errorCode, const std::string& customErrorString)
{
  if (!customErrorString.empty())
    errorString_[errorCode] = customErrorString;

  errorCodeStack_.push_back(errorCode);
  return false;
}

/*  */ 
int ErrorStack::GetErrorCode(IntVector::size_type index) const
{
  return (errorCodeStack_.size()) ? errorCodeStack_[std::min(index, errorCodeStack_.size()-1)] : 0;
}

/*  */ 
int ErrorStack::GetLastErrorCode() const
{
  return (errorCodeStack_.size()) ? errorCodeStack_[errorCodeStack_.size()-1] : 0;
}

/*  */ 
std::string ErrorStack::GetErrorString(StringVector::size_type index)
{
/*  if (!indexUnTranslated_)
    if (rootIsSystemError_)
    {
      std::ostringstream auxStream;
      auxStream << "The following system error occurred: '" << strerror(errorCodeStack_[indexUnTranslated_]) << "'";
      errorStringStack_.push_back(auxStream.str());
      indexUnTranslated_++;
    }*/

  for (; indexUnTranslated_<errorCodeStack_.size(); indexUnTranslated_++)
  {
    int errorCode = errorCodeStack_[indexUnTranslated_];
    std::string errorString = errorString_[errorCode];
    errorStringStack_.push_back(errorString_[errorCodeStack_[indexUnTranslated_]]);
  }

  return errorStringStack_[(errorCodeStack_.size()) ? (std::min(index, errorStringStack_.size()-1)) : 0];
}

/*  */ 
void ErrorStack::Prepend(ErrorStack* errorStack)
{
  Clear();

  for (IntVector::size_type index=0; index<errorStack->GetNumErrors(); index++)
    errorStringStack_.push_back(errorStack->GetErrorString(index));
}

/*  */ 
IntVector::size_type ErrorStack::GetNumErrors() const
{
  return errorStringStack_.size() + errorCodeStack_.size() - indexUnTranslated_;
}

/*  */ 
IntVector::size_type ErrorStack::GetNumErrorCodes() const
{
  return errorCodeStack_.size();
}

/*  */ 
void ErrorStack::PrintErrors(std::ostream& output, bool inverse)
{
  if (inverse)
    for (IntVector::size_type index=GetNumErrors(); index; )
      output << GetErrorString(--index) << std::endl;
  else
    for (IntVector::size_type index=0; index<GetNumErrors(); index++)
      output << GetErrorString(index) << std::endl;
}


