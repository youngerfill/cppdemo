
#ifdef WIN32
#pragma warning(disable:4786) 
#endif

#include <iostream>

#include "make.hpp"
#include "FileTools.hpp"

using namespace std;
using namespace blib;
using namespace blib::make;

int main()
{
  ChangeDir("../data");

  cout << "Current dir = " << GetCurrentDir() << endl;

  FileElement* goal = new FileElement("prog.exe", new SystemCommand("g++ part1.o part2.o main.o -o prog.exe"));

  string clOptions = "-I\"C:\\Program Files\\Dev-Cpp\\include\\C++\" -I\"C:\\Program Files\\Dev-Cpp\\include\\c++\\mingw32\"";

  FileElement* objectFile1;
  FileElement* objectFile2;
  FileElement* objectFile3;
  FileElement* headerFile1;
  FileElement* headerFile2;

  goal->AddPrerequisite( objectFile1 = new FileElement("part1.o", new SystemCommand("g++ -c part1.cpp " + clOptions)) );
  goal->AddPrerequisite( objectFile2 = new FileElement("part2.o", new SystemCommand("g++ -c part2.cpp " + clOptions)) );
  goal->AddPrerequisite( objectFile3 = new FileElement("main.o", new SystemCommand("g++ -c main.cpp " + clOptions)) );

  objectFile1->AddPrerequisite( new FileElement("part1.cpp") );
  objectFile1->AddPrerequisite( headerFile1 = new FileElement("part1.hpp") );

  objectFile2->AddPrerequisite( new FileElement("part2.cpp") );
  objectFile2->AddPrerequisite( headerFile2 = new FileElement("part2.hpp") );

  objectFile3->AddPrerequisite( new FileElement("main.cpp") );
  objectFile3->AddPrerequisite( headerFile1 );
  objectFile3->AddPrerequisite( headerFile2 );

  goal->Make();

  return 0;
}

