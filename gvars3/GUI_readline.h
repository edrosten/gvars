/*                       
	This file is part of the GVars3 Library.

	Copyright (C) 2005 The Authors

	This library is free software, see LICENSE file for details
*/

#ifndef __GUI_READLINE_H
#define __GUI_READLINE_H

#include <string>

namespace GVars3
{

	class spawn_readline_thread
	{
		public:
			spawn_readline_thread(const std::string&);
			~spawn_readline_thread();

		private:
			static bool running;
			static bool quit;
			static std::string quit_callback;
			pthread_t cmd;
			bool 	  none;
			static  void* proc(void*);
	};


	class readline_in_current_thread
	{
		public:
			void poll();
			readline_in_current_thread(const std::string&);
			~readline_in_current_thread();
		private:
			static void lineread(char*);	
			static std::string quit_callback;


	};
};
#endif
