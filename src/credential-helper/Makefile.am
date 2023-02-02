
bin_PROGRAMS = credhelper

noinst_HEADERS= cred_helper_types.h \
	cred_helper.h \
	cred_helper_i.h \
	cred_helper_op.h \
	cred_helper_parser.h \
	client_id.h \
	client_secret.h \
	credential_op.h \
	credential_storage_key_id_table.h \
	exe_info.h \
	http.h \
	l10n.h \
	lmd_connections.h \
	lmd_op.h \
	lmd.h \
	lmd_i.h \
	lmd_parser.h \
	lmd_requests.h \
	lmd_types.h \
	mem_dbg.h \
	option.h \
	url.h

credhelper_src = main.c \
	cred_helper.c \
	cred_helper_i.c \
	cred_helper_op.c \
	cred_helper_parser.c \
	lmd.c \
	lmd_parser.c \
	client_secret.c \
	client_id.c \
	lmd_connections.c \
	lmd_requests.c \
	lmd_op.c \
	lmd_i.c \
	mem_dbg.c

if !TARGET_WASM
credhelper_SOURCES = $(credhelper_src)
else
credhelper_SOURCES = main_wasm.c
endif

deploy_dir = @DEPLOY_DIR@
pkg_bindir = @pkg_bindir@
deploy_dir_def = $(abs_top_builddir)/pkg-img

if RUN_WIN
credhelper_SOURCES += exe_info_win.c
credhelper_SOURCES += l10n_user_win.c
endif
if RUN_LINUX
credhelper_SOURCES += exe_info_linux.c
credhelper_SOURCES += l10n_user_linux.c
endif

if RUN_WIN
credhelper_SOURCES += credhelper_res.rc
endif

credhelper_CFLAGS= -g -O0
credhelper_LDFLAGS= -g -static

if !TARGET_WASM
credhelper_lib_flags=$(LTLIBINTL) -ljson-c -lcurl -lsqlite3 -lm
else
credhelper_lib_flags=$(LTLIBINTL) -ljson-c -lsqlite3 -lm
endif



credhelper_LDADD=$(top_builddir)/oclib/buffer/src/libocbuffer.la \
	$(top_builddir)/oclib/cstr/src/liboccstr.la \
	$(top_builddir)/src/core/liboccredcore.la \
	$(credhelper_lib_flags)

if TARGET_WASM
credhelper_LDFLAGS += -static
endif

SH_LOG_DRIVER = env AM_TAP_AWK='$(AWK)' $(SHELL) $(top_srcdir)/tap-driver.sh
TEST_EXTENSIONS = .sh
SH_LOG_COMPILER = $(SHELL)
TESTS = t_store_01.sh t_site_01.sh t_erase_01.sh t_erase_02.sh
AM_TESTS_ENVIRONMENT = HOME=$(builddir);
CLIENT_ID_TXT=$(top_srcdir)/src/config/client-id.txt
CLIENT_SECRET_TXT=$(top_srcdir)/src/config/client-secret.txt
UI_TOKEN_EXEC = electron

pkgdatadir=$(datadir)/$(PACKAGE_NAME)

textdomain_dir=$(pkgdatadir)/locale

ui_exe_name=electron$(EXEEXT)

ui_builddir=$(builddir)/../ui
ui_exe_dir=$(ui_builddir)/pkg
ui_exe_subdir=ui
ui_exe_relative_path=$(ui_exe_subdir)/$(ui_exe_name)

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

awk_cmd_1 = '{ \
			gsub(/@LMTD_APP_FILE_NAME@/, \
				"\"credhelper\""); \
			gsub(/@LMTD_APP_PRODUCT_NAME@/, \
				"\"CLI for limited device access\""); \
			print \
		} \
	'
awk_cmd_2 = 'BEGIN { \
		map_size = 0; \
		src_file = "$(srcdir)/lmd_res.rc.in"; \
	} \
	{ \
		langs[map_size] = $$1; \
		inc_file[map_size] = $$2; \
		map_size++; \
	} \
	END { \
		trans_lines = "\"Translation\", 0x0, 1200"; \
		if (map_size > 0) { \
			for (i = 0; i < map_size; i++) { \
				trans_lines = trans_lines ",\n" langs[i] ", 1200"; \
			} \
			inc_lines = "\x23include \"" inc_file[0] "\""; \
			for (i = 1; i < map_size; i++) { \
				inc_lines = inc_lines "\n\x23include \"" inc_file[i] "\""; \
			} \
		} \
		while ((getline line < src_file) > 0) { \
			gsub(/@INCLUDE@/, inc_lines, line); \
			gsub(/@TRANSLATION@/, trans_lines, line); \
			print line; \
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

credhelper_CPPFLAGS=-I$(top_srcdir)/oclib/buffer/include \
	-I$(top_srcdir)/oclib/cstr/include \
	-I$(top_srcdir)/src/core

$(builddir)/client-secret-dbg.txt:
	: > $@

$(builddir)/client-id-dbg.txt:
	: > $@


mem_dbg.c : mem_dbg_off.c.in
	cp $< $@

mem_dbg: mem_dbg_on.c.in
	cp $< mem_dbg.c

.PHONY: mem_dbg

client-dbg:
	cp $(srcdir)/../config/client-secret-dbg.txt \
		$(builddir)/client-secret-dbg.txt 
	cp $(srcdir)/../config/client-id-dbg.txt \
		$(builddir)/client-id-dbg.txt 

clean-client-dbg:
	rm $(builddir)/client-secret-dbg.txt
	rm $(builddir)/client-id-dbg.txt 

.PHONY: client-dbg clean-client-dbg

all-local: rt_get_01.sh rt_ui_01.sh

t_site_01.sh : t_site_01.sh.in
	sed -e 's!%CLIENT_ID_PATH%!$(top_srcdir)/src/config/client-id.txt!' $< | \
	sed -e 's!%CLIENT_ID_DBG_PATH%!$(builddir)/client-id-dbg.txt!' | \
	sed -e 's!%CLIENT_SECRET_PATH%!$(top_srcdir)/src/config/client-secret.txt!' | \
	sed -e 's!%CLIENT_SECRET_DBG_PATH%!$(builddir)/client-secret-dbg.txt!' | \
	sed -e 's!%AWK%!$(AWK)!' \
	> $@
rt_get_01.sh : rt_get_01.sh.in
	sed -e 's!%CLIENT_ID_PATH%!$(abs_top_srcdir)/src/config/client-id.txt!' \
		$< | \
	sed -e 's!%CLI%!$(abs_builddir)/credhelper$(EXEEXT)!' | \
	sed -e 's!%AWK%!$(AWK)!' >$@
	chmod u+x $@

rt_ui_01.sh : rt_ui_01.sh.in | link-ui
	sed -e 's!%UI_EXE_RELPATH%!$(ui_exe_relative_path)!' $< >$@
	chmod u+x $@

client_secret.gpf : client_secret.gpf.in $(srcdir)/../config/client-secret.txt $(builddir)/client-secret-dbg.txt 
	awk -v in_file=$< -e $(awk_cmd_0) \
		$(srcdir)/../config/client-secret.txt \
		$(builddir)/client-secret-dbg.txt >$@

client_id.gpf : client_id.gpf.in $(srcdir)/../config/client-id.txt $(builddir)/client-id-dbg.txt
	awk -v in_file=$< -e $(awk_cmd_0) \
		$(srcdir)/../config/client-id.txt \
		$(builddir)/client-id-dbg.txt >$@

exec_names.c: exec_names.c.in
	$(AWK) -v token_exec=$(UI_TOKEN_EXEC) -e ' \
		{ \
			gsub(/!UI_TOKEN_GEN_EXEC!/, token_exec); \
		} \
	'


lmd_const.h : lmd_const.h.in $(srcdir)/Makefile.am
	awk $(awk_cmd_1) $< >$@

lmd_res.rc : lmd_res.rc.in lmd_const.h
	awk $(awk_cmd_2) $(top_srcdir)/win/lang_map.txt > $@

pkg-install-local:
	depdir=$(deploy_dir) ; \
	if test -z $$depdir ;  then \
		depdir=$(deploy_dir_def); \
	fi; \
	$(MAKE) $(AM_MAKEFLAGS) DESTDIR=$$depdir \
		bindir=$(pkg_bindir) install-exec-am install-data-am

pkg-uninstall-local:
	depdir=$(deploy_dir); \
	if test -z $$depdir ; then \
		depdir=$(deploy_dir_def); \
	fi; \
	$(MAKE) $(AM_MAKEFLAGS) DESTDIR=$$depdir bindir=$(pkg_bindir) uninstall-am

link-ui:
	if  [ ! -h $(ui_exe_subdir) ]; then \
		$(LN_S) $(ui_exe_dir) $(ui_exe_subdir) ; \
	fi

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
