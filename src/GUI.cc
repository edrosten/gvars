#include "gvars3/GUI.h"
#include "gvars3/GStringUtil.h"



#include <cctype>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <algorithm>


using namespace std;
namespace GVars3
{
	//GUI GUI;

bool setvar(string s)
{
	//Execution failed. Maybe its an assignment.
	unsigned int n;
	n=s.find("=");

	if(n != string::npos)
	{    
		
		string var = s.substr(0, n);
		string val = s.substr(n+1);

		//Strip whitespace from around var;
		int s=0, e = var.length(); 
		for(; isspace(var[s]) && s < var.length(); s++);
		for(; isspace(var[e]) && e >=0; e--);
		
		if(e > s)
		{
			var = var.substr(s, e-s);
			GV3::set_var(var, val);
			return true;
		}
	}

	return false;
}

char* GV3ReadlineCommandGenerator(const char *szText, int nState)
{
	static vector<string> tags;
	static vector<string>::iterator tag_i;
	
	if(!nState)
	{
		tags = GV3::tag_list();
		tag_i = tags.begin();
		
	};

	while(tag_i != tags.end())
	{
		int text_len = strlen(szText);
		string current = *tag_i;

		tag_i++;

		if(strncmp(szText, current.c_str(), text_len) == 0)
		{
			//If it's a prefect match, complete with the value
			if(text_len == current.size())
				current = szText + ("=" + GV3::get_var(szText));
				
			char* r;
			
			if((r= (char*)malloc(current.size() + 1)) == NULL)
				return 0;

			strcpy(r, current.c_str());
			return r;
		}
	}

	return 0;
}




GUI *GUI::mpReadlineCompleterGUI=NULL;


void GUI::UnRegisterCommand(string sCommandName)
{
	mmCallBackMap.erase(sCommandName);
};


void GUI::RegisterCommand(string sCommandName, GUICallbackProc callback, void* thisptr)
{
	if(builtins.count(sCommandName))
	{
		cerr << "!!GUI::RegisterCommand: Tried to register reserved keyword " << sCommandName << "." << endl;
		return;
	}

	CallbackInfoStruct s;
	s.cbp = callback;
	s.thisptr = thisptr;

	bool bAlreadyThere = false;
	CallbackVector *cbv = &mmCallBackMap[sCommandName];


	for(CallbackVector::iterator i = cbv->begin(); i<cbv->end();i++)
		if( (i->cbp ==s.cbp) && (i->thisptr == s.thisptr))
			bAlreadyThere=true;

	if(!bAlreadyThere)
			cbv->push_back(s);
};


// Returns true if something was called;
// otherwise returns false
bool GUI::CallCallbacks(string sCommand, string sParams)
{
	if(mmCallBackMap.count(sCommand)==0)
		return false;

	//Make a copy of this callback vector, since the command might call Unregister.
	CallbackVector cbv = mmCallBackMap[sCommand];

	for(CallbackVector::iterator i = cbv.begin(); i<cbv.end();i++)
		i->cbp(i->thisptr, sCommand, sParams);

	return true;
};


void GUI::ParseStream(istream& is)
{
	string buffer;
	while(getline(is, buffer))
		ParseLine(buffer);
}

void GUI::LoadFile(string sFileName)
{
	ifstream ifs;

	vector<string> v = ChopAndUnquoteString(sFileName);

	if(v.size()<1) 
		return;
		
	ifs.open(v[0].c_str());

	if(!ifs.good())
	{
		cerr << "! GUI::Loadfile: Failed to load script file \"" << sFileName << "\"."<< endl;
		return;
	}

	ParseStream(ifs);
	ifs.close();
}





int FindCloseBrace(const string& s, int start, char op, char cl)
{
	int open=1;
	int i;

	for(i=start+1; i < s.size() ; i++)
	{
		if(s[i] == op) 
			open++;
		else if(s[i] == cl) 
			open--;

		if(open == 0)
			break;
	}
	
	if(i == s.size())
		i = s.npos;
	return i;
}


void GUI::ParseLine(string s, bool bSilentFailure )
{
	s = UncommentString(s);	
	if(s == "")
		return;





	  // New for 2004! brace expansion! any line with {gvarname} in it 
	  // will have the brace bit replaced with the contents of that gvar,
	  // one line per word in the gvar value...
	  // e.g. 
	  // > TrackerList = Tracker FireTracker
	  // > {TrackerList}.ReloadModel 
	  // would be equivalent to writing 
	  // > Tracker.ReloadModel 
	  // > FireTracker.ReloadModel
	  // Can use double braces so that brace expansion only occurs later;
	  // e.g. GUI_Motif.AddPushButton DoStuffToTrackerList {{TrackerList}}.do_stuff
	  // will create a single button which then exectues {TrackerList}.do_stuff
	  // (Without double braces, the it would try to make two buttons!)

  {   // Brace expansion wrapper follows:
    int nOpenBracePos = s.find("{");
    //int nCloseBracePos = s.rfind("}");
    int nCloseBracePos = FindCloseBrace(s, nOpenBracePos, '{', '}');
    if( (nOpenBracePos  !=s.npos) && 
	(nCloseBracePos !=s.npos) &&
	(nCloseBracePos > nOpenBracePos))
      {   // Brace Pair found!!

	string sBegin = s.substr(0,nOpenBracePos);
	string sVarName = s.substr(nOpenBracePos+1,nCloseBracePos-nOpenBracePos-1);
	string sEnd = s.substr(nCloseBracePos+1);
	
	int nLength = sVarName.size();   // Check if it's a double brace: {{foo}} in which case remove one pair, but don't expand.
	bool bIsDoubleQuoted = false;
	
	if(nLength>2)
	  if((sVarName[0]=='{') && (sVarName[nLength-1]=='}'))
	    bIsDoubleQuoted = true;
	
	if(!bIsDoubleQuoted)
	  {
	    //vector<string> vsExpandedList = ChopAndUnquoteString(mpGV2->StringValue(sVarName, true));
	    vector<string> vsExpandedList = ChopAndUnquoteString(GV3::get_var(sVarName));

		//ER: look at the end of the string for '$'s to also perform the substitution
		vector<string> chopped;
		string end;
		int dollarpos=-1, lastpos=0;
		while((dollarpos = sEnd.find("$", dollarpos+1)) != s.npos)
		{
			chopped.push_back(sEnd.substr(lastpos, dollarpos - lastpos));
			lastpos = dollarpos+1;
		}
		end = sEnd.substr(lastpos);

	    for(vector<string>::iterator i = vsExpandedList.begin(); i!=vsExpandedList.end();i++)
		{
			string line = sBegin + *i;
			for(vector<string>::iterator s=chopped.begin(); s != chopped.end(); s++)
				line += *s + *i;
			line += end;

	      ParseLine(line, bSilentFailure);
		}
	    return;
	  };
	
	// if it was double quoted, just parse it as normal, but replace s with the new, single-braced thingy.
	s = sBegin + sVarName + sEnd;
      }
  }
  
  // Newer for 2004: Round brace expansion 
  // Expands gvar in round braces to the value
  // e.g.  A = 2
  //       B = (A)
  // assigns 2 to B
  // And as before, use double brace to protect.

  {   // Round expansion wrapper follows:
    int nOpenBracePos = s.find("(");
    //int nCloseBracePos = s.rfind(")");
    int nCloseBracePos = FindCloseBrace(s, nOpenBracePos, '(', ')');
    if( (nOpenBracePos  !=s.npos) && 
	(nCloseBracePos !=s.npos) &&
	(nCloseBracePos > nOpenBracePos))
      {   // Brace Pair found!!
	//cout << "Found brace pair. " << endl;
	string sBegin = s.substr(0,nOpenBracePos);
	string sVarName = s.substr(nOpenBracePos+1,nCloseBracePos-nOpenBracePos-1);
	string sEnd = s.substr(nCloseBracePos+1);
	
	int nLength = sVarName.size();   // Check if it's a double brace: {{foo}} in which case remove one pair, but don't expand.
	bool bIsDoubleQuoted = false;
	
	if(nLength>2)
	  if((sVarName[0]=='(') && (sVarName[nLength-1]==')'))
	    {
	      s = sBegin + sVarName + sEnd;  // Just remove the first set of braces.
	      bIsDoubleQuoted = true;
	    };
	
	if(!bIsDoubleQuoted)
	  {
	    //string sExpanded = mpGV2->StringValue(sVarName, true);
	    string sExpanded = GV3::get_var(sVarName);
	    s = sBegin + sExpanded + sEnd;
	    //cout << "DEBUG : xx" << s << "xx" << endl;
	  };
      }
  }
  


	// Old ParseLine code follows, here no braces can be left (unless in arg.)
	istringstream ist(s);
		
	string sCommand;
	string sParams;
	
	//Get the first token (the command name)
	ist >> sCommand;
	if(sCommand == "")
		return;

	//Get everything else (the arguments)...
	
	//Remove any whitespace
	ist >> ws;
	getline(ist, sParams);
	
	//Attempt to execute command
	if (CallCallbacks(sCommand, sParams))
		return;
	
	if(setvar(s))
		return;

	if(!bSilentFailure) 
		cerr << "? GUI::ParseLine: Unknown command \"" << sCommand << "\" or invalid assignment." << endl;
}



////////////////////////////////////////////////////////////////////////////////
//
// Builtin commands

void builtin_shell(void* ptr, string sCommand, string sParams)
{
	vector<string> v = ChopAndUnquoteString(sParams);

	if(v.size()==0) 
	{
		cerr <<"? GUI internal shell command: No prog/args given."<< endl; 
		return;
	}

	system(sParams.c_str());
}

void builtin_ls(void* ptr, string sCommand, string sParams)
{
	GUI* p = (GUI*)ptr;
	p->ParseLine("shell ls " + sParams);	
}

void builtin_ll(void* ptr, string sCommand, string sParams)
{
	GUI* p = (GUI*)ptr;
	p->ParseLine("shell ls -l " + sParams);	
}

void builtin_try(void* ptr, string sCommand, string sParams)
{
	GUI* p = (GUI*)ptr;
	p->ParseLine(sParams, true);
}

void builtin_exec(void* ptr, string sCommand, string sParams)
{
	GUI* p = (GUI*)ptr;
	p->LoadFile(sParams);
}

void builtin_echo(void* ptr, string sCommand, string sParams)
{
	cout << sParams << endl;
}

void builtin_qmark(void* ptr, string sCommand, string sParams)
{
	cout << "  Perhaps you'd like to type \"commandlist\" or \"gvarlist\"." << endl;
}

void builtin_if(void* ptr, string sCommand, string sParams)
{
  GUI* p = (GUI*)ptr;

  vector<string> v = ChopAndUnquoteString(sParams);

  if(v.size()<2)
  {
	cerr <<"? GUI internal if command: not enough params (syntax: if gvar command)"<< endl; 
	return;
  }

  string sValue = GV3::get_var(v[0]);

  if(sValue == "(not in GVar list)")
	return;

  // Have to do this to fix whitespace issues on non-registered gvars
  vector<string> vv = ChopAndUnquoteString(sValue); 

  if(vv.size()<1) 
  	return;
  else if(vv[0]!="0")
  {
	string s;
	s = "";

	for(int i=1;i<v.size();i++)
	  s = s + " " +  v[i];

	p->ParseLine(s);
  }
  return;
}

void builtin_set(void* ptr, string sCommand, string sParams)
{
	setvar(sParams);
}


void builtin_gvarlist(void* ptr, string sCommand, string sParams)
{
	GV3::print_var_list(cout);
}


void builtin_printvar(void* ptr, string sCommand, string sParams)
{
	cout << sParams << "=" << GV3::get_var(sParams) << endl;;
}


void builtin_commandlist(void* ptr, string sCommand, string sParams)
{
	GUI* p = (GUI*)ptr;

	cout << "  Builtin commands:" << endl;

	for(set<string>::iterator i=p->builtins.begin(); i!=p->builtins.end(); i++)
		cout << "    " << *i << endl;


	cout << "  Registered commands:" << endl;

	for(map<string,CallbackVector>::iterator i=p->mmCallBackMap.begin(); i!=p->mmCallBackMap.end(); i++)
		if(p->builtins.count(i->first) == 0)
			  cout << "    " << i->first << endl;
}

///////////////////////////////////////
///////////////////////////////////////
////////     Readline stuff:
///////////////////////////////////////
///////////////////////////////////////



#ifdef HOSTTYPE_I386

#include <readline/readline.h>
#include <readline/history.h>


char * GUI::ReadlineCommandGeneratorCB(const char *szText, int nState)
{
  return mpReadlineCompleterGUI->ReadlineCommandGenerator(szText, nState);

}

char * GUI::ReadlineCommandGenerator(const char *szText, int nState)
{
  //static vector<string>::iterator iBuiltin;
  static map<string,CallbackVector>::iterator iRegistered;
  static int nTextLength;
  static int nOffset;
  const char *pcName;
  static char acMyTextCopy[1000];
  
  if(!nState)
  {
//	iBuiltin = mvBuiltinCommands.begin();
	iRegistered = mmCallBackMap.begin();
	nTextLength = strlen(szText);
	nOffset=-1;
  }
  
/*  while(iBuiltin!=mvBuiltinCommands.end())
  {
	pcName = iBuiltin->c_str();

	iBuiltin++;

	if(strncmp(pcName,szText,nTextLength) == 0)
	{
	  char *t = (char*) malloc(strlen(pcName)+2);
	  if(!t) 
	  	return NULL;
	  strcpy(t,pcName);
	  t[strlen(pcName)]=' ';
	  t[strlen(pcName)+1]=0;
	  return t;
	}
  }
*/
  while(iRegistered!=mmCallBackMap.end())
    {
      pcName = iRegistered->first.c_str();
      iRegistered++;
      if(strncmp(pcName,szText,nTextLength) == 0)
	{
	  char *t = (char*) malloc(strlen(pcName)+2);
	  if(!t) return NULL;
	  strcpy(t,pcName);
	  t[strlen(pcName)]=' ';
	  t[strlen(pcName)+1]=0;
	  return t;
	};
    };
  
  if(nOffset==-1)
    nOffset = nState;
	
  //return mpGV2->ReadlineCommandGenerator(szText, nState-nOffset);
  return GV3ReadlineCommandGenerator(szText, nState-nOffset);
  
  
};

char ** GUI::ReadlineCompletionFunction (const char *text, int start, int end)
{
  rl_completion_append_character=0;
  char **matches;
  matches = (char **)NULL;
  
  /* If this word is at the start of the line, then it is a command
           to complete.  Otherwise it is the name of a file in the current
           directory. */
  //  if (start == 0)
#ifdef BUILDTYPE_i386nith
  matches = completion_matches ( (char*) text, (CPFunction*) ReadlineCommandGeneratorCB);
#else
#ifdef BUILDTYPE_i386davinci
  matches = rl_completion_matches (text,   ReadlineCommandGeneratorCB);
#else
  matches = rl_completion_matches (text,   ReadlineCommandGeneratorCB);
#endif
#endif
  
  return (matches);
}

#endif

void GUI::SetupReadlineCompletion()
{
#ifdef HOSTTYPE_I386
  mpReadlineCompleterGUI = this;
#ifdef BUILDTYPE_i386nith
  rl_attempted_completion_function = (CPPFunction *) ReadlineCompletionFunction;
#else
#ifdef BUILDTYPE_i386davinci
  rl_attempted_completion_function = ReadlineCompletionFunction;
#else
  rl_attempted_completion_function = ReadlineCompletionFunction;
#endif
#endif

#endif
}




void GUI::RegisterBuiltin(string sCommandName, GUICallbackProc callback)
{
	RegisterCommand(sCommandName, callback, this);
	builtins.insert(sCommandName);
}

GUI::GUI()
{
	do_builtins();
}

GUI::GUI(GVars2*)
{
	do_builtins();
}

void GUI::do_builtins()
{
	RegisterBuiltin("shell", builtin_shell);
	RegisterBuiltin("ls", builtin_ls);
	RegisterBuiltin("ll", builtin_ll);
	RegisterBuiltin("try", builtin_try);
	RegisterBuiltin("exec", builtin_exec);
	RegisterBuiltin("echo", builtin_echo);
	RegisterBuiltin("if", builtin_if);
	RegisterBuiltin("set", builtin_set);
	RegisterBuiltin("?", builtin_qmark);
	RegisterBuiltin("gvarlist", builtin_gvarlist);
	RegisterBuiltin("printvar", builtin_printvar);
	RegisterBuiltin("commandlist", builtin_commandlist);

	//mpGV2=pGV2;
}


};
