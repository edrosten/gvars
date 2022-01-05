/*                       
	This file is part of the GVars3 Library.

	Copyright (C) 2005 The Authors

	This library is free software, see LICENSE file for details
*/

#ifndef GV3_INC_GSTRINGUTIL_H
#define GV3_INC_GSTRINGUTIL_H

#include <vector>
#include <string>
#include <sstream>
#include <gvars3/serialize.h>

namespace GVars3
{

	std::string UncommentString(std::string s);
	std::vector<std::string> ChopAndUnquoteString(std::string s);
	template <class T> T* ParseAndAllocate(std::string s)
	{
		std::istringstream is(s);	
		T* n = new T(serialize::from_stream<T>(is));
		return n;
	}

}



#endif
