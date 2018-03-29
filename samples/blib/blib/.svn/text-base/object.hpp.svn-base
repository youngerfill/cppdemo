/*! \file  object.hpp
** \date 	29 june 2002
** \author	Bert Godderis
** \copyright (C) 2003 Bert Godderis
** \license wxWindows Library License : http://opensource.org/licenses/wxwindows
**
** \brief Declares Object, a base class for every class having a Clone() 
** method and Subject/Observer functionality
*/

#if !defined OBJECT_HPP
#define OBJECT_HPP

#ifdef WIN32
#pragma warning(disable:4786) 
#endif

namespace blib {

class Object
{
public:
  Object();
  virtual ~Object();

  void Observe(Object* subject);
  void StopObserving(Object* subject);
  void Notify();
  void Update(Object* changedSubject);

  virtual Object* Clone();

protected:

};

} /* namespace blib  */ 

#endif /* OBJECT_HPP */ 
