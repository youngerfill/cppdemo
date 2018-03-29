#if !defined SERDESPLANB_HPP
#define SERDESPLANB_HPP

#include "serdes.hpp"

////////////////////
// SerDesOutPlanB
////////////////////

struct SerDesOutPlanB : public SerDesOutStream
{
  SerDesOutPlanB(std::ostream& outstream);

  virtual bool Write(const std::string& name, const char& object);
  virtual bool Write(const std::string& name, const short& object);
  virtual bool Write(const std::string& name, const int& object);
  virtual bool Write(const std::string& name, const long& object);
  virtual bool Write(const std::string& name, const float& object);
  virtual bool Write(const std::string& name, const double& object);
  virtual bool Write(const std::string& name, const unsigned char& object);
  virtual bool Write(const std::string& name, const unsigned short& object);
  virtual bool Write(const std::string& name, const unsigned int& object);
  virtual bool Write(const std::string& name, const unsigned long& object);
  virtual bool Write(const std::string& name, const std::string& object);

  virtual bool WriteHead(const std::string& name, const std::string& className);
  virtual bool WriteHeadPoly(const std::string& name, const std::string& className, unsigned long id);
  virtual bool WriteHeadArray(const std::string& name, std::size_t objectSize);
  virtual bool WriteHeadMap(const std::string& name, std::size_t objectSize);
  virtual bool WriteTail(const std::string& name);
  virtual unsigned short GetIndentLevel() const;
  virtual const std::string& GetIndentString() const;

protected:
  void IncIndent();
  void DecIndent();

  unsigned short indentLevel_;
  std::string indentString_;
};


////////////////////
// SerDesInPlanB
////////////////////

struct SerDesInPlanB : public SerDesInStream
{
  SerDesInPlanB(std::istream& stream);

  virtual bool Read(const std::string& name, char& object);
  virtual bool Read(const std::string& name, short& object);
  virtual bool Read(const std::string& name, int& object);
  virtual bool Read(const std::string& name, long& object);
  virtual bool Read(const std::string& name, float& object);
  virtual bool Read(const std::string& name, double& object);
  virtual bool Read(const std::string& name, unsigned char& object);
  virtual bool Read(const std::string& name, unsigned short& object);
  virtual bool Read(const std::string& name, unsigned int& object);
  virtual bool Read(const std::string& name, unsigned long& object);
  virtual bool Read(const std::string& name, std::string& object);


  virtual unsigned short GetIndentLevel() const;
  virtual const std::string& GetIndentString() const;

protected:
  void IncIndent();
  void DecIndent();

//  bool ReadValueString(const std::string& name, std::string& valueString);

  unsigned short indentLevel_;
  std::string indentString_;
};

#endif /* SERDESPLANB_HPP */

