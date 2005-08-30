#ifndef GV3_INC_GSTRINGUTIL_H
#define GV3_INC_GSTRINGUTIL_H

#include <vector>
#include <string>
#include <gvars3/serialize.h>

namespace GVars3
{

	std::string UncommentString(std::string s);
	std::vector<std::string> ChopAndUnquoteString(std::string s);
	template <class T> T* ParseAndAllocate(std::string s)
	{
		T* n = new T;
		serialize::from_string(s, *n);
		return n;
	}

}



#endif
