/*                       
	This file is part of the GVars3 Library.

	Copyright (C) 2005 The Authors

	This library is free software, see LICENSE file for details
*/

#ifndef __GUI_FLTK2_H
#define __GUI_FLTK2_H

#include <gvars3/GUI.h>
#include <pthread.h>

#include <map>
#include <set>
#include <string>

namespace GVars3
{
class GUI_Fltk2_win;

class GUI_Fltk2
{
	public:
		GUI_Fltk2(class GUI * pGUI, class GVars2 * pGV2);

		static const int widget_height = 25;
		static const int widget_padding_x = 5;
		static const int widget_padding_y = 2;

		void   poll_windows();
		void   process_in_crnt_thread();
		void   start_thread();

	private:
		void InitXInterface(std::string sDisplay);
		void AddMonitor(std::string cmd, std::string args);
		void AddWindow(std::string sParams);
		void DestroyWindow(std::string sCommand);
		void AddPushButton(std::string cmd, std::string args);
		void AddToggleButton(std::string cmd, std::string args);
		void AddSlider(std::string cmd, std::string args);
		void AddSpin(std::string cmd, std::string args);
		void AddSmallToggle(std::string cmd, std::string args);
		void AddLabel(std::string cmd, std::string args);

		std::string remove_suffix(std::string cmd, std::string suffix);
		bool check_window(std::string win_name, std::string fname);

		class GUI *gui;
		class GVars2 *gv2;
		bool	init;
		std::string	name;
		pthread_t gui_thread;

		typedef struct
		{
			GUI_Fltk2_win*	win;
			bool			showme;
		} window;


		std::map<std::string, window> windows;

		static void InitXInterfaceCB(void*, std::string, std::string);
		static void AddWindowCB(void*, std::string, std::string);
		static void DestroyWindowCB(void*, std::string, std::string);
		static void AddPushButtonCB(void*, std::string, std::string);
		static void AddToggleButtonCB(void*, std::string, std::string);
		static void AddSliderCB(void*, std::string, std::string);
		static void AddMonitorCB(void*, std::string, std::string);
		static void AddSpinCB(void*, std::string, std::string);
		static void AddSmallToggleCB(void*, std::string, std::string);
		static void AddLabelCB(void*, std::string, std::string);
		static void* do_stuff_CB(void*);
};

extern class GUI_Fltk2 GUI_Fltk2;

}

#endif
