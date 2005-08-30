#ifndef GV3_INC_TYPE_NAME_H
#define GV3_INC_TYPE_NAME_H

#include <string>

namespace GVars3
{

	template <class T> std::string type_name() 
	{
	  static std::string funcname = std::string(__PRETTY_FUNCTION__);
	  static std::string bname = funcname.substr(funcname.rfind(" = ")+3);
	  static std::string name = bname.substr(0, bname.length()-1);
	  return name;
	}

	template<> inline std::string type_name<std::string>()
	{
		return "string";
	}

	template <class T> std::string type_name(const T& t)
	{
	  return type_name<T>();
	}

}



#endif

