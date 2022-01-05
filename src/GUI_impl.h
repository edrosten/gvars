/*                       
	This file is part of the GVars3 Library.

	Copyright (C) 2005 The Authors

	This library is free software, see LICENSE file for details
*/

#ifndef GVARS3_INC_GUI_IMPL_H
#define GVARS3_INC_GUI_IMPL_H

#include <gvars3/gvars3.h>
#include <gvars3/GUI.h>
#include <vector>
#include <iostream>
#include <set>
#include <list>
#include <functional>

namespace GVars3
{
	typedef struct 
	{
	  GUICallbackProc cbp;
	  void* thisptr;
	} CallbackInfoStruct;

	typedef std::vector<CallbackInfoStruct> CallbackVector;




	class GUI_language;

	class GUI_impl
	{
		public:
			GUI_impl();
			void post_init();
			GUI_impl(GVars2*);//Dummy gvars2

			void RegisterCommand(std::string sCommandName, GUICallbackProc callback, void* thisptr=NULL);
			void RegisterCommand(std::string sCommandName, std::function<void(std::string,std::string)>);
			void UnRegisterAllCommands(void* thisptr);
			void UnRegisterCommand(std::string sCommandName, void* thisptr);
			void UnRegisterCommand(std::string sCommandName);
			void ParseLine(std::string s, bool bSilentFailure = false);
			void ParseStream(std::istream& is);
			void LoadFile(std::string sFileName);

			bool CallCallbacks(std::string sCommand, std::string sParams);
			void SetupReadlineCompletion();

			/// Start a thread which parses user input from the console.
			/// Uses libreadline if configured, or just plain old iostream readline
			void StartParserThread();
			/// Stop the console parser thread, if running
			/// Top tip: This is static so that it can be used with atexit(void*)
			static void StopParserThread();

			/// parse command line arguments for GVar values. It expects the form --name value and will stop
			/// parsing when this form is not true anymore. possible cases are a single --, an argument, etc..
			/// if it finds an argument --exec it interprets the next argument as a file name to load via LoadFile
			/// @arg argc total number of arguments from main argc
			/// @arg argv pointer to array of strings from main argv
			/// @arg start first argument to look at to be compatible with other parsers
			/// @arg prefix the prefix to use. note that everything after the prefix will be interpreted as part of the variable name
			/// @arg execKeyword keyword to use to trigger execution of the file given as parameter with LoadFile
			/// @return
			int parseArguments( const int argc, char * argv[], int start = 1, const std::string prefix = "--", const std::string execKeyword = "exec" );

		private:
			void do_builtins();
			void RegisterBuiltin(std::string, GUICallbackProc);

			static GUI_impl *mpReadlineCompleterGUI;

			static char** ReadlineCompletionFunction(const char *text, int start, int end);
			static char * ReadlineCommandGeneratorCB(const char *text, int state);
			char * ReadlineCommandGenerator(const char *text, int state);

			void *mptr;
			GUICallbackProc cbp;

			static void *mpParserThread;
			GUI_language* lang;

			std::map<std::string, CallbackVector > mmCallBackMap;
			std::set<std::string> builtins;
			std::map<std::string, std::vector<std::string> > mmQueues;
			std::list<std::function<void(std::string,std::string)>> callback_functions;

			friend void builtin_commandlist(void* ptr, std::string sCommand, std::string sParams);
			friend void builtin_queue(void* ptr, std::string sCommand, std::string sParams);
			friend void builtin_runqueue(void* ptr, std::string sCommand, std::string sParams);
	};

	template<int D=0> struct GUI_impl_singleton
	{
		static GUI_impl& instance()
		{
			static GUI_impl* inst = 0;
			if(!inst)
			{
				inst = new GUI_impl;
				inst->post_init();
			}
			return *inst;
		}
	};



}

#endif
