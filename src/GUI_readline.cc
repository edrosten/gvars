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

#include "gvars3/instances.h"
#include "gvars3/GUI_readline.h"

#include <pthread.h>
#include <readline/readline.h>
#include <readline/history.h>

using namespace std;
#ifdef GVARS2_COMPAT
#define GUI ::GUI
#endif

namespace GVars3
{

	bool spawn_readline_thread::running=0;
	bool spawn_readline_thread::quit=0;
	std::string spawn_readline_thread::quit_callback;

	spawn_readline_thread::spawn_readline_thread(const std::string& cb)
	{
		if(!running)
		{
			pthread_create(&cmd, 0, proc, 0);
			running = 1;
			quit=0;
			none=0;
			quit_callback = cb;
		}
		else
		{
			none =1;
			std::cerr << "Error: readline thread already running.\n";
		}
	}


	int rlhook()
	{
		return 1;
	}

	void* spawn_readline_thread::proc(void *)
	{
		char* line;
		GUI.SetupReadlineCompletion();

		rl_event_hook = rlhook;
		
		while(1)
		{
			line=readline("> ");

			if(quit || !line)
				break;

			GUI.ParseLine(line);
			add_history(line);
		}
		
		GUI.ParseLine(quit_callback);

		return 0;
	}

	spawn_readline_thread::~spawn_readline_thread()
	{
		//Terminate the readline and wait for it to finish
		if(!none)
		{
			quit = 1;
			rl_done = 1;
			rl_stuff_char('\n');
			pthread_join(cmd, 0);
			quit_callback = "";
			quit = 0;
			running = 0;
		}
	}

}

