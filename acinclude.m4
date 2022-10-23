
AC_DEFUN([OC_CHECK_RUN_ON_WIN],
	[
oc_run_win=no
case $host_os in
cygwin*)
AC_COMPILE_IFELSE(
	[AC_LANG_SOURCE([
#include <Windows.h>
#ifndef _WIN32
#error "We don't have _WIN32"
#endif
	])],
	[oc_run_win=yes],
	[oc_run_win=no]) ;;
cygwin* | mingw* | pw32* | cegcc*) oc_run_win=yes ;;
esac

AM_CONDITIONAL(RUN_WIN, test "x$oc_run_win" = xyes)
])


AC_DEFUN([OC_GEN_WIN_SYSROOT], [
AC_REQUIRE([OC_CHECK_RUN_ON_WIN])
oc_win_sysroot=
if test "x$oc_run_win" = xyes; then
   case $host_os in
   cygwin*)
       oc_win_sysroot=`cygpath -S`
       ;;
   mingw* | pw32* | cegcc*)
       oc_win_sysroot=`AS_ECHO([$SYSTEMROOT]) | sed 's/(^[a-zA-Z]):/\/\1\//' | sed 's/\\/\/'`
       oc_win_sysroot=$oc_win_sysroot/System32
       ;;
   esac
fi
AC_SUBST([oc_win_sysroot])
])


AC_DEFUN([OC_USE_WIN_UCRT], [
AC_REQUIRE([OC_CHECK_RUN_ON_WIN])

if test -z "$1" || test "x$1" != xno ; then
	if test "x$oc_run_win" = xyes; then
		AH_TEMPLATE([__MSVCRT_VERSION__], [You will use ucrtbase.dll on 
			windows system, if you define __MSVCRT_VERSION__ as
			higher value than 0x1400])
		AC_DEFINE([__MSVCRT_VERSION__], [0x1400])
		AM_CONDITIONAL(USE_WIN_UCRT, [test -z ""])
	else
		AM_CONDITIONAL(USE_WIN_UCRT, [test -n ""])
	fi
else
	AM_CONDITIONAL(USE_WIN_UCRT, [test -n ""])
fi
])

AC_DEFUN([OC_CHECK_LIBUUID],[
AC_REQUIRE([OC_CHECK_RUN_ON_WIN])
	
	oc_lib_uuid=yes
	if test "x$oc_run_win" != xyes; then
		AC_CHECK_LIB(uuid, uuid_parse,, 
			AC_COMPILE_IFELSE([
#include <uuid/uuid.h>
int main(int argc, char** argv) {
	int result;
	uuid_t uid;
	result = 0;
	uuid_parse("E6FB3AC2-3850-4D7B-B7C0-9EE3C15AE006", uid);
	return result;
}	
			], [oc_lib_uuid=no])	
		)	
	else
		oc_lib_uuid=no
	fi
	AM_CONDITIONAL(USE_LIB_UUID, [test "x$oc_lib_uuid" == xyes])
])

