
#include "SerDesPlanB.hpp"

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
}
    
//-----------------------------------------------
bool SerDesOutPlanB::Write(const std::string& name, const short& object)
{
  *pStream_ << indentString_ << name << " = " << object << std::endl;
}
    
//-----------------------------------------------
bool SerDesOutPlanB::Write(const std::string& name, const int& object)
{
  *pStream_ << indentString_ << name << " = " << object << std::endl;
}
    
//-----------------------------------------------
bool SerDesOutPlanB::Write(const std::string& name, const long& object)
{
  *pStream_ << indentString_ << name << " = " << object << std::endl;
}
    
//-----------------------------------------------
bool SerDesOutPlanB::Write(const std::string& name, const float& object)
{
  *pStream_ << indentString_ << name << " = "  << object << std::endl;
}
  
//-----------------------------------------------
bool SerDesOutPlanB::Write(const std::string& name, const double& object)
{
  *pStream_ << indentString_ << name << " = "  << object << std::endl;
}
  
//-----------------------------------------------
bool SerDesOutPlanB::Write(const std::string& name, const unsigned char& object)
{
  *pStream_ << indentString_ << name << " = "  << object << std::endl;
}
  
//-----------------------------------------------
bool SerDesOutPlanB::Write(const std::string& name, const unsigned short& object)
{
  *pStream_ << indentString_ << name << " = "  << object << std::endl;
}
  
//-----------------------------------------------
bool SerDesOutPlanB::Write(const std::string& name, const unsigned int& object)
{
  *pStream_ << indentString_ << name << " = "  << object << std::endl;
}
  
//-----------------------------------------------
bool SerDesOutPlanB::Write(const std::string& name, const unsigned long& object)
{
  *pStream_ << indentString_ << name << " = "  << object << std::endl;
}
  
//-----------------------------------------------
bool SerDesOutPlanB::Write(const std::string& name, const std::string& object)
{
  *pStream_ << indentString_ << name << " = '"  << object << "'" << std::endl;
}  
  
//-----------------------------------------------
bool SerDesOutPlanB::WriteHead(const std::string& name, const std::string& className)
{
  *pStream_ << indentString_ << name;
  *pStream_  << " = object(" << className << ")" << std::endl;
  *pStream_  << indentString_ << "{" << std::endl;
  IncIndent();
}

//-----------------------------------------------
bool SerDesOutPlanB::WriteHeadPoly(const std::string& name, const std::string& className, unsigned long id)
{
  *pStream_ << indentString_ << name;
  *pStream_  << " = object(" << className << "," << id << ")" << std::endl;
  *pStream_  << indentString_ << "{" << std::endl;
  IncIndent();
}
  
//-----------------------------------------------
bool SerDesOutPlanB::WriteHeadArray(const std::string& name, std::size_t objectSize)
{
  *pStream_ << indentString_ << name;
  *pStream_  << " = array[" << objectSize << "]" << std::endl;
  *pStream_  << indentString_ << "{" << std::endl;
  IncIndent();
}

//-----------------------------------------------
bool SerDesOutPlanB::WriteHeadMap(const std::string& name, std::size_t objectSize)
{
  *pStream_ << indentString_ << name;
  *pStream_  << " = map[" << objectSize << "]" << std::endl;
  *pStream_  << indentString_ << "{" << std::endl;
  IncIndent();
}

//-----------------------------------------------
bool SerDesOutPlanB::WriteTail(const std::string& name)
{
  DecIndent();
  *pStream_ << indentString_ << "}" << std::endl;
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

