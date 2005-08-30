#include <gvars3/GUI.h>
#include <gvars3/GUI_Motif.h>
#include <gvars3/GUI_Fltk.h>
#include <gvars3/instances.h>
#include <readline/readline.h>
#include <readline/history.h>

using namespace GVars3;
using namespace std;

class P
{
public:
  static void mfunc(void* ptr, string sCommand, string sParams)
  {
    cout << "MFunc: " << sCommand << sParams << " n:"<<endl;
  };
  
  int n;

  void Register()
  {
    GUI.RegisterCommand("asdfs",mfunc, this);
    
  };

};


void func(void* ptr, string sCommand, string sParams)
{
  cout << "Func here! " << sCommand <<  endl;
  
}

int main(void)
{
  GUI_Fltk.start_thread();

  GUI.LoadFile("autoexec.cfg");
  GUI.RegisterCommand("Something", func, NULL);
  GUI.SetupReadlineCompletion();
  
  GV2.GetInt("TestInt", 10);
  GV2.GetDouble("TestDouble", 10);
  GV2.GetString("TestString", "hello world");
  GV2.GetString("TestString2", "hello world");


  GVars3::gvar2_double a_double;
  GV2.Register(a_double, "test_double", 0., true);
  
  char* pcLine;
  do
    {
      pcLine=readline("ModelEdit >");
      if(pcLine) {
	GUI.ParseLine(pcLine);
	add_history(pcLine);
      };
    }
  while(pcLine);
  
  cout << "end." << endl;
  
  
  return 0;
  

  

};
