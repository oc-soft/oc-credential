
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

AC_DEFUN([OC_CHECK_TARGET_WASM],
	[
oc_target_wasm=no
AC_COMPILE_IFELSE(
	[AC_LANG_SOURCE([
#ifndef __EMSCRIPTEN__
#error The compiler is not emscripten 
#endif
	])],
	[oc_target_wasm=yes],
	[oc_target_wasm=no]) 
AM_CONDITIONAL(TARGET_WASM, [test "x$oc_target_wasm" == xyes])
])

AC_DEFUN([OC_CHECK_ELECTRON_TARGET],[
AC_REQUIRE([AC_CANONICAL_TARGET])
electron_target_cpu=
	case $target_cpu in 
	x86_64)
		electron_target_cpu=x64
		;;
	aarch64)
		electron_target_cpu=arm64
		;;
	esac
electron_target_os=
	case $target_os in
	*linux*) electron_target_os=linux;;
	*darwin*) 
		electron_target_os=darwin
		;;
	msdosdjgpp* | cyg* | mingw* | cegcc*)
		electron_target_os=win32
		;;
	esac

AC_SUBST([ELECTRON_TARGET_CPU], [$electron_target_cpu])
AC_SUBST([ELECTRON_TARGET_OS], [$electron_target_os])
])

AC_DEFUN([OC_CHECK_MAC_TARGET],[
AC_REQUIRE([AC_CANONICAL_TARGET])
	case $target_os in
	*darwin*) 
		oc_target_os=darwin
		;;
	esac

AM_CONDITIONAL([TARGET_MACOS], [test x$oc_target_os = xdarwin])
])

AC_DEFUN([OC_CHECK_LINER_OPTION], [
OC_XLINKER_OPTION=
case $build_os in
	darwin*)
		if test "$build_cpu" != "$host_cpu" ; then
			case $host_cpu in
				aarch64)
					OC_XLINKER_OPTION='-Wl,-arch -Wl,arm64'
				;;
				x86_64)
					OC_XLINKER_OPTION='-Wl,-arch -Wl,x86_64'
				;;	
			esac	
		fi
	;;
esac
AC_SUBST(OC_XLINKER_OPTION)
])

# vi: se ts=4 sw=4:
