/*                       
	This file is part of the GVars3 Library.

	Copyright (C) 2005 The Authors

	This library is free software, see LICENSE file for details
*/

#include "gvars3/serialize.h"
#include <vector>

using namespace std;


namespace GVars3
{
namespace serialize
{
	std::string to_string(const std::string& s, bool)
	{
		ostringstream os;
		os << "\"";
		
		for(size_t i=0; i < s.size(); i++)
		{
			if(s[i] == '\\')
				os << "\\\\";
			else if(s[i] == '\n')
				os << "\\n";
			else
				os << s[i];
		}

		os << "\"";
		return os.str();
	}

	string FromStream<string>::from(istream& in)
	{	
		string s;

		bool quoted=0;
		int c;

		//Eat whitespace
		in >> ws;

		if((c=in.get()) == EOF)
			return s;

		if(c == '"')
			quoted=1;
		else
			in.unget();
		
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

		return s;
	}

	int check_stream(std::istream& i)
	{
		if(i.good())
			return 0;

		if(i.bad() || (i.fail() && !i.eof()))
		{
			return -i.tellg();
		}
		return 0;
	}

}


}
