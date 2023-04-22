AC_INIT([credential-ocs],[0.1.0],[toshi@oc-soft.net])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE

LT_INIT([win32-dll])

AM_GNU_GETTEXT_VERSION([0.21.1])

AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_SUBDIRS([oclib])
LT_LANG([Windows Resource])

AC_PROG_CC
AC_CHECK_PROG([GPERF], [gperf --version], [gperf])
AC_CHECK_PROG([HEXDUMP], [hexdump --version], [hexdump])
AC_CHECK_PROG([SQLITE3], [sqlite3 --version], [sqlite])
AC_CHECK_PROG([NODE], [node --version], [node])
AC_CHECK_PROG([NPM], [npm --version], [npm])
AC_CHECK_PROG([CURL], [curl --version], [curl])
AC_CHECK_PROG([UNZIP], [unzip --version], [unzip])

AC_CHECK_TOOL([WINDRES], [windres])

AC_CHECK_LIB([json-c], [json_object_new_object])
AC_CHECK_LIB([curl], [curl_url])
AC_CHECK_LIB([sqlite3], [sqlite3_initialize])

AC_CHECK_HEADER([json-c/json.h])
AC_CHECK_HEADER([curl/curl.h])

AC_CHECK_HEADERS([mcheck.h emscripten.h])


OC_CHECK_RUN_ON_WIN
if test "x$oc_run_win" = xyes; then
LT_PROG_RC
fi

OC_CHECK_TARGET_WASM
OC_CHECK_ELECTRON_TARGET
OC_CHECK_MAC_TARGET

if test "x$oc_run_win" = xyes; then
    oc_data_dir_name='credential-ocs'
else
    oc_data_dir_name='.credential-ocs'
fi
AC_SUBST([OC_DATA_DIR_NAME], [$oc_data_dir_name])
AC_SUBST([OC_DB_FILE_NAME], [user-token])
AC_SUBST([OC_DB_FILE_EXT], [.bsql])

if [[ "x$oc_run_win" = xyes ]]; then
    AC_SUBST([pkg_localedir], [/app/locale])
    AC_SUBST([pkg_bindir], [/app])
else
    AC_SUBST([pkg_localedir], [/locale])
    AC_SUBST([pkg_bindir], [/])
fi

AC_ARG_VAR([DEPLOY_DIR], [specifiy directory to deploy programs])
AC_ARG_VAR([DEPLOY_LIB_DIR], [specify library directory deploy image])


AM_GNU_GETTEXT([external])

oc_ver_major=`echo ${PACKAGE_VERSION} | awk -e '{ split($1, vst, "."); print vst[[1]]; }'`
oc_ver_minor=`echo ${PACKAGE_VERSION} | awk -e '{ split($1, vst, "."); print vst[[2]]; }'`
oc_ver_rev=`echo ${PACKAGE_VERSION} | awk -e '{ split($1, vst, "."); print vst[[3]]; }'`

oc_company_name=OC\ Soft

AC_DEFINE_UNQUOTED([VERSION_MAJOR], [$oc_ver_major], [Major version])
AC_DEFINE_UNQUOTED([VERSION_MINOR], [$oc_ver_minor], [Minor version])
AC_DEFINE_UNQUOTED([VERSION_REVISION], [$oc_ver_rev], [Revision])
AC_DEFINE_UNQUOTED([COMPANY_NAME], ["$oc_company_name"], [Company name])
AC_DEFINE_UNQUOTED([COMPANY_NAME_W], [L"$oc_company_name"], [Wide char company name])
AC_DEFINE_UNQUOTED([VERSION_W], [L"$VERSION"], [wide char package version])
AC_DEFINE_UNQUOTED([GETTEXT_DOMAIN], ["$PACKAGE_NAME"], [gettext domain name])


if test $host_os = "cygwin" ; then
    case $build_cpu in
        x86_64*)
            oc_dll_root=/usr/x86_64-w64-mingw32/sys-root/mingw/bin
        ;;
        i*86)
        ;;
    esac
fi
OC_DLL_SYS_ROOT=$oc_dll_root
OC_COMPANY_NAME=$oc_company_name

AC_SUBST([OC_DLL_SYS_ROOT])
AC_SUBST([OC_COMPANY_NAME])
AC_SUBST([LOCPATH_NAME], [locpath.txt])
AC_SUBST([LOGGING_FILE_NAME], [log.txt])
AC_SUBST([LOGGING_MAX_MESSAGE_SIZE], [1024])

case $host_os in
    linux*)
        oc_run_linux=yes
    ;;
esac
AM_CONDITIONAL(RUN_LINUX, test "x$oc_run_linux" = xyes)


case $build_os in
    msdosdjgpp* | cyg* | mingw* | cegcc*)
        oc_build_ms=yes
    ;;
esac

AM_CONDITIONAL(BUILD_MS, test "x$oc_build_ms" = xyes)

if test "x$oc_target_wasm" = xno ; then
    if test "x$WASM_BUILD_ROOT" == "x" ; then
        WASM_BUILD_ROOT='$(abs_top_srcdir)/build-wasm'
    fi

else
    WASM_BUILD_ROOT='$(abs_top_builddir)'
fi

if test "x$CORE_MODULE_DIR" == "x" ; then
    CORE_MODULE_DIR="$WASM_BUILD_ROOT/src/core"
fi


AC_SUBST([CORE_MODULE_DIR])

GUI_EXE_SUBDIR=ui
AC_SUBST([GUI_EXE_SUBDIR])

OC_EXE_ROOT_NAME=credential-ocs
AC_SUBST([OC_EXE_ROOT_NAME])

AC_ARG_ENABLE([hardcode-libs],
    [AS_HELP_STRING([--disable-hardcode-libs],
        [suppress to write rath or runpath into library by libtool link])],
        [hardcode_into_libs=no])

AC_ARG_ENABLE([dist-private],
    [AS_HELP_STRING([--enable-dist-private],
        [include private data into dist package])],
    [oc_enable_dist_private=yes])

AM_CONDITIONAL(OC_ENABLE_DIST_PRIVATE,
    [test "x${oc_enable_dist_private}" = "xyes"])


## hardcode_into_libs

AM_EXTRA_RECURSIVE_TARGETS([pkg-install pkg-uninstall])


AC_CONFIG_HEADERS([include/config.h])

AC_CONFIG_FILES([Makefile
    em-setting/Makefile
    src/Makefile
    src/config/Makefile
    src/core/Makefile
    src/core/user_resource_const.h
    src/credential-helper/Makefile
    src/credential-helper/logging_win.h
    src/ui/Makefile
    pkg/Makefile
    pkg/win/Makefile
    pkg/win/ja/Makefile
    po/Makefile.in])

AC_REQUIRE_AUX_FILE([tap-driver.sh])

AC_OUTPUT

# vi: se ts=4 sw=4 et:
