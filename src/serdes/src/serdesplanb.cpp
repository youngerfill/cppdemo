
// TESTCODE
#include <iostream>
// END TESTCODE

#include "stringtools.hpp"
#include "serdesplanb.hpp"

using namespace blib;

// TESTCODE
using namespace std;
// END TESTCODE
////////////////////
// SerDesOutPlanB
////////////////////

//-----------------------------------------------
SerDesOutPlanB::SerDesOutPlanB(std::ostream& outstream) 
:SerDesOutStream(outstream)
{
  indentLevel_ = 0;
}

//-----------------------------------------------
void SerDesOutPlanB::IncIndent()
{
  indentLevel_++;
  indentString_ += "  ";
}
  
//-----------------------------------------------
void SerDesOutPlanB::DecIndent()
{
  if (indentLevel_)
  {
    indentLevel_--;
    indentString_.erase(indentLevel_*2, 2);
  }  
}  

//-----------------------------------------------
bool SerDesOutPlanB::Write(const std::string& name, const char& object)
{
  *pStream_ << indentString_ << name << " = " << object << std::endl;
  return true;
}
    
//-----------------------------------------------
bool SerDesOutPlanB::Write(const std::string& name, const short& object)
{
  *pStream_ << indentString_ << name << " = " << object << std::endl;
  return true;
}
    
//-----------------------------------------------
bool SerDesOutPlanB::Write(const std::string& name, const int& object)
{
  *pStream_ << indentString_ << name << " = " << object << std::endl;
  return true;
}
    
//-----------------------------------------------
bool SerDesOutPlanB::Write(const std::string& name, const long& object)
{
  *pStream_ << indentString_ << name << " = " << object << std::endl;
  return true;
}
    
//-----------------------------------------------
bool SerDesOutPlanB::Write(const std::string& name, const float& object)
{
  *pStream_ << indentString_ << name << " = "  << object << std::endl;
  return true;
}
  
//-----------------------------------------------
bool SerDesOutPlanB::Write(const std::string& name, const double& object)
{
  *pStream_ << indentString_ << name << " = "  << object << std::endl;
  return true;
}
  
//-----------------------------------------------
bool SerDesOutPlanB::Write(const std::string& name, const unsigned char& object)
{
  *pStream_ << indentString_ << name << " = "  << object << std::endl;
  return true;
}
  
//-----------------------------------------------
bool SerDesOutPlanB::Write(const std::string& name, const unsigned short& object)
{
  *pStream_ << indentString_ << name << " = "  << object << std::endl;
  return true;
}
  
//-----------------------------------------------
bool SerDesOutPlanB::Write(const std::string& name, const unsigned int& object)
{
  *pStream_ << indentString_ << name << " = "  << object << std::endl;
  return true;
}
  
//-----------------------------------------------
bool SerDesOutPlanB::Write(const std::string& name, const unsigned long& object)
{
  *pStream_ << indentString_ << name << " = "  << object << std::endl;
  return true;
}
  
//-----------------------------------------------
bool SerDesOutPlanB::Write(const std::string& name, const std::string& object)
{
  *pStream_ << indentString_ << name << " = '"  << object << "'" << std::endl;
  return true;
}  
  
//-----------------------------------------------
bool SerDesOutPlanB::WriteHead(const std::string& name, const std::string& className)
{
  *pStream_ << indentString_ << name;
  *pStream_  << " = object(" << className << ")" << std::endl;
  *pStream_  << indentString_ << "{" << std::endl;
  IncIndent();
  return true;
}

//-----------------------------------------------
bool SerDesOutPlanB::WriteHeadPoly(const std::string& name, const std::string& className, unsigned long id)
{
  *pStream_ << indentString_ << name;
  *pStream_  << " = object(" << className << "," << id << ")" << std::endl;
  *pStream_  << indentString_ << "{" << std::endl;
  IncIndent();
  return true;
}
  
//-----------------------------------------------
bool SerDesOutPlanB::WriteHeadArray(const std::string& name, std::size_t objectSize)
{
  *pStream_ << indentString_ << name;
  *pStream_  << " = array[" << objectSize << "]" << std::endl;
  *pStream_  << indentString_ << "{" << std::endl;
  IncIndent();
  return true;
}

//-----------------------------------------------
bool SerDesOutPlanB::WriteHeadMap(const std::string& name, std::size_t objectSize)
{
  *pStream_ << indentString_ << name;
  *pStream_  << " = map[" << objectSize << "]" << std::endl;
  *pStream_  << indentString_ << "{" << std::endl;
  IncIndent();
  return true;
}

//-----------------------------------------------
bool SerDesOutPlanB::WriteTail(const std::string& name)
{
  DecIndent();
  *pStream_ << indentString_ << "}" << std::endl;
  return true;
}

  
//-----------------------------------------------
unsigned short SerDesOutPlanB::GetIndentLevel() const
{
  return indentLevel_;
}
  
//-----------------------------------------------
const std::string& SerDesOutPlanB::GetIndentString() const
{
  return indentString_;
}

////////////////////
// SerDesInPlanB
////////////////////


//-----------------------------------------------
SerDesInPlanB::SerDesInPlanB(std::istream& instream) 
:SerDesInStream(instream)
{
  indentLevel_ = 0;
}

//-----------------------------------------------
void SerDesInPlanB::IncIndent()
{
  indentLevel_++;
  indentString_ += "  ";
}
  
//-----------------------------------------------
void SerDesInPlanB::DecIndent()
{
  if (indentLevel_)
  {
    indentLevel_--;
    indentString_.erase(indentLevel_*2, 2);
  }  
}  

//-----------------------------------------------
bool SerDesInPlanB::Read(const std::string& name, char& object)
{
//  *pStream_ << indentString_ << name << " = " << object << std::endl;
  return true;
}
    
//-----------------------------------------------
bool SerDesInPlanB::Read(const std::string& name, short& object)
{
//  *pStream_ << indentString_ << name << " = " << object << std::endl;
  return true;
}
    
//-----------------------------------------------
bool SerDesInPlanB::Read(const std::string& name, int& object)
{
  string valueString;
  if (ReadValueString(*pStream_, name, valueString))
  {
    object = atoi(valueString.c_str());
    return true;
  }  
  
  return false;
}
    
//-----------------------------------------------
bool SerDesInPlanB::Read(const std::string& name, long& object)
{
//  *pStream_ << indentString_ << name << " = " << object << std::endl;
  return true;
}
    
//-----------------------------------------------
bool SerDesInPlanB::Read(const std::string& name, float& object)
{
  string valueString;
  if (ReadValueString(*pStream_, name, valueString))
  {
    object = atof(valueString.c_str());
    return true;
  }
  else
  {
    cout << "hmm???" << endl;
  }  

  return false;  
}
  
//-----------------------------------------------
bool SerDesInPlanB::Read(const std::string& name, double& object)
{
//  *pStream_ << indentString_ << name << " = "  << object << std::endl;
  return true;
}
  
//-----------------------------------------------
bool SerDesInPlanB::Read(const std::string& name, unsigned char& object)
{
//  *pStream_ << indentString_ << name << " = "  << object << std::endl;
  return true;
}
  
//-----------------------------------------------
bool SerDesInPlanB::Read(const std::string& name, unsigned short& object)
{
 // *pStream_ << indentString_ << name << " = "  << object << std::endl;
  return true;
}
  
//-----------------------------------------------
bool SerDesInPlanB::Read(const std::string& name, unsigned int& object)
{
 // *pStream_ << indentString_ << name << " = "  << object << std::endl;
  return true;
}
  
//-----------------------------------------------
bool SerDesInPlanB::Read(const std::string& name, unsigned long& object)
{
 // *pStream_ << indentString_ << name << " = "  << object << std::endl;
  return true;
}
  
//-----------------------------------------------
bool SerDesInPlanB::Read(const std::string& name, std::string& object)
{
 //*pStream_ << indentString_ << name << " = '"  << object << "'" << std::endl;
  return true;
}  
  
/*  
//-----------------------------------------------
bool SerDesInPlanB::ReadHead(const std::string& name, const std::string& className)
{
  *pStream_ << indentString_ << name;
  *pStream_  << " = object(" << className << ")" << std::endl;
  *pStream_  << indentString_ << "{" << std::endl;
  IncIndent();
  return true;
}

//-----------------------------------------------
bool SerDesInPlanB::ReadHeadPoly(const std::string& name, const std::string& className, unsigned long id)
{
  *pStream_ << indentString_ << name;
  *pStream_  << " = object(" << className << "," << id << ")" << std::endl;
  *pStream_  << indentString_ << "{" << std::endl;
  IncIndent();
  return true;
}
  
//-----------------------------------------------
bool SerDesInPlanB::ReadHeadArray(const std::string& name, std::size_t objectSize)
{
  *pStream_ << indentString_ << name;
  *pStream_  << " = array[" << objectSize << "]" << std::endl;
  *pStream_  << indentString_ << "{" << std::endl;
  IncIndent();
  return true;
}

//-----------------------------------------------
bool SerDesInPlanB::ReadHeadMap(const std::string& name, std::size_t objectSize)
{
  *pStream_ << indentString_ << name;
  *pStream_  << " = map[" << objectSize << "]" << std::endl;
  *pStream_  << indentString_ << "{" << std::endl;
  IncIndent();
  return true;
}

//-----------------------------------------------
bool SerDesInPlanB::ReadTail(const std::string& name)
{
  DecIndent();
  *pStream_ << indentString_ << "}" << std::endl;
  return true;
}
*/
  
//-----------------------------------------------
unsigned short SerDesInPlanB::GetIndentLevel() const
{
  return indentLevel_;
}
  
//-----------------------------------------------
const std::string& SerDesInPlanB::GetIndentString() const
{
  return indentString_;
}

