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

#ifndef GV3_INV_GVARS3_H
#define GV3_INV_GVARS3_H
#include <map>
#include <set>
#include <string>
#include <list>
#include <vector>
#include <iostream>

#include <TooN/TooN.h>

#include <gvars3/type_name.h>
#include <gvars3/serialize.h>

namespace GVars3
{
void parse_warning(int e, std::string type, std::string name, std::string from);

struct type_mismatch{};


class GV3;


class BaseMap
{
	public:
		virtual std::string get_as_string(const std::string& name)=0;
		virtual int set_from_string(const std::string& name, const std::string& val)=0;
		virtual std::string name()=0;
		virtual std::vector<std::string> list_tags()=0;
		virtual ~BaseMap(){};
};

template<class T> class gvar2
{
  friend class GV3;
 public:
  gvar2() {data=NULL;}      // Debugging comfort feature, makes unregistered vars more obvious.
  T& operator*()
    {
      return *data;
    }
  
  const T & operator*() const 
  {
     return *data;
  }
  
  T* operator->()
    {
      return data;
    }
  const T * operator->() const 
  {
    return data;
  }
  
 protected:
  T* data;
};


typedef gvar2<double> gvar2_double;
typedef gvar2<int> gvar2_int;
typedef gvar2<std::string> gvar2_string;
typedef gvar2<TooN::Vector<-1> > gvar2_Vector;
typedef gvar2<TooN::Matrix<-1> > gvar2_Matrix;

template<class T> class gvar3: public gvar2<T>
{
	friend class GV3;
	public:
	inline gvar3(const std::string& name, const T& default_val = T(), bool silent=false);
	inline gvar3(const std::string& name, const std::string& default_val, bool silent);
	inline gvar3(){};
};

template<> class gvar3<std::string>: public gvar2<std::string>
{
	friend class GV3;
	public:
	inline gvar3(const std::string& name, const std::string& default_val = "", bool silent=false);
	inline gvar3(){};
};


class GV3
{
	private:

		template<class T> class TypedMap: public BaseMap
		{
			private:
				friend class GV3;

				//This gives us singletons
				static TypedMap& instance()
				{
					static TypedMap* inst=0;

					if(!inst)
					{
						inst = new TypedMap();
						//Register ourselves with GV3
						GV3::add_typemap(inst);
					}

					return *inst;
				}

				//Get a data member	
				T* get(const std::string& n)
				{
					typename std::map<std::string, T>::iterator i;

					i = data.find(n);

					if(i == data.end())
						return NULL;
					else
						return &(i->second);
				}

				//Create a data member
				T* create(const std::string& n)
				{
					data[n] = T();
					return &data[n];
				}
			
				virtual int set_from_string(const std::string& name, const std::string& val)
				{
					return	serialize::from_string(val, data[name]); 
				}

				virtual std::string get_as_string(const std::string& name)
				{
					return serialize::to_string(data[name]);
				}

				virtual std::string name()
				{
					return type_name<T>();
				}

				virtual std::vector<std::string> list_tags()
				{
					std::vector<std::string> l;
					for(typename std::map<std::string,T>::iterator i=data.begin(); i != data.end(); i++)
						l.push_back(i->first);
					return l;
				}

				std::map<std::string, T>		data;
		};

		template<class T> friend class TypedMap;

		template<class T> static T* attempt_get(const std::string& name)
		{
			T* d = TypedMap<T>::instance().get(name);
			
			if(!d)	 //Data not present in map of the correct type
			{
				//Does it exist with a different type?
				if(typeof_tags.count(name))
				{		//Yes: programmer error.
					std::cerr << "GV3:Error: type mismatch while getting " << type_name<T>() << " " << name << ": already registered "
							"as type " << typeof_tags[name]->name() << ". Fix your code.\n";

					throw type_mismatch();
				}
				else
					return NULL;
			}

			return d;
		}

		static void add_typemap(BaseMap* m);

		static std::map<std::string, std::string>		unmatched_tags;
		static std::map<std::string, BaseMap*>			typeof_tags;
		static std::list<BaseMap*>						maps;

		
		template<class T> static T* get_by_val(const std::string& name, const T& default_val, bool silent);
		template<class T> static T* get_by_str(const std::string& name, const std::string& default_val, bool silent);
		template<class T> static T* register_new_gvar(const std::string &name, const T& default_val, bool silent);

	public:
		//Get references by name
		template<class T> static T& get(const std::string& name, const T& default_val=T(), bool silent=false);
		template<class T> static T& get(const std::string& name, std::string default_val, bool silent=false);
		
		//Register GVars
		template<class T> static void Register(gvar2<T>& gv, const std::string& name, const T& default_val=T(), bool silent=false);
		template<class T> static void Register(gvar2<T>& gv, const std::string& name, const std::string& default_val, bool silent=false);
		static inline void Register(gvar2<std::string>& gv, const std::string& name, const std::string& default_val="", bool silent=false);

		//Get and set by string only
		static std::string get_var(std::string name);
		static bool set_var(std::string name, std::string val, bool silent=false);

		//Some helper functions
		static void print_var_list(std::ostream& o);
		static std::vector<std::string> tag_list();
};



template<class T> gvar3<T>::gvar3(const std::string& name, const T& default_val, bool silent)
{
	GV3::Register(*this, name, default_val, silent);
}

template<class T> gvar3<T>::gvar3(const std::string& name, const std::string& default_val, bool silent)
{
	GV3::Register(*this, name, default_val, silent);
}
gvar3<std::string>::gvar3(const std::string& name, const std::string& default_val, bool silent)
{
	GV3::Register(*this, name, default_val, silent);
}


#include <gvars3/gv3_implementation.hh>

//Compatibility with old GVARS
class GVars2
{
	public:
		template<class T> void Register(gvar2<T>& gv, const std::string& name, const T& default_val=T(), bool silent=false)
		{ 
			GV3::Register(gv, name, default_val, silent);
		}

		template<class T> void Register(gvar2<T>& gv, const std::string& name, const std::string& default_val, bool silent=false)
		{
			GV3::Register(gv, name, default_val, silent);
		}

		inline void Register(gvar2<std::string>& gv, const std::string& name, const std::string& default_val="", bool silent=false)
		{
			GV3::Register(gv, name, default_val, silent);
		}

		template<class T> T& Get(const std::string& name, const T& default_val=T(),  bool silent=false)
		{
			return GV3::get<T>(name, default_val, silent);
		}

		template<class T> T& Get(const std::string& name, const std::string& default_val="", bool silent=false)
		{
			return GV3::get<T>(name, default_val, silent);
		}

		inline std::string& Get(const std::string& name, const std::string& default_val="", bool silent=false)
		{
			return GV3::get<std::string>(name, default_val, silent);
		}

		void SetVar(std::string sVar, std::string sValue, bool silent=false);
		void SetVar(std::string s);


		int& GetInt(const std::string& name, int default_val=0, bool silent = false);
		double& GetDouble(const std::string& name, double default_val=0.0, bool silent = false); 
		std::string& GetString(const std::string& name, const std::string& default_val="", bool silent = false); 
		TooN::Vector<>& GetVector(const std::string& name, const TooN::Vector<>& default_val=TooN::Vector<>(), bool silent = false); 
		TooN::Matrix<>& GetMatrix(const std::string& name, const TooN::Matrix<>& default_val=TooN::Matrix<>(), bool silent = false); 


		int& GetInt(const std::string& name, const std::string& default_val, bool silent = false);
		double& GetDouble(const std::string& name, const std::string& default_val, bool silent = false); 
		TooN::Vector<>& GetVector(const std::string& name, const std::string& default_val, bool silent = false); 
		TooN::Matrix<>& GetMatrix(const std::string& name, const std::string& default_val, bool silent = false);  

		std::string StringValue(const std::string &name, bool no_quotes=false);
		void PrintVarList(std::ostream& os=std::cout);
		void PrintVar(std::string s, std::ostream& os, bool bEndl = true);

		//char* ReadlineCommandGenerator(const char *szText, int nState);

	private:
};

}
#endif
