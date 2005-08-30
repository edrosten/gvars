#include "gvars3/gvars3.h"
#include <vector>
#include <algorithm>

using namespace std;
using namespace TooN;

namespace GVars3
{

	 std::map<std::string, std::string>		GV3::unmatched_tags;
	 std::map<std::string, BaseMap*>		GV3::typeof_tags;
	 std::list<BaseMap*>					GV3::maps;


	void GV3::add_typemap(BaseMap* m)
	{
		maps.push_back(m);
	}


	string GV3::get_var(string name)
	{
		
	
		if(typeof_tags.find(name) != typeof_tags.end())
			return typeof_tags[name]->get_as_string(name);
		else if(unmatched_tags.find(name) != unmatched_tags.end())
			return unmatched_tags[name];
		else
			return "(Not present in GVar list.)";
	}

	bool GV3::set_var(string name, string val)
	{
		if(typeof_tags.count(name))
			return typeof_tags[name]->set_from_string(name, val);
		else
		{
			unmatched_tags[name]=val;
			return true;
		}
	}

	void GV3::print_var_list(ostream& o)
	{
		o << "//Registered GVars:" << endl;
		
		for(map<string, BaseMap*>::iterator i=typeof_tags.begin(); i != typeof_tags.end(); i++)
			cout << i->first << "  =  " << get_var(i->first) << endl;

		o << "//Unmatched tags:" << endl;

		for(map<string,string>::iterator i=unmatched_tags.begin(); i != unmatched_tags.end(); i++)
			o << i->first << "  =  " << i->second << endl;
		o << "// End of GVar list." << endl;

	}

	vector<string> GV3::tag_list()
	{
		vector<string> v;
		for(map<string, BaseMap*>::iterator i=typeof_tags.begin(); i != typeof_tags.end(); i++)
			v.push_back(i->first);

		return v;
	}
};
