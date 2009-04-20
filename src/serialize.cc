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

#include "gvars3/serialize.h"
#include <vector>

using namespace std;


namespace GVars3
{
namespace serialize
{
	std::string to_string(const std::string& s)
	{
		return s;
	}

	istream& get_string(istream& in, string& s)
	{	
		s.clear();

		bool quoted=0;	
		int c;

		//Eat whitespace
		in >> ws;

		if((c=in.get()) == EOF)
			return in;

		if(c == '"')
			quoted=1;



	}

        // For reading strings, if there's double quotes, lift the part inside the double quotes.
        int from_string(std::string s, std::string& so) //Pretty ugly code lifted from GVars2, but works.
	{
	  unsigned char c;
	  int nPos=0;
	  int nLength = s.length();
	  
	  // Find Start....
	  bool bFoundOpenQuote = false;
	  while((nPos < nLength) && !bFoundOpenQuote)
	    {
	      c = s[nPos];
	      if(c=='"') bFoundOpenQuote =true;
	      nPos++;
	    }
	  if(!bFoundOpenQuote) // No quotes found - assume that the entire thing is good to go...
	    {
	      so = s;
	      return 0;
	    }
	  // Found opening ". Copy until end, or closing "
	  
	  std::string sNew("");
	  for (; nPos < nLength; ++nPos) {
	      char c = s[nPos];
	      if (c == '"') {
		  ++nPos;
		  break;
	      }
	      if (nPos+1<nLength && c == '\\') {
		  char escaped = s[++nPos];
		  switch (escaped) {
		  case 'n': c = '\n'; break;
		  case 'r': c = '\r'; break;
		  case 't': c = '\t'; break;
		  default: c = escaped; break;
		  }
	      }
	      sNew.push_back(c);
	  }
	  if (nPos < nLength) {
	      string rest;
	      from_string(s.substr(nPos), rest);
	      sNew += rest;
	  }

	  so = sNew;
	  return 0;
	}
  

	int check_stream(std::istream& i)
	{
		if(i.bad())
			return 1;

		if(i.good())
		{
			i >> ws;
			char c = i.get();

			if(!i.eof())
			{	
				i.putback(c);
				return -i.tellg();
			}
		}

		return 0;
	}

}


}
