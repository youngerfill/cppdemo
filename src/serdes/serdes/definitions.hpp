
#if !defined DEFINITIONS_HPP
#define DEFINITIONS_HPP

#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include <string>
#include <vector>
#include <list>
#include <map>

namespace blib {


typedef std::vector<int> IntVector;
typedef std::vector<std::string> StringVector;

typedef std::list<int> IntList;
typedef std::list<std::string> StringList;

typedef std::map<int, std::string> IntStringMap;
typedef std::map<std::string, std::string> StringStringMap;

#define BLIB_GLOBALINIT_BEGIN(blockID) class GLOBALINITCLASS##blockID {public: GLOBALINITCLASS##blockID(){
#define BLIB_GLOBALINIT_END(blockID) }} GLOBALINITOBJECT##blockID;

#define BLIB_GLOBALEXIT_BEGIN(blockID) class GLOBALEXITCLASS##blockID {public: ~GLOBALEXITCLASS##blockID(){
#define BLIB_GLOBALEXIT_END(blockID) }} GLOBALINST##blockID;

} /* namespace blib  */

#if defined _MSC_VER

#if defined max
#undef max
#endif /* max */

#if defined min
#undef min
#endif /* min */

namespace std
{
  template<class T> inline
	  const T& max(const T& a, const T& b)
	  {return (a < b ? b : a); }
  template<class T, class TP> inline
	  const T& max(const T& a, const T& b, TP p)
	  {return (p(a, b) ? b : a); }

  template<class T> inline
	  const T& min(const T& a, const T& b)
	  {return (b < a ? b : a); }
  template<class T, class TP> inline
	  const T& min(const T& a, const T& b, TP p)
	  {return (p(b, a) ? b : a); }
}

#endif /* _MSC_VER */

#endif /* DEFINITIONS_HPP */

