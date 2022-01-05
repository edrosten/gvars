/*                       
	This file is part of the GVars3 Library.

	Copyright (C) 2005 The Authors

	This library is free software, see LICENSE file for details
*/

#include "gvars3/instances.h"
#include "gvars3/GUI.h"
#include "gvars3/GUI_non_readline.h"
#include <iostream>
#include <pthread.h>

using namespace std;

namespace GVars3
{

  bool spawn_non_readline_thread::running=0;
  bool spawn_non_readline_thread::quit=0;
  std::string spawn_non_readline_thread::quit_callback;
  
  spawn_non_readline_thread::spawn_non_readline_thread(const std::string& cb)
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
	std::cerr << "Error: non_readline thread already running.\n";
      }
  }


  void* spawn_non_readline_thread::proc(void *)
  {
    char line[1000];
    line[999]=0;
    GUI.SetupReadlineCompletion();

    while(1)
      {
	cout << "> ";  
	cin.getline (line, 999);
	
	if(quit)
	  break;
		  
	if(line != string(""))
	  GUI.ParseLine(line);
      }
    
    GUI.ParseLine(quit_callback);
    return 0;
  }

  spawn_non_readline_thread::~spawn_non_readline_thread()
  {
    //Terminate the readline and wait for it to finish
    if(!none)
      {
	cout << endl << endl << endl << "Press enter to quit..." << endl ;
	quit = 1;
	pthread_join(cmd,NULL);
	quit_callback = "";
	quit = 0;
	running = 0;
      }
  }
	
}
