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
};
#endif
