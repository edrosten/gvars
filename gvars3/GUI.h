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
