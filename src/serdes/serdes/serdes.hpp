#if !defined SERDES_HPP
#define SERDES_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>


////////////////////
// ClassName class
////////////////////

template <typename T>
struct ClassName {};

template <typename T>
const std::string& GetClassName(const T& object)
{
  return ClassName<T>::Value();
}

#define SD_MAKE_CLASSNAME(className) \
template <> \
struct ClassName<className> \
{ \
  static const std::string& Value() \
  { \
    static std::string theName = #className; \
    return theName; \
  } \
};

#define SD_DECLARE_ABSTRACT_CLASS \
virtual const std::string& GetClassName() const = 0; \
virtual unsigned long GetClassID() const = 0; \

#define SD_MAKE_BASECLASS(className) \
template<> \
struct TypeInfo<className> \
{ \
  enum { Category = POLYMORPH }; \
};


#define SD_DECLARE_CLASS(className,id) \
virtual const std::string& GetClassName() const \
{ \
  static std::string theName = #className; \
  return theName; \
} \
virtual unsigned long GetClassID() const \
{ \
  return id; \
}

//////////////////////////////
// TypeInfo traits class
//////////////////////////////

enum
{
  BUILTIN,
  NONBUILTIN,
  POLYMORPH,
  C_ARRAY,
  NONMAP,
  MAP,
};

template<typename T>
struct TypeInfo
{ enum { Category = NONBUILTIN }; };

struct TypeInfoBuiltIn
{ enum { Category = BUILTIN }; };

template<> struct TypeInfo<unsigned int> : public TypeInfoBuiltIn {};
template<> struct TypeInfo<int> : public TypeInfoBuiltIn {};
template<> struct TypeInfo<unsigned long> : public TypeInfoBuiltIn {};
template<> struct TypeInfo<long> : public TypeInfoBuiltIn {};
template<> struct TypeInfo<unsigned short> : public TypeInfoBuiltIn {};
template<> struct TypeInfo<short> : public TypeInfoBuiltIn {};
template<> struct TypeInfo<unsigned char> : public TypeInfoBuiltIn {};
template<> struct TypeInfo<char> : public TypeInfoBuiltIn {};
template<> struct TypeInfo<float> : public TypeInfoBuiltIn {};
template<> struct TypeInfo<double> : public TypeInfoBuiltIn {};
template<> struct TypeInfo<std::string> : public TypeInfoBuiltIn {};

template<typename T>
struct TypeInfo<T[]>
{ enum { Category = C_ARRAY }; };

struct TypeInfoNonMap
{ enum { Category = NONMAP }; };

template<typename T> struct TypeInfo< std::vector<T> > : public TypeInfoNonMap {};
template<typename T> struct TypeInfo< std::deque<T> > : public TypeInfoNonMap {};
template<typename T> struct TypeInfo< std::list<T> > : public TypeInfoNonMap {};
template<typename T> struct TypeInfo< std::set<T> > : public TypeInfoNonMap {};
template<typename T> struct TypeInfo< std::multiset<T> > : public TypeInfoNonMap {};

template<typename TKey, typename TValue>
struct TypeInfo< std::map<TKey, TValue> >
{ enum { Category = MAP }; };

template<typename TKey, typename TValue>
struct TypeInfo< std::multimap<TKey, TValue> >
{ enum { Category = MAP }; };


//////////////////////
// SerDes base class
//////////////////////

struct SerDesOutBase
{
  virtual bool Write(const std::string& name, const char& object) = 0;
  virtual bool Write(const std::string& name, const short& object) = 0;
  virtual bool Write(const std::string& name, const int& object) = 0;
  virtual bool Write(const std::string& name, const long& object) = 0;
  virtual bool Write(const std::string& name, const float& object) = 0;
  virtual bool Write(const std::string& name, const double& object) = 0;
  virtual bool Write(const std::string& name, const unsigned char& object) = 0;
  virtual bool Write(const std::string& name, const unsigned short& object) = 0;
  virtual bool Write(const std::string& name, const unsigned int& object) = 0;
  virtual bool Write(const std::string& name, const unsigned long& object) = 0;
  virtual bool Write(const std::string& name, const std::string& object) = 0;

  virtual bool WriteHead(const std::string& name, const std::string& className) = 0;
  virtual bool WriteHeadPoly(const std::string& name, const std::string& className, unsigned long id) = 0;
  virtual bool WriteHeadArray(const std::string& name, std::size_t objectSize) = 0;
  virtual bool WriteTail(const std::string& name) = 0;
};

struct SerDesInBase
{

  virtual bool Read(const std::string& name, char& object) = 0;
  virtual bool Read(const std::string& name, short& object) = 0;
  virtual bool Read(const std::string& name, int& object) = 0;
  virtual bool Read(const std::string& name, long& object) = 0;
  virtual bool Read(const std::string& name, float& object) = 0;
  virtual bool Read(const std::string& name, double& object) = 0;
  virtual bool Read(const std::string& name, unsigned char& object) = 0;
  virtual bool Read(const std::string& name, unsigned short& object) = 0;
  virtual bool Read(const std::string& name, unsigned int& object) = 0;
  virtual bool Read(const std::string& name, unsigned long& object) = 0;
  virtual bool Read(const std::string& name, std::string& object) = 0;

};


/////////////////////////
// SerDesOutStream class
/////////////////////////

struct SerDesOutStream : public SerDesOutBase
{
  SerDesOutStream(std::ostream& outstream)
  :pStream_(&outstream)
  {}

  std::ostream& GetStream()
  {
    // TODO : check if 0
    return *pStream_;
  }

  // TODO : Set I/O Stream methods ???

protected:
  std::ostream* pStream_;
};

////////////////////////
// SerDesInStream class
////////////////////////

struct SerDesInStream : public SerDesInBase
{
  SerDesInStream(std::istream& instream)
  :pStream_(&instream)
  {}

  std::istream& GetStream()
  {
    // TODO : check if 0
    return *pStream_;
  }

  // TODO : Set I/O Stream methods ???

protected:
  std::istream* pStream_;
};


/////////////////
// SDResolver
/////////////////

template <class SerDes, typename T, int>
struct SDResolver
{
};


template <class SerDes, typename T>
struct SDResolver<SerDes, T, BUILTIN>
{
  static bool WriteHead(SerDes& serdes, const std::string& name, const T& object)
  {
      return true;
  }

  static bool WriteBody(SerDes& serdes, const std::string& name, const T& object)
  {
    return serdes.Write(name, object);
  }

  static bool WriteTail(SerDes& serdes, const std::string& name, const T& object)
  {
      return true;
  }

  static bool ReadHead(SerDes& serdes, const std::string& name, T& object)
  {
      return true;
  }

  static bool ReadBody(SerDes& serdes, const std::string& name, T& object)
  {
    return serdes.Read(name, object);
  }

  static bool ReadTail(SerDes& serdes, const std::string& name, T& object)
  {
      return true;
  }
};

template <class SerDes, typename T>
struct SDResolver<SerDes, T, NONBUILTIN>
{
  static bool WriteHead(SerDes& serdes, const std::string& name, const T& object)
  {
      return serdes.WriteHead(name, GetClassName(object));
  }

  static bool WriteTail(SerDes& serdes, const std::string& name, const T& object)
  {
      return serdes.WriteTail(name);
  }
};

template <class SerDes, typename T>
struct SDResolver<SerDes, T, POLYMORPH>
{
  static bool WriteHead(SerDes& serdes, const std::string& name, const T& object)
  {
      return serdes.WriteHeadPoly(name, object.GetClassName(), object.GetClassID());
  }

  static bool WriteBody(SerDes& serdes, const std::string& name, const T& object)
  {
    return object.Write(serdes, name);
  }

  static bool WriteTail(SerDes& serdes, const std::string& name, const T& object)
  {
      return serdes.WriteTail(name);
  }
};

template <class SerDes, typename T>
struct SDResolver< SerDes, T, NONMAP >
{
  static bool WriteHead(SerDes& serdes, const std::string& name, const T& object)
  {
    return serdes.WriteHeadArray(name, object.size());
  }

  static bool WriteBody(SerDes& serdes, const std::string& name, const T& object)
  {
    std::ostringstream auxStream;
    typename T::const_iterator iter;
    size_t index = 0;
    for (iter = object.begin(); iter!=object.end(); iter++, index++)
    {
      auxStream.str("");
      auxStream << "v[" << index << "]";
      WriteObject(serdes, auxStream.str(), *iter);
    }

    return true;
  }

  static bool WriteTail(SerDes& serdes, const std::string& name, const T& object)
  {
    return serdes.WriteTail(name);
  }
};


template <class SerDes, typename T>
struct SDResolver< SerDes, T, C_ARRAY >
{
  static bool WriteHead(SerDes& serdes, const std::string& name, const T& object)
  {
    return serdes.WriteHeadArray(name, sizeof(object));
  }

  static bool WriteBody(SerDes& serdes, const std::string& name, const T& object)
  {
    std::ostringstream auxStream;
    for (int i = 0; i < sizeof(object); i++)
    {
      auxStream.str("");
      auxStream << "v[" << i << "]";
      WriteObject(serdes, auxStream.str(), object[i]);
    }

    return true;
  }

  static bool WriteTail(SerDes& serdes, const std::string& name, const T& object)
  {
    return serdes.WriteTail(name);
  }
};


template <class SerDes, typename T>
struct SDResolver< SerDes, T, MAP >
{
  static bool WriteHead(SerDes& serdes, const std::string& name, const T& object)
  {
    return serdes.WriteHeadMap(name, object.size());
  }

  static bool WriteBody(SerDes& serdes, const std::string& name, const T& object)
  {
    std::ostringstream auxStream;
    typename T::const_iterator iter;
    size_t index = 0;
    for (iter = object.begin(); iter!=object.end(); iter++, index++)
    {
      auxStream.str("");
      auxStream << "k[" << index << "]";
      WriteObject(serdes, auxStream.str(), iter->first);
      auxStream.str("");
      auxStream << "d[" << index << "]";
      WriteObject(serdes, auxStream.str(), iter->second);
    }

    return true;
  }

  static bool WriteTail(SerDes& serdes, const std::string& name, const T& object)
  {
    return serdes.WriteTail(name);
  }
};


///////////////////
// Free functions
///////////////////

//////// Write functions ////////


template <class SerDes, typename T>
bool WriteHead(SerDes& serdes, const std::string& name, const T& object)
{
  return SDResolver<SerDes, T, TypeInfo<T>::Category >::WriteHead(serdes, name, object);
}

template <class SerDes, typename T>
bool WriteBody(SerDes& serdes, const std::string& name, const T& object)
{
  return SDResolver<SerDes, T, TypeInfo<T>::Category >::WriteBody(serdes, name, object);
}

template <class SerDes, typename T>
bool WriteTail(SerDes& serdes, const std::string& name, const T& object)
{
  return SDResolver<SerDes, T, TypeInfo<T>::Category >::WriteTail(serdes, name, object);
}

template <class SerDes, typename T>
bool WriteObject(SerDes& serdes, const std::string& name, T* object)
{
  WriteHead(serdes, name, *object);
  WriteBody(serdes, name, *object);
  WriteTail(serdes, name, *object);

  return true;
}

template <class SerDes, typename T>
bool WriteObject(SerDes& serdes, const std::string& name, const T* object)
{
  WriteHead(serdes, name, *object);
  WriteBody(serdes, name, *object);
  WriteTail(serdes, name, *object);

  return true;
}


template <class SerDes, typename T>
bool WriteObject(SerDes& serdes, const std::string& name, const T& object)
{
  WriteHead(serdes, name, object);
  WriteBody(serdes, name, object);
  WriteTail(serdes, name, object);

  return true;
}


//////// Read functions ////////


template <class SerDes, typename T>
bool ReadHead(SerDes& serdes, const std::string& name, T& object)
{
  return SDResolver<SerDes, T, TypeInfo<T>::Category >::ReadHead(serdes, name, object);
}

template <class SerDes, typename T>
bool ReadBody(SerDes& serdes, const std::string& name, T& object)
{
  return SDResolver<SerDes, T, TypeInfo<T>::Category >::ReadBody(serdes, name, object);
}

template <class SerDes, typename T>
bool ReadTail(SerDes& serdes, const std::string& name, T& object)
{
  return SDResolver<SerDes, T, TypeInfo<T>::Category >::ReadTail(serdes, name, object);
}

template <class SerDes, typename T>
bool ReadObject(SerDes& serdes, const std::string& name, T& object)
{
  ReadHead(serdes, name, object);
  ReadBody(serdes, name, object);
  ReadTail(serdes, name, object);

  return true;
}


#endif /* SERDES_HPP */

