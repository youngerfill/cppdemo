/*! \file FTTApp.hpp
** \date 03 June 2003
** \author Bert Godderis
** 
** \brief Declares the application class for the FileToolsTest program
**  
**/

#if !defined FTTAPP_HPP
#define FTTAPP_HPP

#ifdef WIN32
#pragma warning(disable:4786) 
#endif

#include "app.hpp"

namespace blib {

class ErrorStack;

enum
{
  FTT_ERR_NO_ERROR = 0,
  FTT_ERR_ERROR,
};

/*  */ 
class FTTApp : public App
{
public:
  FTTApp();
  ~FTTApp();

protected:
	virtual bool Enter();
	virtual bool Run();
	virtual int Leave();

  void Test_IsFullPath();
  void Test_EliminateRedundancies();
  void Test_GetFullPath();
  void Test_SplitPath();
  void Test_ConcatPath();
  void Test_IsRoot();
  void Test_GetParentPath();
  void Test_GetItemName();
  void Test_NameSpec_ItemOK();
  void Test_FindFirstItem();
  void Test_Various();
  void TestCopy();

  ErrorStack* errorStack_;
};

} /* namespace blib */ 

#endif /* FTTAPP_HPP */ 
