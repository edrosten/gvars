AC_INIT(gvars3, 0.5)
AC_PROG_AWK
AC_LANG(C++)

AC_CHECK_HEADERS(iostream)

#Host detection
AC_CANONICAL_HOST
if test "$host_vendor" = apple
then
	CPPFLAGS="$CPPFLAGS -I /sw/include -I/opt/local/include -I/usr/X11R6/include"
	LIBS="$LIBS -L/sw/lib -L/opt/local/lib"
	AC_MSG_NOTICE([Adding /sw to the build path.])
	AC_MSG_NOTICE([Adding /opt/local to the build path.])
	AC_SUBST(osx_hacks,yes)
fi

echo
echo --------------------------
echo Checking for prerequisites
echo --------------------------

###################################################
#
#Allow the user to specify the location of TooN
#
AC_ARG_WITH(TooN, [  --with-TooN=directory  Specify location for TooN ], [opt_toon_dir=$withval])

if test "$opt_toon_dir" = no
then
	AC_MSG_ERROR("TooN is not optional. Use --with-TooN=dir to specify where it can be found.")
fi

#Yes is not a useful answer. We know the answer is yes.
if test "$opt_toon_dir" = yes
then
	opt_toon_dir=
fi

#Check for TooN
AC_MSG_CHECKING([for TooN])

#Check for TooN in a bunch of places
save_CPPFLAGS="$CPPFLAGS"
toondir="Missing"
for dir in  $opt_toon_dir "" "$HOME/usr/include" "$HOME/code/TooN" "$HOME/Src/TooN" "../TooN" ".."
do
	a=
	if test "$dir" = ""
	then
		CPPFLAGS="$save_CPPFLAGS"
	else
		CPPFLAGS="$save_CPPFLAGS -I$dir"
	fi

	AC_COMPILE_IFELSE([#include <TooN/TooN.h>
int main(){TooN::Vector<2> v;return 0;}], [a=1]) 

	AC_COMPILE_IFELSE([#include <numerics.h>
int main(){Vector<2> v;return 0;}], [old_toon="$old_toon $dir"]) 
	
	if test x$a = x1
	then
		toondir=$dir
		break
	fi
done

if test "$toondir" = ""
then
	AC_MSG_RESULT(yes)
else
	AC_MSG_RESULT($toondir)
fi

if test "$toondir" = Missing
then
	AC_MSG_WARN(TooN is missing.)
	if test "$old_toon" != ""
	then
	AC_MSG_WARN(You have old versions of TooN installed in $old_toon. These are too old for gvars.)
	fi
	toon=no
else
	options="$options toon"
	toon=yes
	AC_DEFINE(CVD_HAVE_TOON)
	AC_SUBST(have_toon,yes)
fi

if test $toon == no
then
	AC_MSG_ERROR(TooN is required. Visit savannah.nongnu.org/projects/toon)
fi

foo_LIBS="$LIBS"
for curse in "" ncurses curses
do
	LIBS="$foo_LIBS"
	if test x$curse != x
	then 
		AC_MSG_CHECKING($curse as a requirement for readline)
		AC_MSG_RESULT()
		AC_CHECK_LIB($curse, tputs)
	fi
		
	AC_CHECK_LIB(readline,rl_done,[],[])

	if test x$ac_cv_lib_readline_rl_done == xyes
	then
		break
	else
		unset ac_cv_lib_readline_rl_done
	fi
done


if test x$ac_cv_lib_readline_rl_done != xyes
then
	AC_MSG_ERROR(Readline is not optional. It should be. Sorry.)
fi

AC_CHECK_LIB(pthread, pthread_join, [], [])
if test x$ac_cv_lib_pthread_pthread_join != xyes
then
	AC_MSG_ERROR(pthread is not optional. It should be. Sorry.)
fi

LIBS="$LIBS -pthread"
CPPFLAGS="$CPPFLAGS -pthread"


echo ------------------------------------------------
echo "Extra GVars2 Compat:"
echo " (adds GVars2.h, GUI.h, GUI_Motif.h GStringUtil.h wrappers,"
echo " and removes the global GV2 and GUI instances)"
# Check if the user (i.e. Georg) wants extra GV2 backwards-compat
AC_ARG_WITH(gvars2_compat,   [  --with-gvars2-compat    add extra GVars2 backwards compatibility])
if test "$with_gvars2_compat" == "yes"
then
	AC_MSG_NOTICE([Extra GVars2 compat enabled. ])
        AC_SUBST(use_gvars2_compat,yes)
else
	AC_MSG_NOTICE([Extra GVars2 compat not enabled. ])
        AC_SUBST(use_gvars2_compat,no)
fi




################################################################################
#
# Basic dialogue box functionality
#

echo
echo ------------------------------------------------
echo "Checking for widget libraries (provides GUI_...)"
echo ------------------------------------------------

optionlist="fltk motif fltk2"
options=""

#dnl check_GUI(fltk,FL/Fl.h,fltk)

define(check_GUI,[
AC_CHECK_HEADERS($2,a_$1=yes)
AC_CHECK_LIB($3,main)
if test x$ac_cv_lib_[]$3[]_main == xyes && test x$[]a_$1 == xyes
then
	AC_SUBST(have_[]$1, yes)
	options="$options $1"
	$1=yes
fi
])

check_GUI(fltk,FL/Fl.h,fltk)

AC_PATH_X
foo_LIBS="$LIBS"
foo_CPPFLAGS="$CPPFLAGS"
test -n "$x_libraries" && LIBS="-L$x_libraries $LIBS"
test -n "$x_includes" && CPPFLAGS="$CPPFLAGS -I$x_includes"
check_GUI(motif,Xm/Xm.h,Xm)
if test x$motif != xyes
then
	LIBS="$foo_LIBS"
	CPPFLAGS="$foo_CPPFLAGS"
fi
	

# check_GUI(fltk2,fltk/Button.h,fltk2)
a=
AC_PATH_PROG(a,fltk2-config)
if test x$a != x
then
    FLTK2_CPPFLAGS=`$a --cxxflags`
    FLTK2_LIBS=`$a --ldflags`
    CPPFLAGS="$FLTK2_CPPFLAGS $CPPFLAGS"
    LIBS="$FLTK2_LIBS $LIBS"
    AC_SUBST(have_fltk2, yes)
    options="$options fltk2"
    fltk2=yes
fi

if test x$fltk$motif$fltk2 == x
then
	AC_MSG_WARN(No GUI functionality enabled)
fi

################################################################################
#
# The rest
#

major=`echo $PACKAGE_VERSION | awk -F'\\\\.' '{print $1}'`
minor=`echo $PACKAGE_VERSION | awk -F'\\\\.' '{print $2}'`

AC_SUBST(major, $major)
AC_SUBST(minor, $minor)

changequote(,)
echo Options:
echo $options
echo
echo Missing options:
echo $options $optionlist |  $AWK 'BEGIN{RS=" "}{a[$1]++}END{for(i in a)if(a[i]==1)printf(i" "); print""}'
echo
changequote([,])


AC_OUTPUT(Makefile)
