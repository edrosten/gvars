#ifndef GV3_INC_SERIALIZE_H
#define GV3_INC_SERIALIZE_H

#include <string>
#include <sstream>
#include <TooN/TooN.h>

namespace GVars3
{
	namespace serialize
	{
		//Define a serializer for everything that works with iostreams
		//override to add new types with unusual serializers
		template<class T> std::string to_string(const T& val)
		{
			std::ostringstream o;
			o << val;
			return o.str();
		}

		template<class T> bool from_string(std::string s, T& result)
		{	
			std::istringstream i(s);
			i >> result;
			return !i.bad();
		}

		std::string to_string(const TooN::Matrix<>& m);
		bool from_string(std::string s, TooN::Matrix<>& m);
		std::string to_string(const TooN::Vector<>& m);
		bool from_string(std::string s, TooN::Vector<>& m);

	}
}


#endif
