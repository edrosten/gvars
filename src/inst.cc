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
}
