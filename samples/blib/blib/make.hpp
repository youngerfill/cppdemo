/*! \file make.hpp
** \date 19 may 2004
** \author Bert Godderis
** \copyright (C) 2004 Bert Godderis
** \license wxWindows Library License : http://opensource.org/licenses/wxwindows
** 
** \brief Declares a set of classes that constitute a "make" system (like GNU make),
** with rules, targets, commands and prerequisites.
** Targets, rules and prerequisites are represented by the Element class hierarchy,
** and commands by the Command hierarchy.
**/

#if !defined MAKE_HPP
#define MAKE_HPP

#include <list>
#include <ctime>


namespace blib {

namespace make {

//! Abstract base class representing the command needed to build a target
class Command
{
public:
  virtual ~Command(){};
  virtual void Execute()=0;
};

//! Class representing a system command to build a target file
class SystemCommand : public Command
{
public:
  SystemCommand(const std::string& systemCommand);
  virtual ~SystemCommand();
  virtual void Execute();

  void SetSystemCommand(const std::string& systemCommand);
  std::string GetSystemCommand();

protected:
  std::string systemCommand_;
};

//! Abstract base class representing a node in the make system
/*!
** The methods IsOutDated() and IsChanged() must be overriden in the derived classes.
** An Element can have other Elements as prerequisites
** An Element with a Command associated to it corresponds to the "rule" concept from 
** the makefile terminology.
** An Element without a Command is only meaningful as a leaf node in the make system.
** The target associated to an Element is specified in the classes derived from Element.
**/
class Element
{
public:
	Element(Command* command=0);
	virtual ~Element();
	
	void SetCommand(Command* command);
	Command* GetCommand();
	
	void AddPrerequisite(Element* element);
	void RemovePrerequisite(Element* element);
	
	virtual bool IsOutDated(bool force=false) = 0;
  virtual bool IsChanged(time_t timeChangedTarget=0) = 0;
	
	bool Make(bool force=false);
	
protected:
	typedef std::list<Element*> TPrereqList;
	TPrereqList prereqList_;
	
	Command* command_;
};


//! Class representing the most obvious type of Element: an Element whose target is a file.
class FileElement : public Element
{
public:
	FileElement(const std::string& fileName="", Command* command=0);
	virtual ~FileElement();
	
	void SetFileName(const std::string& fileName);
	std::string GetFileName();
	
	virtual bool IsOutDated(bool force=false);
	virtual bool IsChanged(time_t timeChangedTarget=0);
	
private:
	std::string fileName_;
};


} /* namespace make */ 

} /* namespace blib */ 

#endif /* MAKESYSTEM_HPP */ 
