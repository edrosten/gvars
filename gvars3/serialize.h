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
			for(int i=0; i<m.size(); i++)
			  o << m[i] << " ";
			o << "]";
			return o.str();
		}

		template<int N> int from_string(std::string s, TooN::Vector<N>& m);

		int from_string(std::string s, TooN::Vector<>& m);
		template<int N> int from_string(std::string s, TooN::Vector<N>& m)
		{
			TooN::Vector<> t;
			int result = from_string(s, t);
			if( result || t.size() != N )
				return 1;
			m = t;
			return 0;
		}

		std::string to_string(const std::string& s);
		int from_string(std::string s, std::string& so);

		std::string to_string(const TooN::Matrix<>& m);
		template<int N, int M> std::string to_string(const TooN::Matrix<N,M>& m){
            TooN::Matrix<> t = m;
            return to_string(t);
        }
		int from_string(std::string s, TooN::Matrix<>& m);
		template<int N, int M> int from_string(std::string s, TooN::Matrix<N,M>& m)
		{
		  TooN::Matrix<> t;
		  int result = from_string(s,t);
		  if( result || t.num_rows()!= N || t.num_cols()!= M )
		    return 1;
		  m = t;
		  return 0;
		}
		std::string to_string(const TooN::Vector<>& m);
	}
}


#endif
