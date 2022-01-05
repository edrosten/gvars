/*                       
	This file is part of the GVars3 Library.

	Copyright (C) 2005 The Authors

	This library is free software, see LICENSE file for details
*/

#ifndef __GUI_NON_READLINE_H
#define __GUI_NON_READLINE_H

#include <string>

#include <pthread.h>

namespace GVars3
{

	class spawn_non_readline_thread
	{
		public:
			spawn_non_readline_thread(const std::string&);
			~spawn_non_readline_thread();

		private:
			static bool running;
			static bool quit;
			static std::string quit_callback;
			pthread_t cmd;
			bool 	  none;
			static  void* proc(void*);
	};


};
#endif
