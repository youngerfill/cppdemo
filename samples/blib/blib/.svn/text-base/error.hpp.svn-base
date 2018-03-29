/*! \file error.hpp
** \date 11 nov 2002
** \author Bert Godderis
** \copyright (C) 2003 Bert Godderis
** \license wxWindows Library License : http://opensource.org/licenses/wxwindows
** 
** \brief Declares and implements an error stack
**  
**/

#if !defined ERROR_HPP
#define ERROR_HPP

#ifdef WIN32
#pragma warning(disable:4786) 
#endif

#include "definitions.hpp"

namespace blib {

class ErrorStack
{
public:
  /*  */ 
  ErrorStack();

  /*  */ 
  void DefineError(int errorCode, const std::string& errorString);

  /*  */ 
  void Clear();

  /*  */ 
  bool RootError(int errorCode, const std::string& customErrorString="");

  /*  */ 
  bool RootSystemError();

#if defined WIN32
  /*  */ 
  bool RootMSWindowsError();
  bool RootMSWindowsError(int errorCode);
#endif

  /*  */ 
  bool PushError(int errorCode, const std::string& customErrorString="");

  /*  */ 
  int GetErrorCode(IntVector::size_type index) const;

  /*  */ 
  int GetLastErrorCode() const;

  /*  */ 
  std::string GetErrorString(StringVector::size_type index);

  /*  */ 
  void Prepend(ErrorStack* errorStack);

  /*  */ 
  IntVector::size_type GetNumErrors() const;

  /*  */ 
  IntVector::size_type GetNumErrorCodes() const;

  /*  */ 
  void PrintErrors(std::ostream& output, bool inverse=true);

protected:
  IntStringMap errorString_;
  IntVector errorCodeStack_;
  StringVector errorStringStack_;
  IntVector::size_type indexUnTranslated_;
//  bool rootIsSystemError_;
};


} /* namespace blib  */ 


#endif /* ERROR_HPP */ 

