#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>

#include "StringTools.hpp"

using namespace std;
using namespace blib;

int main(int argc, char *argv[])
{
  string theString = "OneTwoThreeFourFiveSixSevenEightNineTen";
  istringstream theStream(theString);
  
  string auxString;
  bool success = ReadUpTo(theStream, 'x', auxString);
  if (success)
    cout << "success" << endl;
  else
    cout << "failure" << endl;
    
  cout << "auxString = '" << auxString << "'" << endl;
  
  system("PAUSE");	
  return 0;
}
