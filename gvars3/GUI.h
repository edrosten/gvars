#ifndef __GUI_H
#define __GUI_H

#include <gvars3/gvars3.h>
#include <vector>
#include <iostream>
#include <set>

namespace GVars3
{
	typedef void (*GUICallbackProc)(void* ptr, std::string sCommand, std::string sParams);

	typedef struct 
	{
	  GUICallbackProc cbp;
	  void* thisptr;
	} CallbackInfoStruct;

	typedef std::vector<CallbackInfoStruct> CallbackVector;

	class GUI
	{
	 public:
	  GUI();
	  GUI(GVars2*);//Dummy gvars2
	  
	  void RegisterCommand(std::string sCommandName, GUICallbackProc callback, void* thisptr=NULL);
	  void UnRegisterCommand(std::string sCommandName);
	  void ParseLine(std::string s, bool bSilentFailure = false);
	  void ParseStream(std::istream& is);
	  void LoadFile(std::string sFileName);
	  
	  bool CallCallbacks(std::string sCommand, std::string sParams);
	  void SetupReadlineCompletion();

	 private:
	  
	  void do_builtins();
	  void RegisterBuiltin(std::string, GUICallbackProc);
	  
	  static GUI *mpReadlineCompleterGUI;

	  static char** ReadlineCompletionFunction(const char *text, int start, int end);
	  static char * ReadlineCommandGeneratorCB(const char *text, int state);
	  char * ReadlineCommandGenerator(const char *text, int state);
	  
	  void *mptr;
	  GUICallbackProc cbp;

	  std::map<std::string, CallbackVector > mmCallBackMap;
	  std::set<std::string> builtins;

	  friend void builtin_commandlist(void* ptr, std::string sCommand, std::string sParams);
	};
}




#endif
