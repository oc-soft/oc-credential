lib_LTLIBRARIES = liboccredcore.la

liboccredcore_la_SOURCES = credential.c \
	credential_i.c \
	app_config_i.c \
	app_config.c \
	user_resource.c \
	user_resource_i.c \
	credential_desc.gpf \
	credential_storage_sqlite.c 

deploy_dir = @DEPLOY_DIR@
pkg_bindir = @pkg_bindir@
deploy_dir_def = $(abs_top_builddir)/pkg-img

if RUN_WIN
liboccredcore_la_SOURCES += user_resource_win.c
endif
if RUN_LINUX
liboccredcore_la_SOURCES += user_resource_linux.c
endif


liboccredcore_la_CFLAGS= -g -O0


liboccredcore_lib_flags=$(LTLIBINTL) -ljson-c -lsqlite3 -lm

liboccredcore_la_LIBADD= \
	$(top_builddir)/oclib/cstr/src/liboccstr.la 

liboccredcore_la_LDFLAGS= -g \
	$(liboccredcore_lib_flags)

SH_LOG_DRIVER = env AM_TAP_AWK='$(AWK)' $(SHELL) $(top_srcdir)/tap-driver.sh
TEST_EXTENSIONS = .sh
SH_LOG_COMPILER = $(SHELL)

AM_TESTS_ENVIRONMENT = HOME=$(builddir);

pkgdatadir=$(datadir)/$(PACKAGE_NAME)

awk_cmd_0='BEGIN { \
		FS=","; \
		entries[0,0] = ""; \
	} \
	{ \
		protocol = $$1; \
		host = $$2; \
		path = $$3; \
		client_data = $$4; \
		key = protocol "://" host; \
		if (length(path)) { \
			key = key "/" path; \
		} \
		entries[NR, 1] = key; \
		entries[NR, 2] = client_data; \
		rows = NR; \
	} \
	END { \
		replacement = ""; \
		for (ridx = 1; ridx <= rows; ridx++) { \
			key = entries[ridx, 1]; \
			value = "\"" entries[ridx, 2] "\""; \
			replacement = replacement key ", " value; \
			if (ridx <= rows - 1) { \
				replacement = replacement "\n"; \
			} \
		} \
		while (getline <in_file) { \
			sub(/@URI_CLIENT_ENTRIES@/, replacement); \
			print $$0; \
		} \
	}' 


awk_cmd_3 = 'BEGIN { \
		line_num = 0; \
		lines[0] = ""; \
	} \
	{ \
		line = ""; \
		for (i = 1; i <= NF; i++) { \
			if ("0x" != $$(i)) { \
				if (line == "") { \
					line = "    " $$(i); \
				} else { \
					line = line ", " $$(i); \
				} \
			} \
		} \
		lines[line_num] = line; \
		line_num++; \
	} \
	END { \
		replacement = ""; \
		if (NR) { \
			replacement = lines[0]; \
			for (i = 1; i < NR; i++) { \
				replacement = replacement ",\n" lines[i]; \
			} \
		} \
		while (getline <in_file) { \
			sub(/@APP_CONFIG@/, replacement); \
			print $$0; \
		} \
	}'

liboccredcore_la_CPPFLAGS= -I$(top_srcdir)/oclib/cstr/include


app_config_i.c: app_config_i.c.in $(top_srcdir)/rc/rc.json 
	$(HEXDUMP) -e "\"    \" 12/1 \"0x%02x \" \"\n\"" $(top_srcdir)/rc/rc.json | awk -v in_file=$< -e $(awk_cmd_3) >$@ 

pkg-install-local:
	depdir=$(deploy_dir) ; \
	if test -z $$depdir ;  then \
		depdir=$(deploy_dir_def); \
	fi; \
	$(MAKE) $(AM_MAKEFLAGS) DESTDIR=$$depdir bindir=$(pkg_bindir) install-exec-am install-data-am

pkg-uninstall-local:
	depdir=$(deploy_dir); \
	if test -z $$depdir ; then \
		depdir=$(deploy_dir_def); \
	fi; \
	$(MAKE) $(AM_MAKEFLAGS) DESTDIR=$$depdir bindir=$(pkg_bindir) uninstall-am


clean-generic:
	rm -f *.rc	

.rc.o:
	$(LIBTOOL) $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --tag=RC --mode=compile \
		$(RC) $(RCFLAGS) \
			--preprocessor $(CC) \
			--preprocessor-arg -E \
			--preprocessor-arg -x \
			--preprocessor-arg c \
			--preprocessor-arg -I$(top_builddir)/include \
			--preprocessor-arg -I$(builddir) \
			--preprocessor-arg -I$(srcdir) \
			-i $< -o $@
.gpf.c:
	$(GPERF) --output-file=$@ $< 

# vi: se ts=4 sw=4 noet:
