/*! \file FTTApp.cpp
** \date 03 June 2003
** \author Bert Godderis
** 
** \brief Implements the application class for the FileToolsTest program
**  
**/

#ifdef WIN32
#pragma warning(disable:4786) 
#endif

#include <iostream>
#include <fstream>

#include "error.hpp"
#include "FileTools.hpp"
#include "FTTApp.hpp"

using namespace blib;
using namespace std;


/*  */ 
FTTApp::FTTApp()
{
  errorStack_ = new ErrorStack;
  errorStack_->DefineError(FTT_ERR_NO_ERROR, "FTTApp : No error occurred (FTT_ERR_NO_ERROR)");
  errorStack_->DefineError(FTT_ERR_ERROR, "FTTApp : An error has occurred (FTT_ERR_ERROR)");
}

/*  */ 
FTTApp::~FTTApp()
{
  delete errorStack_;
}

/*  */ 
bool FTTApp::Enter()
{
  return true;
}

/*  */ 
bool FTTApp::Run()
{
  TestCopy();
  return true;
}

/*  */ 
int FTTApp::Leave()
{
  int numErrors = errorStack_->GetNumErrorCodes();
  if (numErrors)
  {
    cout << "An error has occurred:" << endl << endl;
    errorStack_->PrintErrors(cout);
    return errorStack_->GetErrorCode(numErrors-1);
  }

  return 0;
}

/*  */ 
void FTTApp::TestCopy()
{
  if (!ChangeDir(string("..") + PATH_SEPARATOR + string("data")))
  {
    errorStack_->Prepend(GetErrorStackFileTools());
    errorStack_->PushError(FTT_ERR_ERROR);
    return;
  }

  if (!Copy("subdir", "destDir"))
  {
    errorStack_->Prepend(GetErrorStackFileTools());
    errorStack_->PushError(FTT_ERR_ERROR);
    return;
  }
}

/*  */ 
void FTTApp::Test_Various()
{
  cout << "Changing dir to data ... ";

  if (!ChangeDir("../data"))
  {
    errorStack_->Prepend(GetErrorStackFileTools());
    errorStack_->PushError(FTT_ERR_ERROR);
    return;
  }

  cout << "done." << endl;

  if (Exists("subdir", blib::DIR))
  {
    cout << "Removing previous data ...";

    if (!Remove("subdir"))
    {
      errorStack_->Prepend(GetErrorStackFileTools());
      errorStack_->PushError(FTT_ERR_ERROR);
      return;
    }

    cout << "done." << endl;
  }

  cout << "Creating directory tree ...";
  if (!CreateDir("subdir"))
  {
    errorStack_->Prepend(GetErrorStackFileTools());
    errorStack_->PushError(FTT_ERR_ERROR);
    return;
  }

  if (!ChangeDir("subdir"))
  {
    errorStack_->Prepend(GetErrorStackFileTools());
    errorStack_->PushError(FTT_ERR_ERROR);
    return;
  }

  if (!CreateDir("subdir01"))
  {
    errorStack_->Prepend(GetErrorStackFileTools());
    errorStack_->PushError(FTT_ERR_ERROR);
    return;
  }

  if (!CreateDir("subdir02"))
  {
    errorStack_->Prepend(GetErrorStackFileTools());
    errorStack_->PushError(FTT_ERR_ERROR);
    return;
  }

  if (!ChangeDir("subdir01"))
  {
    errorStack_->Prepend(GetErrorStackFileTools());
    errorStack_->PushError(FTT_ERR_ERROR);
    return;
  }

  if (!CreateDir("subdir0101"))
  {
    errorStack_->Prepend(GetErrorStackFileTools());
    errorStack_->PushError(FTT_ERR_ERROR);
    return;
  }

  if (!CreateDir("subdir0102"))
  {
    errorStack_->Prepend(GetErrorStackFileTools());
    errorStack_->PushError(FTT_ERR_ERROR);
    return;
  }

  if (!ChangeDir(string("..") + PATH_SEPARATOR + string("..")))
  {
    errorStack_->Prepend(GetErrorStackFileTools());
    errorStack_->PushError(FTT_ERR_ERROR);
    return;
  }

  if (!Exists("file.ext"))
  {
    ofstream fileStream("file.ext");
    fileStream << "Contents." << endl;
  }

  if (!Copy("file.ext", "subdir/subdir01/subdir0101/file0101a.tmp"))
  {
    errorStack_->Prepend(GetErrorStackFileTools());
    errorStack_->PushError(FTT_ERR_ERROR);
    return;
  }

  if (!Copy("file.ext", "subdir/subdir01/subdir0101/file0101b.tmp"))
  {
    errorStack_->Prepend(GetErrorStackFileTools());
    errorStack_->PushError(FTT_ERR_ERROR);
    return;
  }

  if (!Copy("file.ext", "subdir/subdir01/subdir0102/file0102a.tmp"))
  {
    errorStack_->Prepend(GetErrorStackFileTools());
    errorStack_->PushError(FTT_ERR_ERROR);
    return;
  }

  if (!Copy("file.ext", "subdir/subdir01/subdir0102/file0102b.tmp"))
  {
    errorStack_->Prepend(GetErrorStackFileTools());
    errorStack_->PushError(FTT_ERR_ERROR);
    return;
  }

  cout << "done." << endl;
  cout << "Current dir = '" << GetCurrentDir() << "'" << endl;

  StringList itemList;
  TypeSpec theSpec(blib::FILE);
  GetItems(itemList, theSpec, ".", true);
  StringList::iterator iter;
  for (iter = itemList.begin(); iter!= itemList.end(); iter++)
  {
    cout << GetItemName(*iter) << endl;
  }

}

/*  */ 
void FTTApp::Test_IsFullPath()
{
  string path1, path2;

#if defined WIN32
  path1 = "haha\\hihi\\dada";
  path2 = "d:\\data";
#else
  path1 = "haha/hihi/dada";
  path2 = "/";
#endif

  cout << "path1 = '" << path1 << "'" << endl;
  cout << "IsFullPath(path1) == " << (IsFullPath(path1) ? "true" : "false") << endl;
  cout << "path2 = '" << path2 << "'" << endl;
  cout << "IsFullPath(path2) == " << (IsFullPath(path2) ? "true" : "false") << endl;
}

/*  */ 
void FTTApp::Test_EliminateRedundancies()
{
  string path1 = "/root/data/pics/../../projects";
  cout << "path1 = " << path1 << endl;
  cout << "EliminateRedundancies(path1) == " << EliminateRedundancies(path1) << endl;

/*  StringList theList;
  SplitPath(path1, theList);
  StringList::iterator iter;
  for (iter = theList.begin(); iter != theList.end(); iter++)
    cout << "'" << *iter << "'" << endl;*/
}

/*  */ 
void FTTApp::Test_GetFullPath()
{
  string path1 = "/root/data/pics/../../projects";
  string path2 = GetCurrentDir();
  cout << "path1 == " << path1 << endl;
  cout << "path2 = GetCurrentDir() == " << GetCurrentDir() << endl;
  cout << "GetFullPath(path1) == " << GetFullPath(path1) << endl;
}

/*  */ 
void FTTApp::Test_NameSpec_ItemOK()
{
  cout << "Checking for 'haha' ..." << endl;
  string fileName;
  NameSpec theSpec("hmm");

  if (theSpec.ItemOK("haha"))
    cout << "OK!" << endl;
  else
    cout << "NOK!" << endl;
}

/*  */ 
void FTTApp::Test_FindFirstItem()
{
  cout << "Looking for 'haha' ..." << endl;
  string fileName;
  NameSpec theSpec("haha");

  if (FindFirstItem(&fileName, theSpec))
  {
    cout << "Found: '" << fileName << "'" << endl;
  }
  else
  {
    cout << "Not found." << endl;
  }

  FindClose();
}

/*  */ 
void FTTApp::Test_IsRoot()
{
  string path1;
#if defined WIN32
  path1 = "d:\\";
#else
  path1 = "/data/../.";
#endif

  cout << "path1 == " << path1 << endl;
  cout << "IsRoot(path1) == " << ( IsRoot(path1) ? "true" : "false" ) << endl;
}

/*  */ 
void FTTApp::Test_GetParentPath()
{
  string path1;

#if defined WIN32
  path1 = "d:\\";
#else
  path1 = "/";
#endif

  cout << "path1 == " << path1 << endl;
  cout << "GetParentPath(path1) == " << GetParentPath(path1) << endl;
}

/*  */ 
void FTTApp::Test_GetItemName()
{
  string path1 = "/haha/../.";
  string path2 = GetCurrentDir();
  cout << "path1 == " << path1 << endl;
  cout << "path2 = GetCurrentDir() == " << GetCurrentDir() << endl;
  cout << "GetItemName(path1) == " << GetItemName(path1) << endl;
}

/*  */ 
BLIB_MAIN(FTTApp)
