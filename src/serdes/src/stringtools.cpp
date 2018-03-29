#if !defined WIN32
#include <ctype.h>
#endif

#include <iostream>
#include <sstream>
#include <cstring>
#include "stringtools.hpp"

using namespace std;

namespace blib {


/*  */
bool IsWhiteSpace(char ch)
{
  return ( isspace(ch) || ch==0x1a );
}

/*  */
string TrimWhiteSpaces(const string& argString)
{
    string::size_type i,j;

    // Look for first non-whitespace character
    for (i=0; i<argString.length(); i++)
        if (!IsWhiteSpace(argString[i]))
            break;

    // Return empty string if argString contains nothing but whitespaces
    if (i==argString.length())
        return "";

    // Look for last non-whitespace character
    for (j=argString.length()-1; j>=0; j--)
        if (!IsWhiteSpace(argString[j]))
            break;

    return argString.substr(i, j-i+1);
}

/*  */
bool FindString(istream& inputStream, const string& theString, streampos& begin, bool caseSensitive)
{
  char ch='\0';
  char *stringChar=(char*)theString.c_str();
  streampos initialPos = inputStream.tellg();

  while (inputStream.good())
  {
    /* store starting point of search */
    streampos beginPos = inputStream.tellg();

    /* Advance through string & stream and see if characters match */
    while (*stringChar)
    {
      inputStream.get(ch);

      /* If characters match at one position, keep advancing */
      if ( caseSensitive ? (ch==(*stringChar)) : toupper(ch)==toupper(*stringChar) )
        stringChar++;

      /* If they don't match ... */
      else
        /* ... and if the end of the stream is reached, this function fails. */
        if (inputStream.eof())
        {
          inputStream.clear();
          inputStream.seekg(initialPos);
          return false;
        }
        /*  ... if the end of the stream is not reached yet, break out of the advancement loop ... */
        else
          break;

    } /* while (*stringChar) */

    /* ... and repeat the loop starting one character further than the previous time */
    if (*stringChar)
    {
      stringChar = (char*)theString.c_str();
      inputStream.seekg(beginPos+(streampos)1L);
    }

    /* Graceful termination of "while (*stringChar)"-loop. This function succeeded. */
    else
    {
      begin = beginPos;
      return true;
    }

  } /* while (inputStream.good()) */

  /* The string was not found, the end of the stream was not reached, */
  /* but on the way some other error condition occurred. */
  inputStream.seekg(initialPos);
  return false;
}

/*  */
bool FindString(istream& inputStream, const string& theString, bool caseSensitive)
{
  streampos begin;
  return FindString(inputStream, theString, begin, caseSensitive);
}

/*  */
bool SkipToNextLine(istream& inputStream)
{
  while ( (!inputStream.eof()) && (inputStream.get()!='\n') );

  return (!inputStream.eof());
}

/*  */
bool SkipPast(istream& inputStream, const string& theString, bool caseSensitive)
{
  streampos begin;

  return FindString(inputStream, theString, begin, caseSensitive);
}

/*  */
bool SkipPast(istream& inputStream, char theChar, bool caseSensitive)
{
  if (caseSensitive)
  {
//    char ch;
    while ( (!inputStream.eof()) && (inputStream.get()!=theChar) );
  }
  else
    while ( (!inputStream.eof()) && (tolower(inputStream.get())!=tolower(theChar)) );

  return (!inputStream.eof());
}

/*  */
bool SkipWhiteSpace(istream& inputStream)
{
  while ( (!inputStream.eof()) && (IsWhiteSpace(inputStream.peek())) )
    inputStream.get();

  return (!inputStream.eof());
}

/*  */
bool ReadUpTo(istream& inputStream, const string& stringToFind, string& readString, bool caseSensitive)
{
  streampos origPos = inputStream.tellg();
  streampos beginString;

  if (FindString(inputStream, stringToFind, beginString, caseSensitive))
  {
    readString.resize(beginString-origPos);
    inputStream.seekg(origPos);
    inputStream.read((char*)readString.c_str(), readString.length());
    return true;
  }

  return false;
}

/*  */
bool ReadUpTo(istream& inputStream, char charToFind, string& readString, bool caseSensitive)
{
  streampos begin = inputStream.tellg();

  if (SkipPast(inputStream, charToFind, caseSensitive))
  {
    streampos end = inputStream.tellg();
    readString.resize(end-begin-1L);
//    unsigned long test1 = end-begin-1L;
//    unsigned long test2 = readString.length();
    inputStream.seekg(begin);
    inputStream.read((char*)readString.c_str(), readString.length());
    return true;
  }

  return false;
}

/*  */
bool ReadLine(istream& inputStream, string& readString, bool caseSensitive)
{
  if (inputStream.eof())
    return false;

  streampos begin = inputStream.tellg();

  if (SkipToNextLine(inputStream))
  {
    streampos end = inputStream.tellg();
    readString.resize(end-begin-1L);
    inputStream.seekg(begin);
    inputStream.read((char*)readString.c_str(), readString.length());
    SkipToNextLine(inputStream);
    return true;
  }
  else
  {
    inputStream.clear();
    inputStream.seekg(0, ios::end);
    streampos end = inputStream.tellg();
    readString.resize(end-begin);
    inputStream.seekg(begin);
    inputStream.read((char*)readString.c_str(), readString.length());
    return true;
  }

  return false;
}


/*  */
bool ReadValueString(istream& inputStream, const string& name, string& valueString)
{
/*  if (FindString(inputStream, name))
    if (SkipPast(inputStream, '='))
      if (SkipWhiteSpace(inputStream))
        return (ReadLine(inputStream, valueString));
*/

  if (SkipWhiteSpace(inputStream))
  {
    string lineString;
    while (ReadLine(inputStream, lineString))
    {
      istringstream lineStream(lineString);
      SkipWhiteSpace(lineStream);
      streampos initialPos = lineStream.tellg();
      streampos stringPos;
      if (FindString(lineStream, name, stringPos))
        if (stringPos==initialPos)
          if (SkipPast(lineStream, '='))
            if (SkipWhiteSpace(lineStream))
              return (ReadLine(lineStream, valueString));
    }
  }

  return false;
}


/*
** Jack Handy's (sic) wildcard string compare function
** Taken from "The Code Project", http://www.codeproject.com/string/wildcmp.asp
*/
int wildcmp(const char *wild, const char *string)
{
	const char *cp, *mp;

	while ((*string) && (*wild != '*')) {
		if ((*wild != *string) && (*wild != '?')) {
			return 0;
		}
		wild++;
		string++;
	}

	while (*string) {
		if (*wild == '*') {
			if (!*++wild) {
				return 1;
			}
			mp = wild;
			cp = string+1;
		} else if ((*wild == *string) || (*wild == '?')) {
			wild++;
			string++;
		} else {
			wild = mp;
			string = cp++;
		}
	}

	while (*wild == '*') {
		wild++;
	}

	return !*wild;
}

/*  */
bool MatchesWildcard(const string& aString, const string& format, bool caseSensitive)
{
  if (caseSensitive)
    return ( wildcmp(format.c_str(), aString.c_str()) ? true : false );
  else
  {
    string newString = GetLowerCase(aString);
    string newFormat = GetLowerCase(format);
    return ( wildcmp(newFormat.c_str(), newString.c_str()) ? true : false );
  }
}

/*  */
void SplitString(const string& aString, const string& delimiters, StringList& stringList)
{
  /* Avoid in-place modification of argument string */
  char* tempString = new char[aString.size()+1];
  memcpy(tempString, aString.c_str(), aString.size()+1);

  char* token = strtok(tempString, delimiters.c_str());
  while (token)
  {
    stringList.push_back(token);
    token = strtok(0, delimiters.c_str());
  }

  delete [] tempString;
}

/*  */
string ConcatString(const StringList& stringList, const string& delimiter)
{
  string theString;
  for (StringList::const_iterator iter = stringList.begin(); iter!=stringList.end(); iter++)
    theString += *iter + delimiter;

  theString = theString.substr(0, theString.length()-delimiter.length());

  return theString;
}

/*  */
string GetLowerCase(const string& aString)
{
  string newString = aString;

#if defined _MSC_VER
  ctype<char> theFacet;
  theFacet.tolower( (char*)newString.c_str(), (char*)newString.c_str()+newString.size() );
#else
  string::size_type index = newString.size();
  while (index--)
    newString[index] = tolower(newString[index]);
#endif

  return newString;
}

/*  */
string GetUpperCase(const string& aString)
{
  string newString = aString;

#if defined _MSC_VER
  ctype<char> theFacet;
  theFacet.toupper( (char*)newString.c_str(), (char*)newString.c_str()+newString.size() );
#else
  string::size_type index = newString.size();
  while (index--)
    newString[index] = toupper(newString[index]);
#endif

  return newString;
}



#if 0

/* Base64 enoding/decoding taken from:
    http://www.experts-exchange.com/Programming/Programming_Languages/C/Q_20245582.html
*/

// TODO: move base64 stuff to a Codec class

/*  */
enum
{
  BASE64_OK = 0,
  BASE64_FAIL = -1,
  BASE64_BUFOVER = -2,
};

/*  */
static char basis_64[] =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/???????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????";

/*  */
static char index_64[128] = {
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
  52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
  -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
  15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
  -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
  41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1
};

/*  */
char Char64(char ch)
{
  return (((ch) < 0 || (ch) > 127) ? -1 : index_64[(ch)]);
}

/*  */
int encode64(const char *_in, unsigned inlen, char *_out, unsigned outmax, unsigned *outlen)
{

  const unsigned char *in = (const unsigned char *) _in;
  unsigned char  *out = (unsigned char *) _out;
  unsigned char   oval;
  char           *blah;
  unsigned        olen;

  olen = (inlen + 2) / 3 * 4;
  if (outlen)
      *outlen = olen;
  if (outmax < olen)
      return BASE64_BUFOVER;

  blah = (char *) out;
  while (inlen >= 3)
  {
      /* user provided max buffer size; make sure we don't go over it */
      *out++ = basis_64[in[0] >> 2];
      *out++ = basis_64[((in[0] << 4) & 0x30) | (in[1] >> 4)];
      *out++ = basis_64[((in[1] << 2) & 0x3c) | (in[2] >> 6)];
      *out++ = basis_64[in[2] & 0x3f];
      in += 3;
      inlen -= 3;
  }
  if (inlen > 0)
  {
      /* user provided max buffer size; make sure we don't go over it */
      *out++ = basis_64[in[0] >> 2];
      oval = (in[0] << 4) & 0x30;
      if (inlen > 1)
          oval |= in[1] >> 4;
      *out++ = basis_64[oval];
      *out++ = (inlen < 2) ? '=' : basis_64[(in[1] << 2) & 0x3c];
      *out++ = '=';
  }

  if (olen < outmax)
      *out = '\0';

  return BASE64_OK;

}

/*  */
int decode64(const char *in, unsigned inlen, char *out, unsigned *outlen)
{
  unsigned        len = 0,
                  lup;
  int             c1,
                  c2,
                  c3,
                  c4;

  if (in[0] == '+' && in[1] == ' ')
      in += 2;

  if (*in == '\0')
      return BASE64_FAIL;

  for (lup = 0; lup < inlen / 4; lup++)
  {
      c1 = in[0];
      if (Char64(c1) == -1)
          return BASE64_FAIL;
      c2 = in[1];
      if (Char64(c2) == -1)
          return BASE64_FAIL;
      c3 = in[2];
      if (c3 != '=' && Char64(c3) == -1)
          return BASE64_FAIL;
      c4 = in[3];
      if (c4 != '=' && Char64(c4) == -1)
          return BASE64_FAIL;
      in += 4;
      *out++ = (Char64(c1) << 2) | (Char64(c2) >> 4);
      ++len;
      if (c3 != '=')
      {
          *out++ = ((Char64(c2) << 4) & 0xf0) | (Char64(c3) >> 2);
          ++len;
          if (c4 != '=')
          {
              *out++ = ((Char64(c3) << 6) & 0xc0) | Char64(c4);
              ++len;
          }
      }
  }

  *out = 0;
  *outlen = len;

  return BASE64_OK;
}

#endif /* 0 */

} /* namespace blib */

