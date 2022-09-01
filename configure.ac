AC_INIT([credential-helper-oc],[0.1.0],[toshi@oc-soft.net])

AM_INIT_AUTOMAKE
LT_INIT([win32-dll])

AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_SUBDIRS([oclib])

AC_PROG_CC

AC_CHECK_LIB([json-c], [json_object_new_object])
AC_CHECK_LIB([curl], [curl_url])

AC_CHECK_HEADER([json-c/json.h])
AC_CHECK_HEADER([curl/curl.h])

AC_CONFIG_FILES([Makefile
    src/Makefile
    src/scripts/Makefile
    src/lmdcli/Makefile])

AC_OUTPUT

# vi: se ts=4 sw=4 et:
