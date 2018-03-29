/*! \file FileTools.hpp
** \date 19 jul 2002
** \author Bert Godderis
** 
** \brief Declares utility functions to deal with the file system
**  
**/

#if !defined FILETOOLS_H
#define FILETOOLS_H

#ifdef WIN32
#pragma warning(disable:4786) 
#endif

#include "error.hpp"

#if defined WIN32

  static const char PATH_SEPARATOR = '\\';

#else

  static const char PATH_SEPARATOR = '/';

#endif

//! This is the namespace of the blib library
namespace blib {

//! Specifies the type of an item in the file system: file, directory or any type
enum
{
  FILE = 1,
  DIR,
  ANY
};

//! Specifies the type of time attribute of an item in the file system, used by GetTime()
enum
{
  CREATED = 0,
  MODIFIED,
  ACCESSED
};

//! Error codes used by fileTools
enum
{
  FT_ERR_NO_ERROR = 0,
  FT_ERR_GETSTAT,
  FT_ERR_GETTIME,
  FT_ERR_GETTIME_NOEXIST,
  FT_ERR_GETSIZE,
  FT_ERR_GETSIZE_NOEXIST,
  FT_ERR_GETFREESPACEONVOLUME,
  FT_ERR_GETTOTALSPACEONVOLUME,
  FT_ERR_CHANGEDIR,
  FT_ERR_CHANGEDIR_NOEXIST,
  FT_ERR_CREATEDIR,
  FT_ERR_CREATEDIR_NOEXISTPARENT,
  FT_ERR_COPY_NOEXISTSOURCE,
  FT_ERR_COPY_NOEXISTPARENTDEST,
  FT_ERR_COPY_DESTNEWER,
  FT_ERR_COPY_CREATEDIR1,
  FT_ERR_COPY_CREATEDIR2,
  FT_ERR_COPY_GETITEMS_DIRS,
  FT_ERR_COPY_GETITEMS_FILES,
  FT_ERR_COPY_COPYFILE1,
  FT_ERR_COPY_COPYFILE2,
  FT_ERR_COPY,
  FT_ERR_RENAME,
  FT_ERR_RENAME_NOEXIST,
  FT_ERR_REMOVE,
  FT_ERR_REMOVE_NOEXIST,
  FT_ERR_REMOVE_GETITEMS,
  FT_ERR_REMOVE_REMOVE,
  FT_ERR_FINDFIRSTITEM_OPENDIR,
  FT_ERR_FINDNEXTITEM_ILLEGALCALL,
  FT_ERR_GETITEMS_NOEXISTSPARENT,
  FT_ERR_GETITEMS,
  FT_ERR_EXEC_EMPTYCOMMAND,
};


/*! Abstract base class representing a specification for files or directories. 
** Used by GetItems() to find items wich meet certain specs
*/
class Spec
{
public:
  virtual ~Spec() {};
  virtual bool ItemOK(const std::string& path) const = 0;
};


//! The item name must meet a wildcard specification
class NameSpec : public Spec
{
public:
#if defined WIN32
  NameSpec(const std::string& wildcard, bool caseSensitive=false);
#else
  NameSpec(const std::string& wildcard, bool caseSensitive=true);
#endif
  virtual bool ItemOK(const std::string& path) const;
protected:
  std::string wildcard_;
  bool caseSensitive_;
};


//! The item must be of a certain type, FILE, DIR or ANY
class TypeSpec : public Spec
{
public:
  TypeSpec(int type);
  virtual bool ItemOK(const std::string& path) const;
protected:
  int type_;
};


//! The item may not meet a given spec
class NotSpec : public Spec
{
public:
  virtual ~NotSpec();
  NotSpec(const Spec* spec);
  virtual bool ItemOK(const std::string& path) const;
protected:
  const Spec* spec_;
};


//! Abstract base class for combinations of multiple specs
/*! The specs are passed to MultiSpec through AddSpec()
**  Every spec added toMultiSpec must be heap-allocated.
**  MultiSpec takes over ownership of the specs and deletes them in its own destructor.
*/
class MultiSpec : public Spec
{
public:
  MultiSpec(const Spec* spec1=0, const Spec* spec2=0);
  virtual ~MultiSpec();
  void AddSpec(const Spec* spec);
  void Clear();
protected:
  typedef std::list<const Spec*> TSpecList;
  TSpecList specList_;
};


//! The item must meet all out of a list of specs
class AndSpec : public MultiSpec
{
public:
  AndSpec(const Spec* spec1=0, const Spec* spec2=0);
  virtual bool ItemOK(const std::string& path) const;
};


//! The item must meet at least one out of a list of specs
class OrSpec : public MultiSpec
{
public:
  OrSpec(const Spec* spec1=0, const Spec* spec2=0);
  virtual bool ItemOK(const std::string& path) const;
};


//! The item must meet just one out of a list of specs
class XorSpec : public MultiSpec
{
public:
  XorSpec(const Spec* spec1=0, const Spec* spec2=0);
  virtual bool ItemOK(const std::string& path) const;
};


//! Helper function to retrieve the properties of a given item
/*! The properties are cached by GetStat(). If the previous call of GetStat() had the same file as argument, 
**  the cached properties are returned, unless noCache is true. This function pays off anywhere it is likely
**  that the properties of the same item will be repeatedly retrieved, e.g. in the ItemOK() method of a TMultiSpec.
*/
const struct stat* GetStat(const std::string& itemName, bool noCache=false);


//! Get current working directory
/*! \return a string containing the full path to the current working directory
*/
std::string GetCurrentDir();


//! Check whether a given item exists
/*! \param path a string containing the path to the given item, either full or relative to the current working directory
**  \param type specifies the type of item of which you want to test the existence: FILE, DIR or ANY
**  \return true if the file exists, false otherwise
*/
bool Exists(const std::string& path, int type=ANY);


//! Get the time when a given item was last modified
/*! \param fileName a string containing the path to the given item, either full or relative to the current working directory
** \param time a pointer to the structure which will hold the requested time
** \param timeType specifies the kind of time you want to know, the time the item was created (CREATED), 
**                 last modified (MODIFIED) or last accessed (ACCESSED).
** \return true if the function succeeded, false otherwise
*/
bool GetTime(const std::string& path, time_t* time, int timeType=MODIFIED);


//! Get the size of a given item (file or directory)
/*! \param path a string containing the path to the given item, either full or relative to the current working directory
** \param bytes a pointer to an argument which will hold the size in bytes when the function returns, with a maximum of 1024^3-1
** \param gigaBytes a pointer to an argument which will hold the size in gigaBytes when the function returns.
**         (bytes + 1024^3*gigaBytes) will give you the total size in bytes.
** \return true if the function succeeded, false otherwise
*/
bool GetSize(const std::string& path, unsigned long* bytes, unsigned long* gigaBytes=0);


//! Get the free space of a volume
/*! \param path a string containing a path to a file or directory on the volume, either full or relative to the current working directory
** \param bytes a pointer to an argument which will hold the free space in bytes when the function returns, with a maximum of 1024^3-1
** \param gigaBytes a pointer to an argument which will hold the free space in gigaBytes when the function returns.
**         (bytes + 1024^3*gigaBytes) will give you the total free space in bytes.
** \return true if the function succeeded, false otherwise
*/
bool GetFreeSpaceOnVolume(const std::string& path, unsigned long* bytes, unsigned long* gigaBytes=0);


//! Get the total space of a volume
/*! \param path a string containing a path to a file or directory on the volume, either full or relative to the current working directory
** \param bytes a pointer to an argument which will hold the total space in bytes when the function returns, with a maximum of 1024^3-1
** \param gigaBytes a pointer to an argument which will hold the total space in gigaBytes when the function returns.
**         (bytes + 1024^3*gigaBytes) will give you the total space in bytes.
** \return true if the function succeeded, false otherwise
*/
bool GetTotalSpaceOnVolume(const std::string& path, unsigned long* bytes, unsigned long* gigaBytes=0);


//! Changes the current working directory
/*! \param dir a string containing the path to the desired new working directory, either full or relative to the current working directory
** \return true if the function succeeded, false otherwise
*/
bool ChangeDir(const std::string& dir);


//! Creates a new directory
/*! \param dir a string containing the path to the directory to be created, either full or relative to the current working directory
** \return true if the function succeeded, false otherwise
*/
bool CreateDir(const std::string& dir);


//! Copies an item (file or directory)
/*! \param sourcePath a string containing the path to the source item, either full or relative to the current working directory
** \param destPath a string containing the path to the destination item, either full or relative to the current working directory
** \param ifNewer when true, then the item is copied only if the source is newer than the destination
** \return true if the function succeeded, false otherwise
*/
bool Copy(const std::string& sourcePath, const std::string& destPath, bool ifNewer=true);


//! Renames an item (file or directory)
/*! \param origPath a string containing the path to the original item, either full or relative to the current working directory
** \param newPath a string containing the path to the renamed item, either full or relative to the current working directory
** \return true if the function succeeded, false otherwise
*/
bool Rename(const std::string& origPath,const std::string& newPath);


//! Removes an item (file or directory)
/*! \param path a string containing the path to the item, either full or relative to the current working directory
** \return true if the function succeeded, false otherwise
*/
bool Remove(const std::string& path);


//! Searches for an item (file or directory) meeting a given specification
/*! \param itemFound a pointer to the name of the item found, if any. This argument may be set to 0 by the user if he is only 
**                 interested in the mere existence of the item and not in its name.
** \param spec an object representing the specification the item has to meet. See documentation of Spec for more details.
** \return true if an item was found, false otherwise
*/
bool FindFirstItem(std::string* itemFound, const Spec& spec);


//! Searches for another item (file or directory) meeting the same specification given in a preceding call to GetFirstItem()
/*! \param itemFound a pointer to the name of the item found, if any. This argument may be set to 0 by the user if he is only 
**                 interested in the mere existence of the item and not in its name.
** \return true if another item was found, false otherwise
*/
bool FindNextItem(std::string* itemFound);


//! Ends a FindFirstItem(() / FindNextItem() session and releases associated resources
void FindClose();


//! Searches for items (files or directories) meeting a given specification
/*! \param itemList a list of strings which will be appended with the full paths to the items meeting the spec.
**                 This argument may be set to 0 by the user if he is only interested in the mere existence
**                 of items and not in their path.
** \param spec an object representing the specification the items have to meet. See documentation of Spec for more details.
** \param parentPath the search path, either full or relative to the current working directory
** \param recursive a flag indicating if all subdirectories of the search path have to be searched as well
** \return true if the function could be completed successfully, false otherwise
*/
bool GetItems(StringList& itemList, const Spec& spec, const std::string& parentPath, bool recursive=false);


//! Checks whether a given string is a root path
/*! \param path the string to be tested
** \return true if the string is a root path, false otherwise
*/
bool IsRoot(const std::string& path);


//! Checks whether a given string is a valid full path name
/*! \param path the string to be tested
** \return true if the string is a valid full path, false otherwise
*/
bool IsFullPath(const std::string& path);


//! Returns the full path version of a given path
/*! \param path the string to be converted, either full or relative to the current working directory. 
** \return the full path, equivalent to the given one
*/
std::string GetFullPath(const std::string& path);


//! Returns a path relative to another path
/*! \param item a path to an item, either full or relative to the current working directory
** \param refDir the path to a reference directory, either full or relative to the current working directory
** \return the path to the item given in the first argument, but relative to the directory given in the second argument.
*/
std::string GetRelativePath(const std::string& item, const std::string& refDir);


//! Gets the full path to the program's executable
/*! \return the full path to the program's executable
*/
std::string GetAppPath();


//! Returns a URL-save version of the path
/*! For now (21/1/2003), this function simply replaces backslashes with forward slashes.
** TODO: In the future it could convert the string to URL encoding as a second step.
** \param path the path to be made URL-save
** \return a URL-save version of the path
*/
std::string GetURLSavePath(const std::string& path);


//! Removes all redundancies out of a given path
/*! \param redundantPath a path to a file or directory, either full or relative to the current working directory, and possibly containing redundancies 
** \return an equivalent path to the given one, but stripped from all redundancies. For example, "./subdir/../subdir" will be converted to "subdir"
*/
std::string EliminateRedundancies(const std::string& redundantPath);


//! Determines the parent directory of a given item
/*! \param path a path to a file or directory, either full or relative to the current working directory
** \return the full path to the parent directory of the given item.
**  If the given item has no parent (because it's the root directory), the item itself is returned.
*/
std::string GetParentPath(const std::string& path);


//! Strips all path info off an item
/*! \param path a path to a file or directory, either full or relative to the current working directory
** \return the name of the file or directory, without any path info
*/
std::string GetItemName(const std::string& path);


//! Splits a filename into its constituting parts
/*! \param fullFileName a file name, complete with its extension
** \param fileName upon return, this will be the file name but without its extension
** \param extension upon return, this will be the file name's extension
*/
void SplitFileName(const std::string& fullFileName, std::string* fileName, std::string* extension=0);


//! Splits a path into its constituting parts
/*! \param path the string to be splitted
** \param stringList the container to which the found tokens will be added
*/
void SplitPath(const std::string& path, StringList& stringList);


//! Concats strings into a path
/*! \param stringList the container holding the strings
** \return a string containing all strings from stringList, separated by PATH_SEPARATOR
*/
std::string ConcatPath(const StringList& stringList);


//! Executes another program in a child thread
/*! \param command a string containing the command, complete with arguments
** \param exitCode if nonzero, exitCode will point to the exit code returned by the command
** \return true if the call succeeded, false otherwise
** The function waits for the child thread to terminate
*/
//bool Exec(const std::string& command, int* exitCode=0);


//! Returns the error stack of FileTools
ErrorStack* GetErrorStackFileTools();


} /* namespace blib */ 


#endif /* FILETOOLS_H */ 
