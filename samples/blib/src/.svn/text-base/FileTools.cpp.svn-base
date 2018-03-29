/*! \file  FileTools.cpp
** \date 	19 jul 2002
** \author	Bert Godderis
** \copyright (C) 2003 Bert Godderis
** \license wxWindows Library License : http://opensource.org/licenses/wxwindows
** 
** \brief  Implements utility functions to deal with the file system
**
**/

#include <sys/types.h>
#include <sys/stat.h>
#include <cstdio>
#include <sstream>

#if defined WIN32

  #pragma warning(disable:4786) 
  #include <process.h>
  #include <io.h>
  #include <windows.h>
  #include <direct.h>
  #define S_IFDIR _S_IFDIR
  #define S_IFREG _S_IFREG

#else

  #define _MAX_PATH 260
  #include <unistd.h>
  #include <sys/vfs.h>
  #include <stdint.h>
  #include <dirent.h>

#endif

#include "StringTools.hpp"
#include "FileTools.hpp"

using namespace std;

namespace blib {


static const unsigned long POW_1024_3 = 1073741824;

#if defined WIN32

  static long _fileFindHandle_;
  static struct _finddata_t _fileInfo_;

#else

  ::DIR* _dirInfo_;

#endif

static ErrorStack _errorStack_;
static NotSpec _noDirSpec_(new OrSpec(new NameSpec("."), new NameSpec("..")));
static const Spec* _pFindSpec_;

BLIB_GLOBALINIT_BEGIN(FILE_TOOLS_CPP_ERRORSTACK)

  _errorStack_.DefineError(FT_ERR_NO_ERROR, "FileTools: No error occurred. (FT_ERR_NO_ERROR)");
  _errorStack_.DefineError(FT_ERR_GETSTAT, "blib::GetStat() Error while retrieving stat. (FT_ERR_GETSTAT)");
  _errorStack_.DefineError(FT_ERR_GETTIME, "blib::GetTime() (FT_ERR_GETTIME)");
  _errorStack_.DefineError(FT_ERR_GETTIME_NOEXIST, "blib::GetTime() The item doesn't exist (FT_ERR_GETTIME_NOEXIST)");
  _errorStack_.DefineError(FT_ERR_GETSIZE, "blib::GetSize() (FT_ERR_GETSIZE)");
  _errorStack_.DefineError(FT_ERR_GETSIZE_NOEXIST, "blib::GetSize() The item doesn't exist (FT_ERR_GETSIZE_NOEXIST)");
  _errorStack_.DefineError(FT_ERR_GETFREESPACEONVOLUME, "blib::GetFreeSpaceOnVolume() (FT_ERR_GETFREESPACEONVOLUME)");
  _errorStack_.DefineError(FT_ERR_GETTOTALSPACEONVOLUME, "blib::GetTotalSpaceOnVolume() (FT_ERR_GETTOTALSPACEONVOLUME)");
  _errorStack_.DefineError(FT_ERR_CHANGEDIR, "blib::ChangeDir() (FT_ERR_CHANGEDIR)");
  _errorStack_.DefineError(FT_ERR_CHANGEDIR_NOEXIST, "blib::ChangeDir() The item doesn't exist (FT_ERR_CHANGEDIR_NOEXIST)");
  _errorStack_.DefineError(FT_ERR_CREATEDIR, "blib::CreateDir() (FT_ERR_CREATEDIR)");
  _errorStack_.DefineError(FT_ERR_CREATEDIR_NOEXISTPARENT, "blib::CreateDir() The item's parent directory doesn't exist (FT_ERR_CREATEDIR_NOEXISTPARENT)");
  _errorStack_.DefineError(FT_ERR_COPY_NOEXISTSOURCE, "blib::Copy() The source item does't exist (FT_ERR_COPY_NOEXISTSOURCE)");
  _errorStack_.DefineError(FT_ERR_COPY_NOEXISTPARENTDEST, "blib::Copy() The destination item's parent directory doesn't exist (FT_ERR_COPY_NOEXISTPARENTDEST)");
  _errorStack_.DefineError(FT_ERR_COPY_DESTNEWER, "blib::Copy() The destination item exists and is newer than the source item (FT_ERR_COPY_DESTNEWER)");
  _errorStack_.DefineError(FT_ERR_COPY, "blib::Copy() (FT_ERR_COPY)");
  _errorStack_.DefineError(FT_ERR_RENAME, "blib::Rename() (FT_ERR_RENAME)");
  _errorStack_.DefineError(FT_ERR_RENAME_NOEXIST, "blib::Rename() The item doesn't exist (FT_ERR_RENAME_NOEXIST)");
  _errorStack_.DefineError(FT_ERR_REMOVE, "blib::Remove() (FT_ERR_REMOVE)");
  _errorStack_.DefineError(FT_ERR_REMOVE_NOEXIST, "blib::Remove() The item doesn't exist (FT_ERR_REMOVE_NOEXIST)");
  _errorStack_.DefineError(FT_ERR_REMOVE_GETITEMS, "blib::Remove() Error while calling GetItems() (FT_ERR_REMOVE_GETITEMS)");
  _errorStack_.DefineError(FT_ERR_REMOVE_REMOVE, "blib::Remove() Error while calling Remove() (FT_ERR_REMOVE_REMOVE)");
  _errorStack_.DefineError(FT_ERR_FINDFIRSTITEM_OPENDIR, "blib::FindFirstItem(): Error while calling opendir() (FT_ERR_FINDFIRSTITEM_OPENDIR)");
  _errorStack_.DefineError(FT_ERR_FINDNEXTITEM_ILLEGALCALL, "blib::FindNextItem() Illegal call. Use FindFirstItem() first. (FT_ERR_FINDNEXTITEM_ILLEGALCALL)");
  _errorStack_.DefineError(FT_ERR_GETITEMS_NOEXISTSPARENT, "blib::GetItems() The parent directory doesn't exist (FT_ERR_GETITEMS_NOEXISTSPARENT)");
  _errorStack_.DefineError(FT_ERR_GETITEMS, "blib::GetItems() (FT_ERR_GETITEMS_NOEXISTSPARENT)");
  _errorStack_.DefineError(FT_ERR_EXEC_EMPTYCOMMAND, "blib::Exec() The given command is an empty string (FT_ERR_EXEC_EMPTYCOMMAND)");
  

BLIB_GLOBALINIT_END(FILE_TOOLS_CPP_ERRORSTACK)


//static NameSpec _currentDirSpec_(".");
//static NameSpec _parentDirSpec_("..");
/*static AndSpec _findSpec_;
static OrSpec _dirSpec_;

DO_GLOBAL_BEGIN(FILE_TOOLS_CPP_SPEC)

  _dirSpec_.AddSpec(new NameSpec(".");
  _dirSpec_.AddSpec(new NameSpec("..");

DO_GLOBAL_END(FILE_TOOLS_CPP_SPEC)*/


/****************/ 
/* Spec classes */ 
/****************/ 

/* NameSpec */ 
NameSpec::NameSpec(const string& wildcard, bool caseSensitive)
{
  wildcard_ = wildcard;
  caseSensitive_ = caseSensitive;
}

/*  */  
bool NameSpec::ItemOK(const string& path) const
{
  return (MatchesWildcard(GetItemName(path), wildcard_,caseSensitive_));
}

/* TypeSpec */ 
TypeSpec::TypeSpec(int type)
:type_(type) {}

/*  */ 
bool TypeSpec::ItemOK(const string& path) const
{
  const struct stat* fileInfo = GetStat(path);
  if (!fileInfo)
    return false;

  switch (type_)
  {
    case FILE : return (fileInfo->st_mode & S_IFREG) ? true : false;
    case DIR : return (fileInfo->st_mode & S_IFDIR) ? true : false;
  }
   
  return true;
}

/* NotSpec */ 
NotSpec::NotSpec(const Spec* spec)
:spec_(spec) {}

/*  */ 
NotSpec::~NotSpec()
{
  delete spec_;
}

/*  */ 
bool NotSpec::ItemOK(const string& path) const
{
  return (!spec_->ItemOK(path));
}

/* MultiSpec */ 
MultiSpec::MultiSpec(const Spec* spec1, const Spec* spec2)
{
  if (spec1)
    AddSpec(spec1);

  if (spec2)
    AddSpec(spec2);
}

/*  */ 
MultiSpec::~MultiSpec()
{
  Clear();
}

/*  */ 
void MultiSpec::AddSpec(const Spec* spec)
{
  specList_.push_back(spec);
}

/*  */ 
void MultiSpec::Clear()
{
  for (TSpecList::const_iterator iter = specList_.begin(); iter!=specList_.end(); iter++)
    delete *iter;
  specList_.erase(specList_.begin(), specList_.end());
}

/* AndSpec */ 
AndSpec::AndSpec(const Spec* spec1, const Spec* spec2)
:MultiSpec(spec1, spec2)
{}

/*  */ 
bool AndSpec::ItemOK(const string& path) const
{
  for (TSpecList::const_iterator iter = specList_.begin(); iter!=specList_.end(); iter++)
    if (!(*iter)->ItemOK(path))
      return false;

  return true;
}

/* OrSpec */ 
OrSpec::OrSpec(const Spec* spec1, const Spec* spec2)
:MultiSpec(spec1, spec2)
{}

/*  */ 
bool OrSpec::ItemOK(const string& path) const
{
  for (TSpecList::const_iterator iter = specList_.begin(); iter!=specList_.end(); iter++)
    if ((*iter)->ItemOK(path))
      return true;

  return false;
}

/* XorSpec */ 
XorSpec::XorSpec(const Spec* spec1, const Spec* spec2)
:MultiSpec(spec1, spec2)
{}

/*  */ 
bool XorSpec::ItemOK(const string& path) const
{
  TSpecList::size_type numOK = 0;

  for (TSpecList::const_iterator iter = specList_.begin(); (iter!=specList_.end()) && (numOK<=1); iter++)
    if ((*iter)->ItemOK(path))
      numOK++;

  return (numOK==1);
}


/***********************/ 
/* FileTools functions */ 
/***********************/ 

/*  */ 
const struct stat* GetStat(const string& itemName, bool noCache)
{
  static string lastName="";
  static struct stat lastStat;

  if (lastName!=itemName)
  {
    if (::stat(itemName.c_str(), &lastStat)<0)
    {
      _errorStack_.RootSystemError();
      _errorStack_.PushError(FT_ERR_GETSTAT);
      return 0;
    }
  }

  lastName=itemName;
  return &lastStat;
}

/*  */ 
string GetCurrentDir()
{
  char currentDir[_MAX_PATH];
  getcwd(currentDir, _MAX_PATH);
  return currentDir;
}

/*  */ 
bool Exists(const string& path, int type)
{
  if (path.empty())
    return false;

  string fullPath = GetFullPath(path);
  string parentPath = GetParentPath(fullPath);
  string origWorkingDir = GetCurrentDir();

  if (chdir(parentPath.c_str()))
    return false;

#if defined WIN32
  AndSpec theSpec(new NameSpec(GetItemName(fullPath), false), new TypeSpec(type));
#else
  AndSpec theSpec(new NameSpec(GetItemName(fullPath)), new TypeSpec(type));
#endif

  bool success = FindFirstItem(0, theSpec);
  FindClose();

  chdir(origWorkingDir.c_str());
  return success;
}

/*  */ 
bool GetTime(const string& path, time_t* time, int timeType)
{
  if (!Exists(path))
  {
    _errorStack_.RootError(FT_ERR_GETTIME_NOEXIST);
    return false;
  }

  const struct stat* fileInfo = GetStat(path);
  if (!fileInfo)
  {
    _errorStack_.PushError(FT_ERR_GETTIME);
    return false;
  }

  switch (timeType)
  {
    case CREATED : *time = fileInfo->st_ctime; break;
    case MODIFIED : *time = fileInfo->st_mtime; break;
    case ACCESSED : *time = fileInfo->st_atime; break;
  }
  
  return true;
}

/*  */ 
bool GetSize(const string& path, unsigned long* bytes, unsigned long* gigaBytes)
{
  if (!Exists(path))
  {
    _errorStack_.RootError(FT_ERR_GETSIZE_NOEXIST);
    return false;
  }

  const struct stat* statistics = GetStat(path);
  if (!statistics)
  {
    _errorStack_.PushError(FT_ERR_GETSIZE);
    return false;
  }

  *bytes =  (statistics->st_size)%POW_1024_3;
  *gigaBytes =  (statistics->st_size)/POW_1024_3;

  return true;
}

/*  */ 
bool GetFreeSpaceOnVolume(const string& path, unsigned long* bytes, unsigned long* gigaBytes)
{
  string fullPath = GetFullPath(path);

#if defined WIN32
  struct _diskfree_t freeSpace;
  if ( _getdiskfree(tolower(fullPath[0])-'a'+1, &freeSpace) )
  {
    _errorStack_.RootError(FT_ERR_GETFREESPACEONVOLUME);
    return false;
  }

  __int64 totalNumBytes = (__int64)freeSpace.bytes_per_sector * (__int64)freeSpace.sectors_per_cluster * (__int64)freeSpace.avail_clusters;
#else
  struct statfs fsInfo;
  if (!statfs(path.c_str(), &fsInfo))
    return false;
    
  uint64_t totalNumBytes = (uint64_t)fsInfo.f_bfree * (uint64_t)fsInfo.f_bsize;

#endif

  *bytes = totalNumBytes%POW_1024_3;

  if (gigaBytes)
    *gigaBytes = totalNumBytes/POW_1024_3;

  return true;
}

/*  */ 
bool GetTotalSpaceOnVolume(const string& path, unsigned long* bytes, unsigned long* gigaBytes)
{
  string fullPath = GetFullPath(path);

#if defined WIN32

  struct _diskfree_t freeSpace;
  if ( _getdiskfree(tolower(fullPath[0])-'a'+1, &freeSpace) )
  {
    _errorStack_.RootError(FT_ERR_GETTOTALSPACEONVOLUME);
    return false;
  }

  __int64 totalNumBytes = (__int64)freeSpace.bytes_per_sector * (__int64)freeSpace.sectors_per_cluster * (__int64)freeSpace.total_clusters;

#else

  struct statfs fsInfo;
  if (!statfs(path.c_str(), &fsInfo))
    return false;
    
  uint64_t totalNumBytes = (uint64_t)fsInfo.f_blocks * (uint64_t)fsInfo.f_bsize;

#endif

  *bytes = totalNumBytes%POW_1024_3;

  if (gigaBytes)
    *gigaBytes = totalNumBytes/POW_1024_3;

  return true;
}

/*  */ 
bool ChangeDir(const string& path)
{
  if (!Exists(path, DIR))
  {
    _errorStack_.RootError(FT_ERR_CHANGEDIR_NOEXIST);
    return false;
  }

  if (chdir(path.c_str()))
  {
    _errorStack_.RootSystemError();
    _errorStack_.PushError(FT_ERR_CHANGEDIR);
    return false;
  }

  return true;
}

/*  */ 
bool CreateDir(const string& path)
{
  if (Exists(path, DIR))
    return true;

  if (!Exists(GetParentPath(path), DIR))
  {
    _errorStack_.RootError(FT_ERR_CREATEDIR_NOEXISTPARENT);
    return false;
  }

  int retVal;

#if defined WIN32
  retVal = mkdir(path.c_str());
#else
  retVal = mkdir(path.c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH);
#endif

  if (retVal)
  {
    _errorStack_.RootSystemError();
    _errorStack_.PushError(FT_ERR_CREATEDIR);
    return false;
  }

  return true;
}

/*  */ 
bool CopyFile(const string& source, const string& dest)
{
#ifdef WIN32
  return ( ::CopyFile(source.c_str(), dest.c_str(), false) ? true : false);
#else

#define COPYFILE_BUFSIZE 1024

  static char buf[COPYFILE_BUFSIZE];

  ::FILE* fin = fopen(source.c_str(), "rb");
  ::FILE* fout = fopen(dest.c_str(), "wb");

  if ( (!fin) || (!fout) )
    return false;

  size_t size;

  do 
  {
    size = fread(buf, 1, COPYFILE_BUFSIZE, fin);
    fwrite(buf, 1, size, fout);
  } while (size == COPYFILE_BUFSIZE);

  fclose(fin);
  fclose(fout);
#endif

  return true;
}

/*  */ 
bool Copy(const string& sourcePath, const string& destPath, bool ifNewer)
{
  if (!Exists(sourcePath))
    return _errorStack_.RootError(FT_ERR_COPY_NOEXISTSOURCE);

  if (!Exists(GetParentPath(destPath), DIR))
    return _errorStack_.RootError(FT_ERR_COPY_NOEXISTPARENTDEST);

	if (ifNewer)
		if (Exists(destPath))
    {
      time_t timeDest, timeSource;
      GetTime(destPath, &timeDest);
      GetTime(sourcePath, &timeSource);
			if (timeDest>=timeSource) 
        return _errorStack_.RootError(FT_ERR_COPY_DESTNEWER);
    }

  if (Exists(sourcePath, DIR))
  {
    if (!Exists(destPath, DIR))
      if (!CreateDir(destPath))
        return _errorStack_.RootError(FT_ERR_COPY_CREATEDIR1);

    // copy dirs recursively
    TypeSpec dirSpec(DIR);
    StringList dirList;
    if ( !GetItems( dirList, dirSpec, sourcePath, true /*recursive*/ ) )
      return _errorStack_.RootError(FT_ERR_COPY_GETITEMS_DIRS);

    string destDir;

    StringList::iterator iter;
    for (iter = dirList.begin(); iter!=dirList.end(); iter++)
      if (!CreateDir(destPath + PATH_SEPARATOR + GetRelativePath(*iter, sourcePath)))
        return _errorStack_.RootError(FT_ERR_COPY_CREATEDIR2);

    TypeSpec fileSpec(FILE);
    StringList fileList;
    if ( !GetItems( fileList, fileSpec, sourcePath, true /*recursive*/ ) )
      return _errorStack_.RootError(FT_ERR_COPY_GETITEMS_FILES);

    for (iter = fileList.begin(); iter!=fileList.end(); iter++)
      if (!CopyFile(*iter, destPath + PATH_SEPARATOR + GetRelativePath(*iter, sourcePath)))
        return _errorStack_.RootError(FT_ERR_COPY_COPYFILE1);
    //
  }
  else
    if (!CopyFile(sourcePath, destPath))
      return _errorStack_.RootError(FT_ERR_COPY_COPYFILE2);

  return true;
}

/*  */ 
bool Rename(const string& origPath,const string& newPath)
{
  if (!Exists(origPath))
  {
    _errorStack_.RootError(FT_ERR_RENAME_NOEXIST);
    return false;
  }

  if ( rename(origPath.c_str(), newPath.c_str()) )
  {
    _errorStack_.RootSystemError();
    _errorStack_.PushError(FT_ERR_RENAME);
    return false;
  }

  return true;
}

/*  */ 
bool Remove(const string& path)
{
  if (!Exists(path))
  {
    _errorStack_.RootError(FT_ERR_REMOVE_NOEXIST);
    return false;
  }

  /* The path argument specifies a dir */ 
  if (Exists(path, DIR))
  {
    /* 1. find all entries in dir & remove them */ 
    StringList itemList;
    NameSpec theSpec("*");
    if (!GetItems(itemList, theSpec, path))
      return _errorStack_.PushError(FT_ERR_REMOVE_GETITEMS);
    StringList::iterator iter;
    for (iter=itemList.begin(); iter!=itemList.end(); iter++)
      if (!Remove(*iter))
        return _errorStack_.PushError(FT_ERR_REMOVE_REMOVE);

    /* 2. remove empty dir */ 
    if ( rmdir(path.c_str()) )
    {
      _errorStack_.RootSystemError();
      _errorStack_.PushError(FT_ERR_REMOVE);
      return false;
    }
  }

  /* The path argument specifies a file */ 
  else
  {
    if ( remove(path.c_str()) )
    {
      _errorStack_.RootSystemError();
      _errorStack_.PushError(FT_ERR_REMOVE);
      return false;
    }
  }

  return true;
}

/*  */ 
bool FindFirstItem(string* itemFound, const Spec& spec)
{
  _pFindSpec_ = &spec;

#if defined WIN32

  _fileFindHandle_ = _findfirst("*", &_fileInfo_);

  bool success = (_fileFindHandle_!=-1);

  while (success)
  {
    if ( _noDirSpec_.ItemOK(_fileInfo_.name) && spec.ItemOK(_fileInfo_.name) )
    {
      if (itemFound)
        *itemFound = _fileInfo_.name;
      return true;
    }
    success = _findnext(_fileFindHandle_, &_fileInfo_) ? false : true;
  }

#else
  _dirInfo_ = opendir(".");
  if (!_dirInfo_)
  {
    _errorStack_.RootSystemError();
    return _errorStack_.PushError(FT_ERR_FINDFIRSTITEM_OPENDIR);
  }

  struct dirent* entryInfo = readdir(_dirInfo_); 
  bool success = entryInfo ? true : false;
  
  while (success)
  {
    if ( _noDirSpec_.ItemOK(entryInfo->d_name) && spec.ItemOK(entryInfo->d_name) )
    {
      if (itemFound)
        *itemFound = entryInfo->d_name;
      return true; 
    }

    success = (entryInfo = readdir(_dirInfo_)) ? true : false;
  }

#endif

  return false;
}

/*  */ 
bool FindNextItem(string* itemFound)
{
  if (!_pFindSpec_)
  {
    _errorStack_.PushError(FT_ERR_FINDNEXTITEM_ILLEGALCALL);
    return false;
  }

#if defined WIN32
  bool success = ( _findnext(_fileFindHandle_, &_fileInfo_) != -1 );

  while (success)
  {
    if (_pFindSpec_->ItemOK(_fileInfo_.name) && _noDirSpec_.ItemOK(_fileInfo_.name) )
    {
      if (itemFound)
        *itemFound = _fileInfo_.name;
      return true;
    }
    success = _findnext(_fileFindHandle_, &_fileInfo_) ? false : true;
  }

#else
  struct dirent* entryInfo = readdir(_dirInfo_); 
  bool success = entryInfo ? true : false;

  while (success)
  {
    if ( _noDirSpec_.ItemOK(entryInfo->d_name) && _pFindSpec_->ItemOK(entryInfo->d_name) )
    {
      if (itemFound)
        *itemFound = entryInfo->d_name;
      return true;
    }
    success = (entryInfo = readdir(_dirInfo_)) ? true : false;
  }

#endif

  return false;
}

/*  */ 
void FindClose()
{
  _pFindSpec_ = 0;

#if defined WIN32
	_findclose(_fileFindHandle_);
#else

  closedir(_dirInfo_);
#endif
}

/*  */ 
bool GetItems(StringList& itemList, const Spec& spec, const string& parentPath, bool recursive)
{
  string origDir = GetCurrentDir();
  string fullParentPath = GetFullPath(parentPath);

  if (!Exists(fullParentPath))
    return _errorStack_.RootError(FT_ERR_GETITEMS_NOEXISTSPARENT);

  ChangeDir(fullParentPath);

  string itemFound;
  bool success = FindFirstItem(&itemFound, spec);
  while (success)
  {
    itemList.push_back(GetFullPath(itemFound));
    success = FindNextItem(&itemFound);
  }
  FindClose();

  if (recursive)
  {
    StringList subdirList;
    TypeSpec subdirSpec(DIR);
    GetItems(subdirList, subdirSpec, fullParentPath);

    if (subdirList.size())
      for (StringList::iterator iter = subdirList.begin(); iter!=subdirList.end(); iter++)
        if (!GetItems(itemList, spec, *iter, true))
          return _errorStack_.PushError(FT_ERR_GETITEMS);
  }

  ChangeDir(origDir);
  return true;
}

/*  */ 
bool IsRoot(const string& path)
{
  bool isRoot = false;
  string thePath = EliminateRedundancies(GetFullPath(path));
#if defined WIN32
  StringList stringList;
  SplitPath(thePath, stringList);
  return ( (stringList.size()==1) && (IsFullPath(thePath)) );
#else
  return (thePath == "/");
#endif
}

/*  */ 
bool IsFullPath(const string& path)
{
#if defined WIN32
  return ( (strchr(path.c_str(), ':')) || ( path[0] == PATH_SEPARATOR) );
#else
  return ( path[0] == PATH_SEPARATOR );
#endif
}

/*  */ 
string GetFullPath(const string& path)
{
#if defined WIN32
  if ( path[path.size()-1] == ':' )
    return path + '\\';
#endif

  string fullPath;
  if (IsFullPath(path))
    fullPath = path;
  else
    fullPath = GetCurrentDir() + PATH_SEPARATOR + path;

  return EliminateRedundancies(fullPath);
}

/*  */ 
string GetRelativePath(const string& item, const string& refDir)
{
  string theItem = GetFullPath(item);
  string theRefDir = GetFullPath(refDir);

  StringList itemStringList;
  SplitPath(theItem, itemStringList);

  StringList refDirStringList;
  SplitPath(theRefDir, refDirStringList);

  bool elementsRemoved = false;

  while ( (refDirStringList.size()) && (itemStringList.size()) )
    if ( *(refDirStringList.begin()) == *(itemStringList.begin()) )
    {
      refDirStringList.pop_front();
      itemStringList.pop_front();
      elementsRemoved = true;
    }
    else
      break;

  if (elementsRemoved)
    for (StringList::size_type numFolders = refDirStringList.size(); numFolders; numFolders--)
      itemStringList.push_front("..");

  return ConcatPath(itemStringList);
}

/*  */ 
string GetAppPath()
{
#if defined WIN32
  char fileName[_MAX_PATH];
  GetModuleFileName(0, fileName, _MAX_PATH);
  return fileName;
#else
  return "";
#endif
}

/*  */ 
string GetURLSavePath(const string& path)
{
  string returnString = path;
  for (string::size_type charIndex=0; charIndex<path.size(); charIndex++)
    if (returnString[charIndex] == '\\')
      returnString[charIndex] = '/';

  return returnString;
}

/*  */ 
string EliminateRedundancies(const string& redundantPath)
{
  StringList oldList;
  StringList newList;
  SplitPath(redundantPath, oldList);
  oldList.remove(".");
  bool previousWasDirName=false;

  StringList::iterator iter;
  for (iter = oldList.begin(); iter!=oldList.end(); iter++)
  {
    if ( (*iter == "..") && (previousWasDirName) )
      newList.pop_back();
    else
      newList.push_back(*iter);
    if (newList.size())
      previousWasDirName = ( newList.back() != ".." );
    else
      previousWasDirName = false;
  }

  string newPath = ConcatPath(newList);

#if !defined WIN32
  if (IsFullPath(redundantPath))
    newPath = "/" + newPath;
#endif

  return newPath;
}

/*  */ 
string GetParentPath(const string& path)
{
  if (IsRoot(path))
    return GetFullPath(path);

  StringList stringList;
  SplitPath(GetFullPath(path), stringList);
  stringList.pop_back();
  string parentPath = ConcatPath(stringList);

  if (IsRoot(parentPath))
    return GetFullPath(parentPath);

#if !defined WIN32
  parentPath = "/" + parentPath;
#endif

  return parentPath;
}

/*  */ 
string GetItemName(const string& path)
{
#if !defined WIN32
  if (IsRoot(path))
    return "/";
#endif

  StringList stringList;
  SplitPath(path, stringList);
  string itemName = ( stringList.size() ? stringList.back() : "" );

  return itemName;
}

/*  */ 
void SplitFileName(const string& fullFileName, string* fileName, string* extension)
{
  StringList stringList;
  SplitString(fullFileName, ".", stringList);
  if (extension)
    *extension = stringList.back();
  stringList.pop_back();
  if (fileName)
    *fileName = ConcatString(stringList, ".");
}

/*  */ 
void SplitPath(const string& path, StringList& stringList)
{
#if defined WIN32
  string delimiters = "  ";
  delimiters[0] = PATH_SEPARATOR;
  delimiters[1] = '/';
#else
  string delimiters = " ";
  delimiters[0] = PATH_SEPARATOR;
#endif
  SplitString(path, delimiters, stringList);
}


/*  */ 
string ConcatPath(const StringList& stringList)
{
  string path;
  StringList::const_iterator iter;
  for (iter = stringList.begin(); iter!=stringList.end(); iter++)
    path += *iter + PATH_SEPARATOR;

  path = path.substr(0, path.length()-1);

  return path;
}

#if 0
/*  */ 
bool Exec(const string& command, int* exitCode)
{
#ifdef WIN32

  /* Split up command string into arguments */ 
  StringList argList;
  SplitString(command, " ", argList);
  if (!argList.size())
    return _errorStack_.PushError(FT_ERR_EXEC_EMPTYCOMMAND);

  /* Build argv */ 
  const char ** argv = new const char*[argList.size()+1];
  unsigned short argIndex=0;
  for (StringList::iterator iter=argList.begin(); iter!=argList.end(); iter++, argIndex++)
    argv[argIndex] = iter->c_str();
  argv[argIndex] = 0;

  /* Call command */ 
  int code = _spawnvp(_P_WAIT, argv[0], argv);
 
  int hihi = errno;
  char* haha = strerror(errno);
  if (exitCode)
    *exitCode = code;

  delete [] argv;

  return true;

#else

  

#endif
}
#endif

/*  */ 
ErrorStack* GetErrorStackFileTools()
{
  return &_errorStack_;
}

} /* namespace blib */
