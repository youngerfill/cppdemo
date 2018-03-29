#if !defined CLAPI_H
#define CLAPI_H


/* #define CLAPI_TESTFORCEEXIT */


enum
{
  CLAPI_OK=0,
  CLAPI_SYNTAXERROR,
  CLAPI_OUTOFMEMORY,
  CLAPI_FATALERROR,
  CLAPI_FILEERROR,
  CLAPI_CIRCULARINCLUDES,
  CLAPI_OUTOFBOUNDS,
  CLAPI_REQUESTEXIT,
  CLAPI_NUMERRORS
};



typedef struct
{
  int* pvalue_;
  int default_;
  int min_;
  int max_;
} ClapiVarTypeInt;


typedef struct
{
  double* pvalue_;
  double default_;
  double min_;
  double max_;
} ClapiVarTypeDouble;


typedef struct
{
  const char** pvalue_;
  const char* default_;
  unsigned int maxLength_;
} ClapiVarTypeString;


typedef struct
{
  unsigned int* pvalue_;
  unsigned int default_;
  unsigned int numChoices_;
  const char** choices_;
} ClapiVarTypeChoice;


typedef struct
{
  unsigned short type_;
  const char* name_;
  
  const char* shortDesc_;
  const char* longDesc_;
  const char* dimension_;
  
  union
  {
    ClapiVarTypeInt varInt_;
    ClapiVarTypeDouble varDouble_;
    ClapiVarTypeString varString_;
    ClapiVarTypeChoice varChoice_;
  };
} ClapiVarType;

int clapi_init();

ClapiVarType* clapi_addVarInt(const char* name, int* pvalue, int defaultvalue, int min, int max);
ClapiVarType* clapi_addVarDouble(const char* name, double* pvalue, double defaultvalue, double min, double max);
ClapiVarType* clapi_addVarString(const char* name, const char (*pvalue)[], const char* defaultValue, unsigned int maxLength);
ClapiVarType* clapi_addVarChoice(const char* name, unsigned int* pvalue, unsigned int defaultValue, unsigned int numChoices, const char* choices[]);

void clapi_varSetShortDesc(ClapiVarType* pvar, const char* shortDesc);
void clapi_varSetLongDesc(ClapiVarType* pvar, const char* longDesc);
void clapi_varSetDimension(ClapiVarType* pvar, const char* dimension);

void clapi_appSetShortDesc(const char* shortDesc);
void clapi_appSetLongDesc(const char* longDesc);

int clapi_parseCommandLine(int argc, char *argv[]);
 
#endif /* CLAPI_H */


