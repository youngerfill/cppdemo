#include <iostream>
#include <stdlib.h>

#include "serdesplanb.hpp"
#include "shapes.hpp"
#include "blib/StringTools.hpp"

#include <fstream>
#include <vector>

using namespace std;
using namespace blib;

////////////////////
// 3rd party class
////////////////////
struct Class3rdParty
{
  Class3rdParty(int number, std::string name)
  {
    number_ = number;
    name_ = name;
  }

  int number_;
  std::string name_;
};


////////////////////////////////////
// Make Class3rdParty serializable
////////////////////////////////////
template <class SerDes>
bool WriteBody(SerDes& serdes, const std::string& name, const Class3rdParty& object)
{
    WriteObject(serdes, "number", object.number_);
    WriteObject(serdes, "name", object.name_);
    return true;
}

SD_MAKE_CLASSNAME(Class3rdParty)

///////////////////////
// app-specific class
///////////////////////
struct ClassApp
{
  ClassApp(double temp, std::string place)
  {
    temp_ = temp;
    place_ = place;
  }

  virtual bool Write(SerDesOutBase& serdes, const std::string& name) const
  {
    WriteObject(serdes, "temp", temp_);
    WriteObject(serdes, "place", place_);
    return true;
  }

  SD_DECLARE_CLASS(ClassApp, 2000)

  float temp_;
  std::string place_;
};

SD_MAKE_BASECLASS(ClassApp)


//////////////////////////////////////////////
// app-specific class, derived from ClassApp
//////////////////////////////////////////////
struct ClassApp2 : public ClassApp
{
  ClassApp2(double temp, std::string place, std::string time)
  :ClassApp(temp, place)
  {
    time_ = time;
  }

  virtual bool Write(SerDesOutBase& serdes, const std::string& name) const
  {
    ClassApp::Write(serdes, name);
    WriteObject(serdes, "time", time_);
    return true;
  }

  SD_DECLARE_CLASS(ClassApp2, 2010)

  std::string time_;
};

////////////////////////////////////////////
////////////////////////////////////////////
void MiscTests()
{
  SerDesOutPlanB theSerDes(cout);

  int Inty = 5;
  WriteObject(theSerDes, "Inty", Inty);

  float Floaty = 3.14F;
  WriteObject(theSerDes, "Floaty", Floaty);

  cout << endl << endl;

  ClassApp myObject(17.31F, "garden");
  WriteObject(theSerDes, "myObject", myObject);

  cout << endl << endl;

  ClassApp2 myObject2(20.63F, "kitchen", "yesterday");
  WriteObject(theSerDes, "myObject2", (const ClassApp&)myObject2);

  cout << endl << endl;

  ClassApp* pObject = &myObject2;
  WriteObject(theSerDes, "*pObject", *pObject);

  cout << endl << endl;

  Class3rdParty theirObject(25, "NumberTwentyFive");
  WriteObject(theSerDes, "theirObject", theirObject);

  cout << endl << endl;

  int aVec[] = { 1, 2, 3, 5, 7, 11, 13, 17, 19 };
  vector<int> theVec;

  for (int i=0; i<9; i++)
    theVec.push_back(aVec[i]);

  WriteObject(theSerDes, "theVec", theVec);

  cout << endl;

  list<int> theList;
  for (int i=0; i<9; i++)
    theList.push_back(aVec[i]);

  WriteObject(theSerDes, "theList", theList);

  int theCArray[9];
  for (int i=0; i<9; i++)
    theCArray[i] = aVec[i];

  cout << endl;

  WriteObject(theSerDes, "theCArray", theList);

  list<ClassApp> classAppList;
  for (int i=0; i<9; i++)
    classAppList.push_back(ClassApp(i, "heheh"));

  cout << endl;

  WriteObject(theSerDes, "classAppList", classAppList);

  cout << endl;

  WriteObject(theSerDes, "pObject", pObject);

  cout << endl;

  map<string, int> months;
  months["january"] = 31;
  months["february"] = 28;
  months["march"] = 31;
  months["april"] = 30;
  months["may"] = 31;
  months["june"] = 30;
  months["july"] = 31;
  months["august"] = 31;
  months["september"] = 30;
  months["october"] = 31;
  months["november"] = 30;
  months["december"] = 31;
  WriteObject(theSerDes, "theMap", months);


}

////////////////////////////////////////////
////////////////////////////////////////////
void WriteDataToFile()
{
  ofstream theFile("../data/file.txt");

  SerDesOutPlanB theSerDes(theFile);

  int Inty = 5;
  WriteObject(theSerDes, "Inty", Inty);

  float Floaty = 3.14F;
  WriteObject(theSerDes, "Floaty", Floaty);

  cout << endl << endl;

}


////////////////////////////////////////////
////////////////////////////////////////////
void ReadDataFromFile()
{
  ifstream theFile("../data/file.txt");
  SerDesInPlanB theSerDes(theFile);

  int Inty = 0;
  float Floaty = 0;

  streampos test0 = theFile.tellg();

  ReadObject(theSerDes, "Inty", Inty);
  ReadObject(theSerDes, "Floaty", Floaty);

  cout << "Inty = " << Inty << endl;
  cout << "Floaty = " << Floaty << endl;
}

////////////////////////////////////////////
////////////////////////////////////////////
void ReadTest()
{
//  WriteDataToFile();
  ReadDataFromFile();
}

////////////////////////////////////////////
////////////////////////////////////////////
void PolymTest()
{
   Circle circle;
   circle.center_ = Point(5,6);
   circle.radius_ = 10;

   Triangle triangle;
   triangle.point1_ = Point(10,1);
   triangle.point2_ = Point(2,20);
   triangle.point3_ = Point(30,3);

   Rectangle rectangle;
   rectangle.point1_ = Point(-10,-10);
   rectangle.point2_ = Point(20,20);

   typedef list<Shape*> ShapeList;
   ShapeList shapeList;
   shapeList.push_back(&circle);
   shapeList.push_back(&triangle);
   shapeList.push_back(&rectangle);

   SerDesOutPlanB theSerDes(cout);
   WriteObject(theSerDes, "shapeList", shapeList);

   Point thePoint(1,2);
   WriteObject(theSerDes, "thePoint", thePoint);
}

//////////////////////////////////////////////
//             main()
//////////////////////////////////////////////
int main(int argc, char *argv[])
{
  MiscTests();

/*  ifstream theFile("../data/file.txt");
  string auxString;
  if (ReadValueString(theFile, "Value", auxString))
    cout << "Value = '" << auxString << "'" << endl;
  else
    cout << "ERROR!" << endl;*/

/*  while (theFile.good())
  {
    theFile.get(ch);
    cout << ch;
  }*/

  system("PAUSE");
  return 0;
}


