CC=gcc
CXX=g++

#OBJS=GStringUtil.o GVars2.o GUI.o GUI_Fltk.o
#GUI_Motif.o 
OBJS=\
	src/gvars3.o			\
	src/GUI.o				\
	src/GStringUtil.o		\
	src/GUI_Fltk.o			\
	src/gvars2.o			\
	src/serialize.o			\
	src/GUI_Motif.o			\
	src/inst.o				\



OFLAGS=-O2
OFLAGS=-g -ggdb


IFLAGS= \
		-I.\
		-I$(HOME)/usr/include\
		-I/usr/X11R6/include \
		#-I /usr/X11R6/Lesstif-0.93.94/include/\

CXXFLAGS=$(OFLAGS) $(IFLAGS) -DHOSTTYPE_I386 

INSTALL_LIBS=$(HOME)/usr/arch/Linux-suse/lib
INSTALL_HDRS=$(HOME)/usr/include/

V=4


test: libGUI.so.$V

libGUI.so.$V: libGUI.a
	$(CXX) -shared -o $@ -lc $(OBJS)	  -L/usr/X11R6/lib	-lX11 -lXm -lXft -lfltk -lpthread -lreadline

	
install:libGUI.so.$V libGUI.a
	cp libGUI.so.$V $(INSTALL_LIBS)
	cp libGUI.a $(INSTALL_LIBS)
	cp -r gvars3 $(INSTALL_HDRS)
	
	rm -f $(INSTALL_LIBS)/libGUI.so
	ln -s $(INSTALL_LIBS)/libGUI.so.$V $(INSTALL_LIBS)/libGUI.so
	
	

libGUI.a:$(OBJS)
	ar cvrs libGUI.a  $(OBJS)



test:libGUI.a main.o
	$(CXX) -o test main.o libGUI.a -lreadline -lncurses \
			  -L/usr/X11R6/lib	-lX11 -lXm -lXft -lfltk -lpthread
			  #-L$(HOME)/usr/arch/Linux/lib -lfltk
			  #/home/cabinet1/er258/usr/src/fltk-1.1.5rc1/lib/libfltk.a\
			  #
test2.o: gvars3.o

test2:test2.o 
	$(CXX) -o test2 test2.o gvars3.o

clean:
	rm -f *.a src/*.o *.so *.so.? test *.o
