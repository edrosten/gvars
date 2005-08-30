CC=gcc
CXX=g++ -fPIC

#OBJS=GStringUtil.o GVars2.o GUI.o GUI_Fltk.o
#GUI_Motif.o 
OBJS=\
	src/gvars3.o			\
	src/GUI.o				\
	src/GStringUtil.o		\
	src/GUI_Fltk.o			\
	src/GUI_Motif.o			\
	src/inst.o				\
	src/gvars2.o			\
	src/serialize.o



OFLAGS=-O2
OFLAGS=-g -ggdb


IFLAGS= -I /usr/X11R6/Lesstif-0.93.94/include/\
		-I$(HOME)/code/TooN\
		-I$(HOME)/code/TooN/TooN\
		-I.\
		-I$(HOME)/usr/include\
		-I/usr/X11R6/include \

CXXFLAGS=$(OFLAGS) $(IFLAGS) -DHOSTTYPE_I386 -pthread

INSTALL_LIBS=$(HOME)/usr/arch/Linux/lib
INSTALL_HDRS=$(HOME)/usr/include/

V=4


test: libGUI.so.$V

libGUI.so.$V: libGUI.a
	ld -shared -soname $@ -o $@ -lc $(OBJS)
	
install:libGUI.so.$V libGUI.a
	cp libGUI.so.$V $(INSTALL_LIBS)
	cp libGUI.a $(INSTALL_LIBS)
	cp -r gvars3 $(INSTALL_HDRS)
	
	rm -f $(INSTALL_LIBS)/libGUI.so
	ln -s $(INSTALL_LIBS)/libGUI.so.$V $(INSTALL_LIBS)/libGUI.so
	
	

libGUI.a:$(OBJS)
	ar cvrs libGUI.a  $(OBJS)



test:libGUI.a main.o
	$(CXX) -o test main.o libGUI.a -lreadline -lncurses -pthread\
			  -L/usr/X11R6/lib	-lX11 -lXm -lXft -lfltk
			  #-L$(HOME)/usr/arch/Linux/lib -lfltk
			  #/home/cabinet1/er258/usr/src/fltk-1.1.5rc1/lib/libfltk.a\
			  #
test2.o: gvars3.o

test2:test2.o 
	$(CXX) -o test2 test2.o gvars3.o

clean:
	rm -f *.a *.o *.so *.so.? test test2
