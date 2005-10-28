#include <gvars3/GUI.h>
#include <gvars3/GUI_Fltk.h>
#include <gvars3/GUI_Fltk2.h>
#include <gvars3/GUI_Motif.h>
#include <gvars3/config.h>

namespace GVars3
{
	extern GVars2 GV2;
	extern class GUI GUI;

	#ifdef GV3_HAVE_FLTK
		extern class GUI_Fltk GUI_Fltk;
	#endif

    #ifdef GV3_HAVE_FLTK2
        extern class GUI_Fltk2 GUI_Fltk2;
    #endif

	#ifdef GV3_HAVE_MOTIF
	extern GUI_Motif GUI_Motif;
	#endif
};
