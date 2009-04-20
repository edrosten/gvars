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

	istream& from_stream(istream& in, string& s)
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
		
		//This variable holds an escape sequence in progress.
		//empty means no escaping.
		string escape;

		for(;;)
		{
			c = in.get();
			if(c == EOF || (quoted && escape.empty() && c == '"'))
				break;
			else if(escape.empty() && c == '\\')
				escape = "\\";
			else if(!escape.empty())
				escape += c;
			else
				s += c;

			//Check escapes
			if(escape == "\\\\")
			{
				s+="\\"; 
				escape.clear();
			}
			else if(escape == "\\n")
			{
				s+="\n";
				escape.clear();
			}
		}
		
		//Append any trailing parts of an escape sequence
		s += escape;

		return in;
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
