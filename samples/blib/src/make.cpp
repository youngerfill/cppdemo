
#ifdef WIN32
#pragma warning(disable:4786) 
#endif

#include <iostream>

#include "FileTools.hpp"
#include "make.hpp"

using namespace std;
using namespace blib;
using namespace blib::make;

////////////////////////
// SystemCommand  //
////////////////////////
//
SystemCommand::SystemCommand(const string& systemCommand)
{
  systemCommand_ = systemCommand;
}

//
SystemCommand::~SystemCommand()
{
}

//
void SystemCommand::Execute()
{
  system(systemCommand_.c_str());
}

//
void SystemCommand::SetSystemCommand(const string& systemCommand)
{
  systemCommand_ = systemCommand;
}

//
std::string SystemCommand::GetSystemCommand()
{
  return systemCommand_;
}


//////////////////
// Element  //
//////////////////
//
Element::Element(Command* command)
{
	command_ = command;
}

//
Element::~Element()
{
	delete command_;
}

//
Command* Element::GetCommand()
{
	return command_;
}

//
void Element::SetCommand(Command* command)
{
	command_ = command;
}

//
void Element::AddPrerequisite(Element* element)
{
	prereqList_.push_back(element);
}

//
void Element::RemovePrerequisite(Element* element)
{
	prereqList_.remove(element);
}

//	
bool Element::Make(bool force)
{
	for ( TPrereqList::iterator iter = prereqList_.begin(); iter != prereqList_.end(); iter++ )
		(*iter)->Make(force);
		
	if (IsOutDated(force))
		if (command_)
			command_->Execute();

  return true;
}


//////////////////////
// FileElement  //
//////////////////////
//
FileElement::FileElement(const std::string& fileName, Command* command)
:Element(command)
{
	fileName_ = fileName;
}

//
FileElement::~FileElement()
{
}

//
void FileElement::SetFileName(const std::string& fileName)
{
	fileName_ = fileName;
}
	
//
std::string FileElement::GetFileName()
{
	return fileName_;
}
	
//
bool FileElement::IsOutDated(bool force)
{
  if (!command_)
    return false;

	if ( (Exists(fileName_)) && (!force) )
  {
  	time_t timeChanged;
    GetTime(fileName_, &timeChanged, MODIFIED);

    TPrereqList::iterator iter = prereqList_.begin();
	  for ( ; iter != prereqList_.end(); iter++ )
		  if ((*iter)->IsChanged(timeChanged))
        break;

    if (iter == prereqList_.end())
      return false;
  }
			
  cout << "Outdated : " << fileName_ << endl;
  return true;
}

//
bool FileElement::IsChanged(time_t timeChangedTarget)
{
	time_t timeChanged;
  GetTime(fileName_, &timeChanged, MODIFIED);

	return ( timeChanged > timeChangedTarget );
}


