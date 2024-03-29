/*                       
	This file is part of the GVars3 Library.

	Copyright (C) 2005 The Authors

	This library is free software, see LICENSE file for details
*/

#include "gvars3/gvars3.h"

using namespace std;


namespace GVars3
{
	void GVars2::SetVar(std::string sVar, std::string sValue, bool s)
	{
		GV3::set_var(sVar, sValue, s);
	}


	void GVars2::SetVar(std::string s)
	{
	  // Expected format: "foo = bar"
	  // So crack the string in two at the equals sign. Easy.
	  // SetVar(string,string) is quite tolerant to whitespace.
	  
	  string::size_type n;
	  n=s.find("=");
	  if(n>=s.find("//"))  // Perhaps the line is a comment?
		return;
	  if(n==s.npos)
		{
		  cout << "? Gvars::SetVar(string): No equals sign found in \""<<s<<"\""<< endl;
		  return;
		};
	  SetVar(s.substr(0,n),s.substr(n+1,s.npos-n));
	}

	int& GVars2::GetInt(const string& name, int default_val, int flags)
	{
		return Get<int>(name, default_val, flags);
	}

	double& GVars2::GetDouble(const string& name, double default_val, int flags)
	{
		return Get<double>(name, default_val, flags);
	}
	string& GVars2::GetString(const string& name, const string& default_val, int flags)
	{
		return Get<string>(name, default_val, flags);
	}


	int& GVars2::GetInt(const string& name, const string& default_val, int flags)
	{
		return Get<int>(name, default_val, flags);
	}

	double& GVars2::GetDouble(const string& name, const string& default_val, int flags)
	{
		return Get<double>(name, default_val, flags);
	}

	string GVars2::StringValue(const string &name, bool no_quotes)
	{
		if(no_quotes)
			return GV3::get_var(name);
		else
			return "\"" + GV3::get_var(name) + "\"";
	}

	void GVars2::PrintVarList(ostream& os)
	{
  	        GV3::print_var_list(os);
	}

	void GVars2::PrintVar(string s, ostream& os, bool bEndl)
	{
		os << s << "=" << StringValue(s, true);
		if(bEndl)
			os << endl;

	}
}

