#include <gvars3/instances.h>

namespace GVars3
{
	GVars2 GV2;
	class GUI GUI;
	#ifdef GV3_HAVE_FLTK
		class GUI_Fltk GUI_Fltk(&GUI, &GV2);
	#endif
	#ifdef GV3_HAVE_MOTIF
		class GUI_Motif GUI_Motif(&GUI, &GV2);
	#endif
}
