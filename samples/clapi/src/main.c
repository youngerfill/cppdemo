#include <stdio.h>
#include <assert.h>
#include <string.h> /* TODO : remove */ 

#include <clapi.h>

#define NUMDELIM 5



/********************/ 
int main(int argc, char *argv[])
{
  int retVal = CLAPI_OK;
  ClapiVarType* pvar = NULL;
  
  double temperature;
  
  unsigned int dayOfWeek;
  const char* days[] = { "monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday" };
  
  const char comment[120];
  
  int numSamples;
  
/*
#if defined NDEBUG
  printf("ndebug defined\n");
#else
  printf("ndebug not defined\n");
#endif
*/

  clapi_init();

  pvar = clapi_addVarDouble("temp", &temperature, 20, -40, 60);
  if (pvar!=NULL)
  {
    clapi_varSetShortDesc(pvar, "The temperature");
    clapi_varSetDimension(pvar, "Degrees Celsius");
  }

  pvar = clapi_addVarChoice("day", &dayOfWeek, 2, 7, days);
  if (pvar!=NULL)
  {
    clapi_varSetShortDesc(pvar, "The day of the week");
    clapi_varSetLongDesc(pvar, "Can be monday etc ... up to and including sunday");
  }
  
  pvar = clapi_addVarString("comment", &comment, "No_comment", 120);
  if (pvar!=NULL)
  {
    clapi_varSetShortDesc(pvar, "Some helpful comment");
  }

  pvar = clapi_addVarInt("numsamples", &numSamples, 5, 1, 100);
  if (pvar!=NULL)
  {
    clapi_varSetShortDesc(pvar, "The number of samples to be taken.");
  }
  
  retVal = clapi_parseCommandLine(argc, argv);
  if (retVal==CLAPI_REQUESTEXIT)
    return CLAPI_OK;
  if (retVal!=CLAPI_OK)
    return retVal;
    
  printf("Rest of program.\n");
  
  return 0;
}


