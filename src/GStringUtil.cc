#include "gvars3/GStringUtil.h"

using namespace std;

namespace GVars3
{

string UncommentString(string s)
{
  int n = s.find("//");
  return s.substr(0,n);
};

vector<string> ChopAndUnquoteString(string s)
{
  vector<string> v;
  int nPos=0;
  int nLength = s.length();
  while(1)
    {
    string sTarget;
    char cDelim;
    // Get rid of leading whitespace:  
    while((nPos<nLength)&&(s[nPos]==' '))
      nPos++;
    if(nPos==nLength)
      return v;
    
    // First non-whitespace char...
    if(s[nPos]!='\"')
      cDelim=' ';
    else
      {
	cDelim = '\"';
	nPos++;
      }
    while((nPos<nLength)&&(s[nPos]!=cDelim))
      {
	sTarget+=(s[nPos]);
	nPos++;
      }
    v.push_back(sTarget);
    if(cDelim=='\"')
      nPos++;
    }
};

}
