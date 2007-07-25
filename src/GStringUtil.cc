/*                       
	This file is part of the GVars3 Library.

	Copyright (C) 2005 The Authors

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 
    51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "gvars3/GStringUtil.h"

using namespace std;

namespace GVars3
{

string UncommentString(string s)
{
  //int n = s.find("//");
  //return s.substr(0,n);

  int q=0;

  for(int n=0; n < s.size(); n++)
  {
  	if(s[n] == '"')
		q = !q;

	if(s[n] == '/' && !q)
	{
		if(n < s.size() -1 && s[n+1] == '/')
			return s.substr(0, n);
	}
  }

  return s;
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
