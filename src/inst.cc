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

#ifndef GVARS2_COMPAT
#include <gvars3/instances.h>

#ifdef GUI_HAVE_FLTK
	#include <gvars3/GUI_Fltk.h>
#endif

#ifdef GUI_HAVE_FLTK2
	#include <gvars3/GUI_Fltk2.h>
#endif

#ifdef GUI_HAVE_MOTIF
	#include <gvars3/GUI_Motif.h>
#endif


#ifdef GUI_HAVE_READLINE
	#include <readline/readline.h>
	#include <readline/history.h>
#endif

#include "gvars3/GUI.h"
#include "gvars3/GStringUtil.h"

#include <iostream>
using namespace std;

namespace GVars3
{
	GVars2 GV2;
	class GUI GUI;

	#ifdef GUI_HAVE_FLTK
		class GUI_Fltk GUI_Fltk(&GUI, &GV2);
	#endif
	#ifdef GUI_HAVE_FLTK2
        class GUI_Fltk2 GUI_Fltk2(&GUI, &GV2);
    #endif
	#ifdef GUI_HAVE_MOTIF
		class GUI_Motif GUI_Motif(&GUI, &GV2);
	#endif

	#ifdef GUI_HAVE_READLINE
		void GUI::SetupReadlineCompletion()
		{
		  mpReadlineCompleterGUI = this;
		  rl_attempted_completion_function = ReadlineCompletionFunction;
		  rl_basic_word_break_characters = " \t\n\"\\'`@$><;|&{("; 
		}

		char ** GUI::ReadlineCompletionFunction (const char *text, int start, int end)
		{
		  rl_completion_append_character=0;
		  char **matches;
		  matches = (char **)NULL;
		  matches = rl_completion_matches (text,   ReadlineCommandGeneratorCB);
		  return (matches);
		}

		void print_history(ostream &ost)
		{
		  HIST_ENTRY **apHistEntries = history_list();
		  if(apHistEntries)
			while((*apHistEntries)!=NULL)
			  {
			ost << (*apHistEntries)->line << endl;
			apHistEntries++;
			  }
		};
	#else

		void GUI::SetupReadlineCompletion()
		{
		}

		char ** GUI::ReadlineCompletionFunction (const char *text, int start, int end)
		{
		  return NULL;
		}


		void print_history(ostream &ost)
		{
		}


	#endif


}
#endif
