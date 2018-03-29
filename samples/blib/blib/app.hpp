/*! \file app.hpp
** \date 12 jan 2003
** \author Bert Godderis
** \copyright (C) 2003 Bert Godderis
** \license wxWindows Library License : http://opensource.org/licenses/wxwindows
** 
** \brief Declares a simple application class
**  
**/

#if !defined APP_HPP
#define APP_HPP

#ifdef WIN32
#pragma warning(disable:4786) 
#endif

#include "definitions.hpp"

namespace blib {

class App
{
public:
  virtual ~App(){};
	int Go(int numargs=0, char **arglist=0)
  {
    if (arglist)
      for (int i=0; i<numargs; i++)
        arguments_.push_back(arglist[i]);

    if (Enter())
      Run();
    return Leave();
  }

protected:
	virtual bool Enter() = 0;
	virtual bool Run() = 0;
	virtual int Leave() = 0;

	StringVector arguments_;
};

} /* namespace blib  */ 


#define BLIB_MAIN(MYAPP) int main(int argc, char **argv) { MYAPP theApp; return theApp.Go(argc, argv); }


#endif /* APP_HPP */ 
