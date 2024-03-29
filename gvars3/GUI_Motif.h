/*                       
	This file is part of the GVars3 Library.

	Copyright (C) 2005 The Authors

	This library is free software, see LICENSE file for details
*/

#ifndef GVARS3_INCLUDE_GUI_MOTIF_H
#define GVARS3_INCLUDE_GUI_MOTIF_H

#include <gvars3/GUI.h>
#include <list>
#include <Xm/Xm.h>

namespace GVars3
{



class GUI_Motif
{
 public:
  GUI_Motif(class GUI *pGUI, class GVars2 *pGV2);
  void InitXInterface(std::string sDisplay);
  void AddPushButton(std::string sCommand, std::string sParams);
  void AddToggleButton(std::string sCommand, std::string sParams);
  void AddMonitor(std::string sCommand, std::string sParams);
  void AddSlider(std::string sCommand, std::string sParams);
  void AddWindow(std::string sParams);
  void AddLabel(std::string sCommand, std::string sParams);
  void AddSpin(std::string sCommand, std::string sParams);
  void DestroyWindow(std::string sCommand);
  void poll();
  void start_thread();
  
  
  
  
  
 private:
  std::string msName;

  void DoMotifEvents();
  static void InitXInterfaceCB(void*, std::string, std::string);
  static void AddPushButtonCB(void*, std::string, std::string);
  static void AddSliderCB(void*, std::string, std::string);
  static void AddWindowCB(void*, std::string, std::string);
  static void AddToggleButtonCB(void*, std::string, std::string);
  static void AddMonitorCB(void*, std::string, std::string);
  static void AddLabelCB(void*, std::string, std::string);
  static void AddSpinCB(void*, std::string, std::string);
  static void DestroyCB(void*, std::string, std::string);
  static void* GUI_Motif_Thread_CB(void* ptr);
  static void ButtonHandlerCB(Widget, XtPointer, XtPointer);
  void ButtonHandler(Widget, XtPointer);
  static void TextBoxCB(Widget, XtPointer, XtPointer);
  void TextBox(Widget, XtPointer);
  void GUI_Motif_Thread();

  static void RemoveWindowCB(Widget, void*, void*);
  void RemoveWindow(Widget w);
  
  class GUI *mpGUI;
  class GVars2 *mpGV2;
  
  pthread_mutex_t* mpMutex;
  
 
  Display* mpDisplay;
  XtAppContext mxtac;
  
  typedef struct {std::string sLabel; std::string sVarName; std::string sCache; int nDelaySetting; int nCurrentDelay; } monitorMapStruct;
  typedef struct {std::string sName; gvar2_int gvn;   int nCache; }   toggleMapStruct;
  typedef struct {std::string sVarName; double dMin; double dMax; std::string sCachedValue; }   sliderMapStruct;

  typedef struct {
    Widget wTopLevel;
    Widget wRowCol;
    int nWidth;
    int nHeight;
	bool die;
    std::map<Widget, std::string> PushButtonMap;
    std::map<Widget, monitorMapStruct> MonitorMap;
    std::map<Widget, toggleMapStruct > ToggleButtonMap;
    std::map<Widget, sliderMapStruct > SliderMap;
    std::map<Widget, std::pair<std::string, std::string> > SpinMap;
  } GUIWindowStruct;
  
  std::map<std::string, GUIWindowStruct> mmWindows;
  std::list<std::string> labels;
  
};


}


#endif
