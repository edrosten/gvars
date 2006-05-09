
// Replacement for operator= which doesn't crash for Vector<-1>; general case
template<class T> inline void robust_assignment(T& lvalue, T rhs)
{
  lvalue =rhs;
};

// Replacement for operator= which doesn't crash for Vector<-1>; specialisation
void inline robust_assignment(TooN::Vector<> &lvalue, TooN::Vector<> rhs)
{
  if(lvalue.size()!=rhs.size())
    lvalue.resize(rhs.size());
  lvalue = rhs;
}

// TODO: Make the specialisation for matrices as well. 


template<class T> T* GV3::register_new_gvar(const std::string& name, const T& default_val, bool silent)
{
  T* d ;
  std::map<std::string, std::string>::iterator i;
  
  i = unmatched_tags.find(name);
  
  d = TypedMap<T>::instance().create(name);
  typeof_tags[name] = &TypedMap<T>::instance();
  
  //Look to see if ``name'' has not already been set	
  if(i == unmatched_tags.end())
    {
      if(!silent)
	std::cerr << "GV3:Register " << type_name<T>() << " " << name << " undefined. Defaults to " 
		  << serialize::to_string(default_val) << std::endl;
      
      //	*d = default_val;   // This crashes with vector<-1> if sizes don't match, which they don't. Replace with:
      robust_assignment(*d, default_val);
    }
  else
    {
      int e = serialize::from_string(i->second, *d);
      parse_warning(e, type_name<T>(), name, i->second);
      
      unmatched_tags.erase(i);
    }
  return d;
}


template<class T> T* GV3::get_by_val(const std::string& name, const T& default_val, bool silent)
{
  T* d = attempt_get<T>(name);
  if(!d)
    d = register_new_gvar(name, default_val, silent);
  return d;
}

template<class T> T* GV3::get_by_str(const std::string& name, const std::string& default_val, bool silent)
{
  T* d = attempt_get<T>(name);
  if(d!=NULL) return d;
  
  T	def=T();
  int e = serialize::from_string(default_val, def);
  parse_warning(e, type_name<T>(), name, default_val);
  
  return register_new_gvar(name, def, silent);
}

template<>inline std::string& GV3::get<std::string>(const std::string& name, std::string default_val, bool silent)
{
	return *get_by_val(name, default_val, silent);
}

template<class T> T& GV3::get(const std::string& name, std::string default_val, bool silent)
{
	return *get_by_str<T>(name, default_val, silent);
}

template<class T> T& GV3::get(const std::string& name, const T& default_val, bool silent)
{
	return *get_by_val(name, default_val, silent);
}



template<class T> void GV3::Register(gvar2<T>& gv, const std::string& name, const T& default_val, bool silent)
{
	gv.data = get_by_val(name, default_val, silent);
}

template<class T> void GV3::Register(gvar2<T>& gv, const std::string& name, const std::string& default_val, bool silent)
{
	gv.data = get_by_str<T>(name, default_val, silent);
}

inline void GV3::Register(gvar2<std::string>& gv, const std::string& name, const std::string& default_val, bool silent)
{
	gv.data = get_by_val(name, default_val, silent);
}





