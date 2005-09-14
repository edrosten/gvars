#ifndef GV3_INC_SERIALIZE_H
#define GV3_INC_SERIALIZE_H

#include <string>
#include <sstream>
#include <TooN/TooN.h>

namespace GVars3
{
	namespace serialize
	{
		/// Checks a stream and returns a statis code
		/// @param input stream to check.
		int check_stream(std::istream& i);
			
		//Define a serializer for everything that works with iostreams
		//override to add new types with unusual serializers
		template<class T> std::string to_string(const T& val)
		{
			std::ostringstream o;
			o << val;
			return o.str();
		}

		template<class T> int from_string(std::string s, T& result)
		{	
			std::istringstream i(s);
			i >> result;
			return check_stream(i);
		}


		template<int N> std::string to_string(const TooN::Vector<N>& m)
		{
			std::ostringstream o;
			o << "[ ";
			o << m;
			o << "]";
			return o.str();
		}

		std::string to_string(const std::string& s);
		int from_string(std::string s, std::string& so);

		std::string to_string(const TooN::Matrix<>& m);
		int from_string(std::string s, TooN::Matrix<>& m);
		std::string to_string(const TooN::Vector<>& m);
		int from_string(std::string s, TooN::Vector<>& m);
	}
}


#endif
