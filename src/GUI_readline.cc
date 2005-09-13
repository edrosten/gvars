#include "gvars3/instances.h"
#include "gvars3/GUI_readline.h"

#include <pthread.h>
#include <readline/readline.h>
#include <readline/history.h>

using namespace std;

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

