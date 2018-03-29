#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include <stdarg.h>
#include <ctype.h>

#include <clapi.h>

#define CLAPI_VERSION_MAJOR 1
#define CLAPI_VERSION_MINOR 0
#define CLAPI_VERSION_REVISION 0

#if !defined CLAPI_MAXNUMVARS
  #define CLAPI_MAXNUMVARS 64
#endif

#if !defined CLAPI_MAXNUMHEAPSTRINGS
  #define CLAPI_MAXNUMHEAPSTRINGS 128
#endif

#if !defined CLAPI_MAXLINELENGTH
  #define CLAPI_MAXLINELENGTH 512
#endif

#if !defined CLAPI_MAXNUMPLUSFILES
  #define CLAPI_MAXNUMPLUSFILES 64
#endif

#if !defined NDEBUG
  #define clapi_assert(assertion, errorMessage) \
    if (!(assertion)) { doAssert(__FILE__, __LINE__, #assertion, getFormattedString errorMessage ); } 
#else
  #define clapi_assert(assertion, errorMessage)
#endif

enum
{
  CLAPI_VARTYPEINT=0,
  CLAPI_VARTYPEDOUBLE,
  CLAPI_VARTYPESTRING,
  CLAPI_VARTYPECHOICE,
  CLAPI_NUMTYPES
};

unsigned int clapi_maxNumVars = CLAPI_MAXNUMVARS;
unsigned int clapi_numVars = 0;
ClapiVarType* clapi_vars = NULL;

unsigned int clapi_maxNumHeapStrings = CLAPI_MAXNUMHEAPSTRINGS;
unsigned int clapi_numHeapStrings = 0;
char** clapi_heapStrings=0;

unsigned int clapi_maxLineLength = CLAPI_MAXLINELENGTH;

unsigned int clapi_maxNumPlusFiles = CLAPI_MAXNUMPLUSFILES;
unsigned int clapi_numPlusFiles = 0;
const char** clapi_plusFiles=0;

const char* clapi_appShortDesc_=NULL;
const char* clapi_appLongDesc_=NULL;

const char* clapi_typeStrings[] = { "int", "double", "string", "choice" };


/**********/ 
char* trimSpaces(char* theString)
{
  char* begin = theString;
  char* end = 0;
  
  assert(theString!=NULL);
  
  end = theString + strlen(theString);
  
  while (isspace(*begin))
    begin++;

  while (end>begin)
    if (!isspace(*(--end)))
      break;
      
  if (end>begin)
    *(end+1)='\0';
  
  return begin;
}

/**********/
int hasAlpha(const char* theString)
{
  const char* pchar = theString;
  const char* end = theString + strlen(theString);
  
  assert(theString!=NULL);

  while (pchar!=end)
    if (isalpha(*pchar++))
      return 1;
      
  return 0;
}

/**********/
int hasNonPrintable(const char* theString)
{
  const char* pchar = theString;
  const char* end = theString + strlen(theString);
  
  assert(theString!=NULL);

  while (pchar!=end)
  {
    if ( *pchar<32 || *pchar>127 )
      return 1;
    pchar++;
  }
      
  return 0;
}

/**********/
int hasQuotes(const char* theString)
{
  const char* pchar = theString;
  const char* end = theString + strlen(theString);
  
  assert(theString!=NULL);

  while (pchar!=end)
  {
    if ( *pchar=='\"' || *pchar=='\'' )
      return 1;
    pchar++;
  }
      
  return 0;
}

/**********/
char* getFormattedString(const char* formatString, ...)
{
  static char formattedString[CLAPI_MAXLINELENGTH];
  
  va_list ap;
  va_start(ap, formatString);

  vsnprintf(formattedString, CLAPI_MAXLINELENGTH, formatString, ap);

  va_end (ap);

  return formattedString;
}

/**********/
void doAssert(const char* fileName, unsigned long lineNumber, const char* assertion, const char* errorMessage)
{
  fprintf(stderr, "%s:%u: Assertion failed: %s\n", fileName, lineNumber, assertion);
  fprintf(stderr, "%s\n", errorMessage);
  fflush(stderr);
  abort();
}

/**********/
void printXMLEncoded(const char* theString)
{
  const char* pchar = theString;

  do
  {
    switch( (*pchar) )
    {
      case '&': 
        printf("&amp;"); 
        break;
      case '<':
        printf("&lt;"); 
        break;
      case '>':
        printf("&gt;"); 
        break;
      case '"':
        printf("&quot;"); 
        break;
      case '\'':
        printf("&apos;"); 
        break;

      default:
        if ( *pchar<32 || *pchar>127 )
          printf("&#%u", (unsigned int)(*pchar));
        else
          printf("%c", (*pchar));
        break;
    }
  }
  while (*(++pchar)!='\0');
}

/**********/
void showVarValue(const ClapiVarType* pvar, int xml)
{
  assert(pvar->type_<CLAPI_NUMTYPES);
  
  switch (pvar->type_)
  {
    case CLAPI_VARTYPEINT:
      printf("%d", *(pvar->varInt_.pvalue_));
      break;
    case CLAPI_VARTYPEDOUBLE:
      printf("%g", *(pvar->varDouble_.pvalue_));
      break;
    case CLAPI_VARTYPESTRING:
      if (xml)
        printXMLEncoded(*(pvar->varString_.pvalue_));
      else
        printf(*(pvar->varString_.pvalue_));
      break;
    case CLAPI_VARTYPECHOICE:
      if (xml)
        printXMLEncoded(pvar->varChoice_.choices_[*(pvar->varChoice_.pvalue_)]);
      else
        printf(pvar->varChoice_.choices_[*(pvar->varChoice_.pvalue_)]);
      break;
    default:
      clapi_assert( (pvar->type_<CLAPI_NUMTYPES), ("Unknown variable type (%d)", pvar->type_) );
  }
}

/**********/
char* createHeapString(unsigned int size)
{
  char* heapString=0;
  assert(clapi_heapStrings!=NULL);
  
  if (clapi_numHeapStrings>=clapi_maxNumHeapStrings)
  {
    fprintf(stderr, "Error: number of strings on the heap exceeds maximum (%u)\n", clapi_numHeapStrings);
    return NULL;
  }
  
  /* +1 to provide room for the null character */ 
  heapString = malloc(size + 1);
  if (heapString==NULL)
  {
    fprintf(stderr, "Error: unable to allocate a string on the heap\n");
    return NULL;
  }
    
  clapi_heapStrings[clapi_numHeapStrings++] = heapString;
}

/**********/
char* strDup(const char* str)
{
  char* str_duplicate=0;
  assert(str!=NULL);

  str_duplicate = createHeapString(strlen(str));
  return strcpy(str_duplicate, str);
}

/**********/
ClapiVarType* findVarByName(const char* name)
{
  unsigned int i=0;

  assert(clapi_vars!=NULL);
  assert(name!=NULL);
  
  for (i=0; i<clapi_numVars; i++)
    if (!strcmp(clapi_vars[i].name_, name))
      return &(clapi_vars[i]);
      
  return NULL;
}

/**********/
ClapiVarType* findVarByIndex(int varindex)
{
  assert(clapi_vars!=NULL);
  
  if ((varindex<0) || (varindex>=clapi_numVars))
  {
    fprintf(stderr, "Error: variable index (%d) out of bounds. Must be: 0 <= varindex < %d\n", varindex, clapi_numVars);
    return NULL;
  }
  
  return &(clapi_vars[varindex]);
}

/**********/
ClapiVarType* findVarByID(const char* id)
{
  assert(clapi_vars!=NULL);
  assert(id!=NULL);

  if (hasAlpha(id))
    return findVarByName(id);
  else
    return findVarByIndex(atoi(id));
}

/**********/
int getStringIndex(const char* theString, unsigned int numStrings, const char* strings[])
{
  unsigned int i=0;

  assert(theString!=NULL);
  assert(strings!=NULL);
  
  for (i=0; i<numStrings; i++)
    if (!strcmp(strings[i], theString))
      return i;
      
  return -1;
}

/**********/ 
int processAssignment(char* statement, char inPlusFile)
{
  char* varName=NULL;
  char* valueString=NULL;
  ClapiVarType* pvar=NULL;
  char isDefaultAssignment=0;

  assert(statement!=NULL);
  assert(strlen(statement)>0);

  /* If no '=' present, treat the statement as an */
  /* assignment to the default value */ 
  if ( strchr(statement, '=') == NULL )  
  {
    isDefaultAssignment = 1;
    varName = statement;
  }
  else
    /* If no LHS to the left of '=' : error */ 
    if (statement[0]=='=')
    {
      fprintf(stderr, "Error: invalid statement: '%s'\n", statement);
      return CLAPI_SYNTAXERROR;
    }
    /* Obtain LHS of assignment: variable name */
    else
    {
      varName = strtok(statement, "=");
      /* Store position of one-past '=' for future use */ 
      valueString = varName + strlen(varName) + 1;
    }
  
  varName = trimSpaces(varName);
  
  /* Look up the variable name in list of vars and obtain */ 
  /* a pointer to the correspondng ClapiVarType struct.        */
  pvar = findVarByName(varName);
  if (pvar==NULL)
  {
    fprintf(stderr, "Error: unknown variable: '%s'\n", varName);
    return CLAPI_SYNTAXERROR;
  }
  
  /* In case of default statement: handle it here and return */ 
  if ( isDefaultAssignment == 1 )
  {
      switch (pvar->type_)
      {
        case CLAPI_VARTYPEINT:
          *(pvar->varInt_.pvalue_) = pvar->varInt_.default_;
          break;
        case CLAPI_VARTYPEDOUBLE:
          *(pvar->varDouble_.pvalue_) = pvar->varDouble_.default_;
          break;
        case CLAPI_VARTYPESTRING:
          *(pvar->varString_.pvalue_) = pvar->varString_.default_;
          break;
        case CLAPI_VARTYPECHOICE:
          *(pvar->varChoice_.pvalue_) = pvar->varChoice_.default_;
          break;
        default:
          clapi_assert( (pvar->type_<CLAPI_NUMTYPES), ("Unknown variable type (%d) in statement: '%s'\n", varName) );
      }
      
    return CLAPI_OK;
  }

  /* Obtain RHS of assignment: value string */
  // valueString = strtok(NULL, "=");
  if (valueString[0]=='\0')
    if (pvar->type_==CLAPI_VARTYPESTRING)
      valueString="";
    else
    {
      fprintf(stderr, "Error: invalid statement: '%s='\n", statement);
      return CLAPI_SYNTAXERROR;
    }
  
  valueString = trimSpaces(valueString);

  /* Convert and validate value string according to the type of the variable */ 
  switch (pvar->type_)
  {
    case CLAPI_VARTYPEINT:
    {
      int value = atoi(valueString);
      if ( value > pvar->varInt_.max_ )
      {
        fprintf(stderr, "Warning: Value exceeds maximum in statement: '%s=%s'\n", varName, valueString);
        fprintf(stderr, "Value (%d) will be clipped to maximum (%d)\n", value, pvar->varInt_.max_);
        value = pvar->varInt_.max_;
      }
      if ( value < pvar->varInt_.min_ )
      {
        fprintf(stderr, "Warning: Value below minimum in statement: '%s=%s'\n", varName, valueString);
        fprintf(stderr, "Value will be clipped to minimum (%d)\n", pvar->varInt_.min_);
        value = pvar->varInt_.min_;
      }
      *(pvar->varInt_.pvalue_) = value;
      break;
    }
      
    case CLAPI_VARTYPEDOUBLE:
    {
      double value = atof(valueString);
      if ( value > pvar->varDouble_.max_ )
      {
        fprintf(stderr, "Warning: Value exceeds maximum in statement: '%s=%s'\n", varName, valueString);
        fprintf(stderr, "Value (%g) will be clipped to maximum (%g)\n", value, pvar->varDouble_.max_);
        value = pvar->varDouble_.max_;
      }
      if ( value < pvar->varDouble_.min_ )
      {
        fprintf(stderr, "Warning: Value below minimum in statement: '%s=%s'\n", varName, valueString);
        fprintf(stderr, "Value will be clipped to minimum (%g)\n", pvar->varDouble_.min_);
        value = pvar->varDouble_.min_;
      }
      *(pvar->varDouble_.pvalue_) = value;
      break;
    }
    
    case CLAPI_VARTYPESTRING:
    {
      if (pvar->varString_.maxLength_>0)
        if (strlen(valueString)>pvar->varString_.maxLength_)
        {
          fprintf(stderr, "Warning: Value string length exceeds maximum in statement: '%s=%s'\n", varName, valueString);
          fprintf(stderr, "Value string will be truncated to maximum length (%u)\n", pvar->varString_.maxLength_);
          valueString[pvar->varString_.maxLength_]='\0';
        }
      /* Clone string on heap if in plusfile, keep pointing into argv[] otherwise */ 
      *(pvar->varString_.pvalue_) = inPlusFile ? strDup(valueString) : valueString;
      break;
    }

    case CLAPI_VARTYPECHOICE:
    {
      int value = getStringIndex(valueString, pvar->varChoice_.numChoices_, pvar->varChoice_.choices_);
      if (value<0)
      {
        fprintf(stderr, "Warning: Invalid choice value in statement: '%s=%s'\n", varName, valueString);
        fprintf(stderr, "Default value (%s) will be used instead.\n", pvar->varChoice_.choices_[pvar->varChoice_.default_]);
        value = pvar->varChoice_.default_;
      }
      *(pvar->varChoice_.pvalue_) = value;
      break;
    }

    default:
      clapi_assert( (pvar->type_<CLAPI_NUMTYPES), ("Unknown variable type (%d) in statement: '%s=%s'\n", varName, valueString) );
  }

  return CLAPI_OK;
}

int processPlusFile(char* includeStatement);

/**********/
int processLine(char* line)
{
  char* trimmedLine = trimSpaces(line);
  
  /* Line with only whitespaces: skip */
  if (strlen(trimmedLine)==0)
    return CLAPI_OK;
    
  /* A comment line: skip */
  if (trimmedLine[0]=='#')
    return CLAPI_OK;
    
  /* Another plusfile: process it */
  if (trimmedLine[0]=='+')
    return processPlusFile(trimmedLine);
    
  /* Any other line: process as assignment */ 
  return processAssignment(trimmedLine, 1 /*in plusfile*/ );
}


/**********/
int pushPlusFile(const char* fileName)
{
  if (getStringIndex(fileName, clapi_numPlusFiles, clapi_plusFiles)>=0)
  {
    fprintf(stderr, "Error: circular including of plusfiles detected\n");
    return CLAPI_CIRCULARINCLUDES;
  }
  
  clapi_plusFiles[clapi_numPlusFiles++] = fileName;
  return CLAPI_OK;
}


/**********/
const char* popPlusFile()
{
  return clapi_plusFiles[--clapi_numPlusFiles];
}


/**********/
int processPlusFile(char* includeStatement)
{
  char* fileName = includeStatement+1;
  char* line=0;
  char* statement=0;
  int retval=CLAPI_OK;
  FILE* fileDesc=NULL;
  const char* plusFilePopped=0;
  
  /* open plusfile for reading */
  fileDesc = fopen(fileName, "r");
  if (fileDesc==NULL)
  {
    fprintf(stderr, "Error opening file:'%s'\n", fileName);
    return CLAPI_FILEERROR;
  }
  
  /* Allocate memory for line buffer */ 
  /* +2 to provide room for newline and null character */
  line = malloc(clapi_maxLineLength+2);
  if (line==NULL)
  {
    fprintf(stderr, "Error: unable to allocate a block with length %u\n", clapi_maxLineLength+2);
    return CLAPI_OUTOFMEMORY;
  }
  
  /* Keep track of stack of opened plusfiles to detect circular dependencies */ 
  /* Otherwise endless recursion could occur */ 
  retval = pushPlusFile(fileName);
  if (retval!=CLAPI_OK)
    return retval;
  
  /* Read line from file and process it */ 
  /* Only clapi_maxLineLength+1 characters will be read maximally */ 
  while (fgets(line, clapi_maxLineLength+2, fileDesc))
    if ( (retval=processLine(line)) != CLAPI_OK )
      break;

  /* Remove plusfile from stack */
  plusFilePopped=popPlusFile();
  clapi_assert ( ( strcmp(plusFilePopped, fileName)==0 ), ( "Confusion between plusfiles: '%s' and '%s' \n", plusFilePopped, fileName) );

  /* clean up */ 
  free(line);
  fclose(fileDesc);
  
  return retval;
}

/**********/ 
int processStatement(char* statement)
{
  assert(statement!=NULL);
  assert(strlen(statement)>0);
  
  if (statement[0]=='+')
    return processPlusFile(statement);
    
  else
    return processAssignment(statement, 0 /*not in plusfile*/ );
}

/**********/
int processClapiArgs(int argc, char *argv[])
{
  int i=0;
  int retval=0;
  
  assert(argc>=0);
  assert(argv!=NULL);

  for (i=0; i<argc; i++)
  {
    retval = processStatement(argv[i]);
    if (retval!=CLAPI_OK)
      return retval;
  }
  
  return CLAPI_OK;
}

/**********/
ClapiVarType* interrogationHelper(int argc, char *argv[], const char* command)
{
    ClapiVarType* pvar=NULL;
    
    if (argc<2)
    {
      fprintf(stderr, "Error: varID expected after '%s'\n", command);
      return NULL;
    }
    
    return findVarByID(argv[1]);
}

/**********/
void writeXMLIndentation(unsigned short indentation)
{
  unsigned short i=0;
  for (i=0; i< indentation; i++)
    printf("\t");
}

/**********/
void writeXMLElement(unsigned short indentation, const char* elementName, const char* valueString)
{
  writeXMLIndentation(indentation);
  if (strlen(valueString)==0)
    printf("<%s/>\n", elementName);
  else
  {
    printf("<%s>", elementName);
    printXMLEncoded(valueString);
    printf("</%s>\n", elementName);
  }
}

/**********/
int doInterrogationMode(int argc, char *argv[])
{
  int retval = CLAPI_REQUESTEXIT;
  int i=0;
  
  assert(clapi_vars!=NULL);

  /* TODO : choose some default action ('help'???) */ 
  if (argc==0)
    return CLAPI_REQUESTEXIT;
    
  /* -clapi version */ 
  /* Displays the version number of CLAPI */ 
  if (!strcmp(argv[0], "version"))
  {
    printf("CLAPI version %d.%d.%d\n", CLAPI_VERSION_MAJOR, CLAPI_VERSION_MINOR, CLAPI_VERSION_REVISION);
    return CLAPI_REQUESTEXIT;
  }
  
  /* -clapi numvars */ 
  /* Displays the number of variables registered to CLAPI */ 
  if (!strcmp(argv[0], "numvars"))
  {
    printf("%u\n", clapi_numVars);
    return CLAPI_REQUESTEXIT;
  }
  
  /* -clapi index VARNAME */ 
  /* Displays the index of the variable specified by VARNAME*/ 
  if (!strcmp(argv[0], "index"))
  {
    if (argc<2)
    {
      fprintf(stderr, "Error: varname expected after 'index'\n");
      return CLAPI_SYNTAXERROR;
    }

    for (i=0; i<clapi_numVars; i++)
      if (!strcmp(clapi_vars[i].name_, argv[1]))
      {
        printf("%d\n", i);
        return CLAPI_REQUESTEXIT;
      }
      
    fprintf(stderr, "Error: Invalid variable name '%s'\n", argv[1]);
    return CLAPI_SYNTAXERROR;
  }  
  
  /* -clapi name VARINDEX */ 
  /* Displays the name of the variable specified by VARINDEX */ 
  if (!strcmp(argv[0], "name"))
  {
    ClapiVarType* pvar=NULL;
    
    if (argc<2)
    {
      fprintf(stderr, "Error: varindex expected after 'name'\n");
      return CLAPI_SYNTAXERROR;
    }

    pvar = findVarByIndex(atoi(argv[1]));
    if (pvar==NULL)
      return CLAPI_OUTOFBOUNDS;
      
    printf("%s\n", pvar->name_);
    return CLAPI_REQUESTEXIT;
  }  

  /* -clapi show */ 
  /* ??? */ 

  /* -clapi show VARID*/ 
  /* ??? */ 
  
  /* -clapi type VARID */
  /* Displays the type of the variable specified by VARID */ 
  if (!strcmp(argv[0], "type"))
  {
    ClapiVarType* pvar = interrogationHelper(argc, argv, "type");
    if (pvar==NULL)
      return CLAPI_FATALERROR;

    assert(pvar->type_<CLAPI_NUMTYPES);
    
    printf("%s\n", clapi_typeStrings[pvar->type_]);
    return CLAPI_REQUESTEXIT;
  }
  
  /* -clapi dimension VARID */
  /* Displays the dimension of the variable specified by VARID */ 
  if (!strcmp(argv[0], "dimension"))
  {
    ClapiVarType* pvar = interrogationHelper(argc, argv, "dimension");
    if (pvar==NULL)
      return CLAPI_FATALERROR;

    printf("%s\n", pvar->dimension_);
    return CLAPI_REQUESTEXIT;
  }
  
  /* -clapi choices VARID */
  /* Displays all possible choices of the variable specified by VARID */ 
  if (!strcmp(argv[0], "choices"))
  {
    int i=0;
    ClapiVarType* pvar = interrogationHelper(argc, argv, "choices");
    if (pvar==NULL)
      return CLAPI_FATALERROR;

    if (pvar->type_!=CLAPI_VARTYPECHOICE)
    {
      fprintf(stderr, "Error: variable ('%s') is not of type 'choice'\n", pvar->name_);
      return CLAPI_FATALERROR;
    }

    for (i=0; i<pvar->varChoice_.numChoices_; i++)
      printf("%s\n", pvar->varChoice_.choices_[i]);

    return CLAPI_REQUESTEXIT;
  }
  
  /* -clapi numchoices VARID */
  /* Displays the number of choices of the variable specified by VARID */ 
  if (!strcmp(argv[0], "numchoices"))
  {
    ClapiVarType* pvar = interrogationHelper(argc, argv, "choices");
    if (pvar==NULL)
      return CLAPI_FATALERROR;

    if (pvar->type_!=CLAPI_VARTYPECHOICE)
    {
      fprintf(stderr, "Error: variable ('%s') is not of type 'choice'\n", pvar->name_);
      return CLAPI_FATALERROR;
    }

    printf("%u\n", pvar->varChoice_.numChoices_);
    return CLAPI_REQUESTEXIT;
  }
  
  /* -clapi choice CHOICEIDX VARID */
  /* Displays one choice (with index CHOICEIDX) of the choice variable specified by VARID */ 
  if (!strcmp(argv[0], "choice"))
  {
    ClapiVarType* pvar=NULL;
    int choiceIndex=0;
    
    if (argc<3)
    {
      fprintf(stderr, "Error: choiceIDX and varID expected after 'choice'\n");
      return CLAPI_SYNTAXERROR;
    }

    pvar = findVarByID(argv[2]);
    if (pvar==NULL)
      return CLAPI_FATALERROR;
      
    if (pvar->type_!=CLAPI_VARTYPECHOICE)
    {
      fprintf(stderr, "Error: variable ('%s') is not of type 'choice'\n", pvar->name_);
      return CLAPI_FATALERROR;
    }

    choiceIndex = atoi(argv[1]);
    if ( (choiceIndex<0) || (choiceIndex>=pvar->varChoice_.numChoices_) )
    {
      fprintf(stderr, "Error: choiceIndex (%d) out of bounds. Must be: 0 <= choiceIndex < %d\n", choiceIndex, choiceIndex>=pvar->varChoice_.numChoices_);
      return CLAPI_OUTOFBOUNDS;
    }
    
    printf("%s\n", pvar->varChoice_.choices_[choiceIndex]);
    return CLAPI_REQUESTEXIT;
  }
  
  /* -clapi min VARID */
  /* Displays the minimum value of the variable specified by VARID */ 
  if (!strcmp(argv[0], "min"))
  {
    ClapiVarType* pvar = interrogationHelper(argc, argv, "min");
    if (pvar==NULL)
      return CLAPI_FATALERROR;

    if (pvar->type_==CLAPI_VARTYPESTRING)
    {
      fprintf(stderr, "Error: Variables of type 'string' don't have a minimum\n");
      return CLAPI_FATALERROR;
    }
    
    switch (pvar->type_)
    {
      case CLAPI_VARTYPEINT : 
        printf("%d\n", pvar->varInt_.min_);
        break;
      case CLAPI_VARTYPEDOUBLE : 
        printf("%g\n", pvar->varDouble_.min_);
        break;
      case CLAPI_VARTYPECHOICE : 
        printf("0\n");
        break;
      default:
        clapi_assert( (pvar->type_<CLAPI_NUMTYPES), ("Unknown variable type (%d)", pvar->type_) );
    }

    return CLAPI_REQUESTEXIT;
  }
  
  /* -clapi max VARID */
  /* Displays the maximum value of the variable specified by VARID */ 
  if (!strcmp(argv[0], "max"))
  {
    ClapiVarType* pvar = interrogationHelper(argc, argv, "max");
    if (pvar==NULL)
      return CLAPI_FATALERROR;

    if (pvar->type_==CLAPI_VARTYPESTRING)
    {
      fprintf(stderr, "Error: Variables of type 'string' don't have a maximum\n");
      return CLAPI_FATALERROR;
    }
    
    switch (pvar->type_)
    {
      case CLAPI_VARTYPEINT : 
        printf("%d\n", pvar->varInt_.max_);
        break;
      case CLAPI_VARTYPEDOUBLE : 
        printf("%g\n", pvar->varDouble_.max_);
        break;
      case CLAPI_VARTYPECHOICE : 
        printf("%u\n", pvar->varChoice_.numChoices_-1);
        break;
      default:
        clapi_assert( (pvar->type_<CLAPI_NUMTYPES), ("Unknown variable type (%d)", pvar->type_) );
    }
    
    return CLAPI_REQUESTEXIT;
  }

  /* -clapi default VARID */
  /* Displays the default value of the variable specified by VARID */ 
  if (!strcmp(argv[0], "default"))
  {
    ClapiVarType* pvar = interrogationHelper(argc, argv, "default");
    if (pvar==NULL)
      return CLAPI_FATALERROR;

    switch (pvar->type_)
    {
      case CLAPI_VARTYPEINT : 
        printf("%d\n", pvar->varInt_.default_);
        break;
      case CLAPI_VARTYPEDOUBLE : 
        printf("%g\n", pvar->varDouble_.default_);
        break;
      case CLAPI_VARTYPESTRING : 
        printf("%s\n", pvar->varString_.default_);
        break;
      case CLAPI_VARTYPECHOICE : 
        printf("%u\n", pvar->varChoice_.default_);
        break;
      default:
        clapi_assert( (pvar->type_<CLAPI_NUMTYPES), ("Unknown variable type (%d)", pvar->type_) );
    }

    return CLAPI_REQUESTEXIT;
  }

  /* -clapi maxlength VARID */
  /* Displays the maximum length of the string variable specified by VARID */ 
  if (!strcmp(argv[0], "maxlength"))
  {
    ClapiVarType* pvar = interrogationHelper(argc, argv, "maxlength");
    if (pvar==NULL)
      return CLAPI_FATALERROR;

    if (pvar->type_!=CLAPI_VARTYPESTRING)
    {
      fprintf(stderr, "Error: variable ('%s') is not of type 'string'\n", pvar->name_);
      return CLAPI_FATALERROR;
    }

    printf("%u\n", pvar->varString_.maxLength_);
    return CLAPI_REQUESTEXIT;
  }
  
  /* -clapi shortdesc [VARID] */
  /* Displays the short description of the variable specified by VARID */ 
  /* If no VARID specified, displays the app-wide short desc */ 
  if (!strcmp(argv[0], "shortdesc"))
  {
    ClapiVarType* pvar = NULL;
    
    if (argc==1)
    {
      printf("%s\n", clapi_appShortDesc_);
      return CLAPI_REQUESTEXIT;
    }

    pvar = findVarByID(argv[1]);
    if (pvar==NULL)
      return CLAPI_FATALERROR;

    printf("%s\n", pvar->shortDesc_);
    return CLAPI_REQUESTEXIT;
  }

  /* -clapi longdesc [VARID] */
  /* Displays the long description of the variable specified by VARID */ 
  /* If no VARID specified, displays the app-wide long desc */ 
  if (!strcmp(argv[0], "longdesc"))
  {
    ClapiVarType* pvar = NULL;
    
    if (argc==1)
    {
      printf("%s\n", clapi_appLongDesc_);
      return CLAPI_REQUESTEXIT;
    }

    pvar = findVarByID(argv[1]);
    if (pvar==NULL)
      return CLAPI_FATALERROR;

    printf("%s\n", pvar->longDesc_);
    return CLAPI_REQUESTEXIT;
  }

  /* -clapi test */ 
  /* Parses everything, displays variables and their values , but doesn't do anything further (i.e.: quits) */ 
  if (!strcmp(argv[0], "test"))
  {
    int retVal = CLAPI_REQUESTEXIT;
    unsigned int i=0;
    
    retVal = processClapiArgs(argc-1, argv+1);
    if (retVal !=CLAPI_OK)
      return retVal;
      
    for (i=0; i<clapi_numVars; i++)
    {
      printf("%s=", clapi_vars[i].name_);
      showVarValue( &(clapi_vars[i]), 0 /* no xml */);
      printf("\n");
    }
    
    return CLAPI_REQUESTEXIT;
  }

  /* -clapi testvar  VARID */ 
  /* Parses everything, displays value of variable specified by VARID, but doesn't do anything further (i.e.: quits) */ 
  if (!strcmp(argv[0], "testvar"))
  {
    ClapiVarType* pvar = interrogationHelper(argc, argv, "testvar");
    if (pvar==NULL)
      return CLAPI_FATALERROR;

    int retVal = CLAPI_REQUESTEXIT;
    retVal = processClapiArgs(argc-2, argv+2);
    if (retVal !=CLAPI_OK)
      return retVal;
      
    showVarValue(pvar, 0 /* no xml */);
    printf("\n");
    return CLAPI_REQUESTEXIT;
  }
  
  /* -clapi testxml */ 
  /* Parses everything, displays variables and their values in xml format, but doesn't do anything further (i.e.: quits) */ 
  if (!strcmp(argv[0], "testxml"))
  {
    int retVal = CLAPI_REQUESTEXIT;
    unsigned int i=0;
    unsigned short indentation=0;
    
    retVal = processClapiArgs(argc-1, argv+1);
    if (retVal !=CLAPI_OK)
      return retVal;
      
    printf("<clapi-values>\n");
    indentation++;

    for (i=0; i<clapi_numVars; i++)
    {
      writeXMLIndentation(indentation);
      printf("<%s>", clapi_vars[i].name_);
      showVarValue( &(clapi_vars[i]), 1 /* xml */);
      printf("</%s>\n", clapi_vars[i].name_);
    }
    
    printf("</clapi-values>\n");
    
    return CLAPI_REQUESTEXIT;
  }

  /* -clapi xml */ 
  /* Displays a complete overview of all variables (and app-wide descriptions) in xml format */ 
  if (!strcmp(argv[0], "xml"))
  {
    unsigned short indentation=0;
    unsigned int i=0;
    unsigned int j=0;
    char auxString[60];
    ClapiVarType* pvar=NULL;
    
    printf("<clapi>\n");
    indentation++;
    
    sprintf(auxString, "%u", clapi_numVars);
    writeXMLElement(indentation, "numvars", auxString);
    
    for (i=0; i<clapi_numVars; i++)
    {
      pvar = &clapi_vars[i];
      writeXMLIndentation(indentation);
      printf("<var name=\"%s\" type=\"%s\" index=\"%u\">\n", pvar->name_, clapi_typeStrings[pvar->type_], i);
      indentation++;
      switch (pvar->type_)
      {
        case CLAPI_VARTYPEINT :
          sprintf(auxString, "%d", pvar->varInt_.default_);
          writeXMLElement(indentation, "default", auxString);
          sprintf(auxString, "%d", pvar->varInt_.min_);
          writeXMLElement(indentation, "min", auxString);
          sprintf(auxString, "%d", pvar->varInt_.max_);
          writeXMLElement(indentation, "max", auxString);
          break;
        case CLAPI_VARTYPEDOUBLE :
          sprintf(auxString, "%g", pvar->varDouble_.default_);
          writeXMLElement(indentation, "default", auxString);
          sprintf(auxString, "%g", pvar->varDouble_.min_);
          writeXMLElement(indentation, "min", auxString);
          sprintf(auxString, "%g", pvar->varDouble_.max_);
          writeXMLElement(indentation, "max", auxString);
          break;
        case CLAPI_VARTYPESTRING :
          sprintf(auxString, "%s", pvar->varString_.default_);
          writeXMLElement(indentation, "default", auxString);
          sprintf(auxString, "%u", pvar->varString_.maxLength_);
          writeXMLElement(indentation, "maxlength", auxString);
          break;
        case CLAPI_VARTYPECHOICE :
          sprintf(auxString, "%u", pvar->varChoice_.default_);
          writeXMLElement(indentation, "default", auxString);
          sprintf(auxString, "%u", pvar->varChoice_.numChoices_);
          writeXMLElement(indentation, "numchoices", auxString);
          for (j=0; j<pvar->varChoice_.numChoices_; j++)
          {
            sprintf(auxString, "%s", pvar->varChoice_.choices_[j]);
            writeXMLElement(indentation, "choice", auxString);
          }
          break;
        default:
          clapi_assert( (pvar->type_<CLAPI_NUMTYPES), ("Unknown variable type (%d)", pvar->type_) );
      }
      sprintf(auxString, "%s", pvar->dimension_);
      writeXMLElement(indentation, "dimension", auxString);
      sprintf(auxString, "%s", pvar->shortDesc_);
      writeXMLElement(indentation, "shortdesc", auxString);
      sprintf(auxString, "%s", pvar->longDesc_);
      writeXMLElement(indentation, "longdesc", auxString);
      indentation--;
      writeXMLIndentation(indentation);
      printf("</var>\n");
    }

    sprintf(auxString, "%s", clapi_appShortDesc_);
    writeXMLElement(indentation, "shortdesc", auxString);
    sprintf(auxString, "%s", clapi_appLongDesc_);
    writeXMLElement(indentation, "longdesc", auxString);
    
    printf("</clapi>\n");
    return CLAPI_REQUESTEXIT;
  }

  fprintf(stderr, "Error: Unknown command: '%s'\n", argv[0]);
  return CLAPI_SYNTAXERROR;
}

/**********/ 
int clapi_parseCommandLine(int argc, char *argv[])
{
  int retval = CLAPI_OK;
  int i=0;
  
  assert(argc>0);
  assert(argv!=NULL);
  assert(clapi_vars!=NULL);
  
  if ( (argc>1) && !strcmp(argv[1], "-clapi") )
    retval = doInterrogationMode(argc-2, argv+2);
  else
    retval = processClapiArgs(argc-1, argv+1);
  
  free(clapi_vars);
  clapi_vars = NULL;
  clapi_numVars = 0;
  
  for (i=0; i<clapi_numHeapStrings; i++)
    free(clapi_heapStrings[i]);
  free(clapi_heapStrings);
  clapi_heapStrings = NULL;
  clapi_numHeapStrings = 0;
  
#if defined CLAPI_TESTFORCEEXIT  
  if (retVal==CLAPI_REQUESTEXIT)
    exit(CLAPI_OK);
#endif
  
  return retval;
}

/**********/
int clapi_init()
{
  if ( (clapi_vars!=NULL) && (clapi_heapStrings!=NULL) )
    return CLAPI_OK;
    
  if (clapi_vars==NULL) 
    clapi_vars = malloc( sizeof(ClapiVarType) * clapi_maxNumVars );
    
  if (clapi_heapStrings==NULL) 
    clapi_heapStrings = malloc( sizeof(char*) * clapi_maxNumHeapStrings );
    
  if (clapi_plusFiles==NULL) 
    clapi_plusFiles = malloc( sizeof(char*) * clapi_maxNumPlusFiles );
    
  clapi_appShortDesc_ = "";
  clapi_appLongDesc_ = "";

    
  return ( (clapi_vars!=NULL) && (clapi_heapStrings!=NULL) ) ? CLAPI_OK : CLAPI_OUTOFMEMORY;
}

/**********/
int addVarOK(const char* name, void* pvalue)
{
  int i=0;
  
  assert(name!=NULL);
  assert(pvalue!=NULL);
  assert(clapi_vars!=NULL);
  
  clapi_assert ( ( clapi_numVars < clapi_maxNumVars ), ("Number of variables exceeded maximum (%u)", clapi_maxNumVars) );
  clapi_assert ( (hasAlpha(name)), ("The variable name ('%s') does not contain any alphabetic characters", name) );
  clapi_assert ( (!hasNonPrintable(name)), ("The variable name ('%s') contains non-printable characters", name) );
  
  clapi_assert ( (!hasQuotes(name)), ("The variable name ('%s') contains quote characters (\" or ')\n", name) );
  clapi_assert ( (findVarByName(name)==NULL), ("A variable named '%s' already exists\n", name) );

  for (i=0; i<clapi_numVars; i++)
    clapi_assert ( (clapi_vars[i].varInt_.pvalue_ != pvalue), ("A variable named '%s' shares the same value pointer with '%s'\n", clapi_vars[i].name_, name) );

  return 1;
}

/**********/
ClapiVarType* clapi_addVarInt(const char* name, int* pvalue, int defaultvalue, int min, int max)
{
  ClapiVarType* pvar = NULL;
  
  if (!addVarOK(name, pvalue))
    return NULL;
  
  clapi_assert( (min < max), ("Variable '%s': min (%d) >= max (%d) \n", name, min, max) );
  
  pvar = &(clapi_vars[clapi_numVars]);
  pvar->type_ = CLAPI_VARTYPEINT;
  pvar->name_ = name;
  
  pvar->shortDesc_ = "";
  pvar->longDesc_ = "";
  pvar->dimension_ = "";
  
  pvar->varInt_.pvalue_ = pvalue;
  *(pvar->varInt_.pvalue_) = defaultvalue;
  pvar->varInt_.default_ = defaultvalue;
  pvar->varInt_.min_ = min;
  pvar->varInt_.max_ = max;
  
  clapi_numVars++;
  
  return pvar;
}

/**********/
ClapiVarType* clapi_addVarDouble(const char* name, double* pvalue, double defaultvalue, double min, double max)
{
  ClapiVarType* pvar = NULL;
  
  if (!addVarOK(name, pvalue))
    return NULL;
  
  clapi_assert ( (min < max), ("Variable '%s': min (%g) >= max (%g) \n", name, min, max) );
  
  pvar = &(clapi_vars[clapi_numVars]);
  pvar->type_ = CLAPI_VARTYPEDOUBLE;
  pvar->name_ = name;
  
  pvar->shortDesc_ = "";
  pvar->longDesc_ = "";
  pvar->dimension_ = "";
  
  pvar->varDouble_.pvalue_ = pvalue;
  *(pvar->varDouble_.pvalue_) = defaultvalue;
  pvar->varDouble_.default_ = defaultvalue;
  pvar->varDouble_.min_ = min;
  pvar->varDouble_.max_ = max;
  
  clapi_numVars++;
  
  return pvar;
}

/**********/
ClapiVarType* clapi_addVarString(const char* name, const char (*pvalue)[], const char* defaultValue, unsigned int maxLength)
{
  ClapiVarType* pvar = NULL;
    
  if (!addVarOK(name, pvalue))
    return NULL;
  
  pvar = &(clapi_vars[clapi_numVars]);
  pvar->type_ = CLAPI_VARTYPESTRING;
  pvar->name_ = name;
  
  pvar->shortDesc_ = "";
  pvar->longDesc_ = "";
  pvar->dimension_ = "";
  
  pvar->varString_.pvalue_ = (const char**)pvalue;
  *(pvar->varString_.pvalue_) = defaultValue;
  pvar->varString_.default_ = defaultValue;
  pvar->varString_.maxLength_ = maxLength;
  
  clapi_numVars++;
  
  return pvar;
}

/**********/
ClapiVarType* clapi_addVarChoice(const char* name, unsigned int* pvalue, unsigned int defaultValue, unsigned int numChoices, const char* choices[])
{
  ClapiVarType* pvar = NULL;
  
  if (!addVarOK(name, pvalue))
    return NULL;
  
  pvar = &(clapi_vars[clapi_numVars]);
  pvar->type_ = CLAPI_VARTYPECHOICE;
  pvar->name_ = name;
  
  pvar->shortDesc_ = "";
  pvar->longDesc_ = "";
  pvar->dimension_ = "";
  
  pvar->varChoice_.pvalue_ = pvalue;
  *(pvar->varChoice_.pvalue_) = defaultValue;
  pvar->varChoice_.default_ = defaultValue;
  pvar->varChoice_.numChoices_ = numChoices;
  pvar->varChoice_.choices_ = choices;
  
  clapi_numVars++;
  
  return pvar;
}

/**********/
void clapi_varSetShortDesc(ClapiVarType* pvar, const char* shortDesc)
{
  assert(pvar!=NULL);
  assert(shortDesc!=NULL);
  pvar->shortDesc_ = shortDesc;
}

/**********/
void clapi_varSetLongDesc(ClapiVarType* pvar, const char* longDesc)
{
  assert(pvar!=NULL);
  assert(longDesc!=NULL);
  pvar->longDesc_ = longDesc;
}

/**********/
void clapi_varSetDimension(ClapiVarType* pvar, const char* dimension)
{
  assert(pvar!=NULL);
  assert(dimension!=NULL);
  pvar->dimension_ = dimension;
}

/**********/
void clapi_appSetShortDesc(const char* shortDesc)
{
  assert(shortDesc!=NULL);
  clapi_appShortDesc_ = shortDesc;
}

/**********/
void clapi_appSetLongDesc(const char* longDesc)
{
  assert(longDesc!=NULL);
  clapi_appLongDesc_ = longDesc;
}
