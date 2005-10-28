AC_INIT(gvars3, 0.5)
AC_LANG(C++)

AC_CHECK_HEADERS(iostream)

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
	AC_MSG_WARN(TooN is missing. Numerics support disabled.)
	if test "$old_toon" != ""
	then
	AC_MSG_WARN(You have old versions of TooN installed in $old_toon. These are too old for libCVD.)
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

optionlist="fltk motif"
options=""

AC_CHECK_HEADERS(FL/Fl.h, [options="${options} fltk"; fltk=yes], [])
AC_SUBST(have_fltk,$fltk)

AC_CHECK_HEADERS(Xm/Xm.h, [options="${options} motif"; motif=yes], [])
AC_SUBST(have_motif,$motif)

AC_CHECK_HEADERS(fltk/Button.h, [options="${options} fltk2"; fltk2=yes], [])
AC_SUBST(have_fltk2,$fltk2)

major=`echo $PACKAGE_VERSION | awk -F'\\\\.' '{print $1}'`
minor=`echo $PACKAGE_VERSION | awk -F'\\\\.' '{print $2}'`

AC_SUBST(major, $major)
AC_SUBST(minor, $minor)


AC_OUTPUT(Makefile)
