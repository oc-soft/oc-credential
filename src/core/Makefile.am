noinst_LTLIBRARIES = liboccredcore.la

noinst_PROGRAMS = t0_credential_desc

if TARGET_WASM
bin_PROGRAMS = occredcore
endif

noinst_HEADERS = user_resource.h \
	app_config.h \
	app_config_i.h \
	credential_desc.h \
	credential.h \
	credential_i.h \
	credential_storage.h \
	user_resource.h \
	user_resource_i.h \
	wasm_i.h

liboccredcore_la_SOURCES = credential.c \
	credential_i.c \
	app_config_i.c \
	app_config.c \
	user_resource.c \
	user_resource_i.c \
	credential_desc.gpf \
	credential_storage_sqlite.c 


occredcore_SOURCES = credcore_main_wasm.c

t0_credential_desc_SOURCES = t0_credential_desc.c

occredcore_DEPENDENCIES = liboccredcore.la
t0_credential_desc_DEPENDENCIES = liboccredcore.la

deploy_dir = @DEPLOY_DIR@
pkg_bindir = @pkg_bindir@
deploy_dir_def = $(abs_top_builddir)/pkg-img
JS_OC_CREDCORE_MOD_NAME = createCredCore
JS_MODEXT=.mjs
TESTS=t0_credential_desc.sh

if RUN_WIN
liboccredcore_la_SOURCES += user_resource_win.c
endif
if RUN_LINUX
liboccredcore_la_SOURCES += user_resource_linux.c
endif


liboccredcore_la_CFLAGS= -g -O0


liboccredcore_lib_flags=$(LTLIBINTL) -ljson-c -lsqlite3 -lm

liboccredcore_la_LIBADD= \
	$(top_builddir)/oclib/cstr/src/liboccstr.la \
	$(top_builddir)/oclib/buffer/src/libocbuffer.la

liboccredcore_la_LDFLAGS= -g \
	$(liboccredcore_lib_flags)

t0_credential_desc_LDADD = liboccredcore.la 

if TARGET_WASM
occredcore_LDADD = liboccredcore.la
occredcore_LDFLAGS = -sMODULARIZE=1 \
	-sEXPORT_NAME=$(JS_OC_CREDCORE_MOD_NAME) \
	--js-library=$(srcdir)/library_core.js \
	--js-library=$(top_builddir)/em-setting/library_type_size.js \
	-sEXPORTED_FUNCTIONS=@$(srcdir)/exported-functions.utf-8 \
	-sEXPORTED_RUNTIME_METHODS=@$(srcdir)/runtime-methods.utf-8 

occredcore_LINK = $(LIBTOOL) $(AM_V_lt) --tag=CC $(AM_LIBTOOLFLAGS) \
	$(LIBTOOLFLAGS) --mode=link $(CCLD) $(AM_CFLAGS) $(CFLAGS) \
	$(occredcore_LDFLAGS) $(LDFLAGS) -o occredcore$(JS_MODEXT)


endif

SH_LOG_DRIVER = env AM_TAP_AWK='$(AWK)' $(SHELL) $(top_srcdir)/tap-driver.sh
TEST_EXTENSIONS = .sh
SH_LOG_COMPILER = $(SHELL)

AM_TESTS_ENVIRONMENT = HOME=$(builddir);

pkgdatadir=$(datadir)/$(PACKAGE_NAME)

liboccredcore_la_CPPFLAGS = -I$(top_srcdir)/oclib/cstr/include \
	-I$(top_srcdir)/oclib/buffer/include

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

awk_cmd_4 = ' \
	{ \
		gsub(/!VERSION!/, version); \
		gsub(/!MODULE!/, module); \
		print $$0; \
	} \
'

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

package.json: package.json.in $(srcdir)/Makefile.am 
	$(AWK) -v version=$(PACKAGE_VERSION) -v module=occredcore$(JS_MODEXT) -e $(awk_cmd_4) $< >$@


t0_credential_desc.sh : t0_credential_desc.sh.in t0_credential_desc$(EXEEXT)
	$(AWK) -v prog_name='$(builddir)/t0_credential_desc$(EXEEXT)' \
		-e '{ \
			gsub(/!TEST_PROG!/, prog_name); \
			print $$0; \
		}' $< >$@

check-local: t0_credential_desc.sh 

clean-generic:
	rm -f *.rc	

clean-local:
	rm -f *.wasm
	rm -f *$(JS_MODEXT)
	rm -f *.js
	rm -f *.json
	rm -f *.ts

occredcore.d.ts: decl.d.ts.in
	$(AWK) \
		-v create_mod=$(JS_OC_CREDCORE_MOD_NAME) \
		-v mod_name=occredcore \
		-e ' { \
			gsub(/!MODULE_NAME!/, mod_name); \
			gsub(/!CREATE_MODULE!/, create_mod); \
			print $$0; \
		} \
		' $< >$@

all-local: package.json occredcore.d.ts

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
