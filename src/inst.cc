#include <gvars3/instances.h>

namespace GVars3
{
	GVars2 GV2;
	class GUI GUI;
	class GUI_Fltk GUI_Fltk(&GUI, &GV2);
	class GUI_Motif GUI_Motif(&GUI, &GV2);
}
