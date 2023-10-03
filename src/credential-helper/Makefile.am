
libexec_PROGRAMS = credential-ocs/credhelper
check_PROGRAMS = credential-ocs/t01_token_gen_ui_exec \
	t01_l10n
noinst_PROGRAMS = credential-ocs/creddef

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
	exec_names.h \
	fd_io.h \
	file_desc.h \
	std_file_desc.h \
	http.h \
	l10n.h \
	l10n_i.h \
	lmd_connections.h \
	lmd_op.h \
	lmd.h \
	lmd_i.h \
	lmd_parser.h \
	lmd_requests.h \
	lmd_types.h \
	mem_dbg.h \
	option.h \
	logging.h \
	logging_win.h \
	ui_token_gen.h \
	ui_token_gen_i.h \
	ui_token_gen_types.h \
	token_gen_ui.h \
	token_gen_ui_exec.h \
	token_gen_ui_i.h \
	path.h \
	time_zone.h \
	url.h


credhelper_src = main.c \
	cred_helper.c \
	cred_helper_i.c \
	cred_helper_op.c \
	cred_helper_parser.c \
	file_desc.c \
	std_file_desc.c \
	lmd.c \
	lmd_parser.c \
	client_secret.c \
	client_id.c \
	lmd_connections.c \
	lmd_requests.c \
	lmd_op.c \
	lmd_i.c \
	l10n_i.c \
	token_gen_ui.c \
	token_gen_ui_exec.c \
	token_gen_ui_i.c \
	ui_token_gen.c \
	ui_token_gen_i.c \
	exec_names.c \
	mem_dbg.c

EXTRA_DIST = client_id.gpf.in \
	credhelper_res.rc.in \
	t_site_01.sh.in \
	rt_ui_01.sh.in \
	mem_dbg_on.c.in \
	rt_get_01.sh.in \
	l10n_i.c.in \
	logging_win.h.in \
	credhelper_const.h.in \
	mem_dbg_off.c.in \
	rt_get_02.sh.in \
	exec_names.c.in \
	client_secret.gpf.in  \
	t_store_01.sh \
	t_erase_01.sh \
	t_erase_02.sh \
	t_l10n_01.sh

if !TARGET_WASM
credential_ocs_credhelper_SOURCES = $(credhelper_src)
else
credential_ocs_credhelper_SOURCES = main_wasm.c
endif

deploy_dir = @DEPLOY_DIR@
pkg_bindir = @pkg_bindir@
GUI_EXE_SUBDIR=@GUI_EXE_SUBDIR@
OC_EXE_ROOT_NAME=@OC_EXE_ROOT_NAME@
LOCPATH_NAME=@LOCPATH_NAME@
LOGGING_FILE_NAME=@LOGGING_FILE_NAME@
LOGGING_MAX_MESSAGE_SIZE=@LOGGING_MAX_MESSAGE_SIZE@
OC_XLINKER_OPTION = @OC_XLINKER_OPTION@

PRODUCT_NAME=@PRODUCT_NAME@
APP_FILE_NAME=@APP_FILE_NAME@
APP_FILE_DESC=@APP_FILE_DESC@
WINDRES_CPP_OPTION=@WINDRES_CPP_OPTION@

SKIP_LOCPATH_INSTALL=
deploy_dir_def = $(abs_top_builddir)/pkg-img

if RUN_WIN
credential_ocs_credhelper_SOURCES += exe_info_win.c
credential_ocs_credhelper_SOURCES += l10n_user_win.c
credential_ocs_credhelper_SOURCES += fd_io_win.c
credential_ocs_credhelper_SOURCES += token_gen_ui_i_win.c
credential_ocs_credhelper_SOURCES += path_win.c
credential_ocs_credhelper_SOURCES += logging_win.c
credential_ocs_credhelper_SOURCES += time_zone_win.c
endif
if RUN_LINUX
credential_ocs_credhelper_SOURCES += exe_info_linux.c
credential_ocs_credhelper_SOURCES += l10n_user_linux.c
credential_ocs_credhelper_SOURCES += fd_io.c
credential_ocs_credhelper_SOURCES += token_gen_ui_i_posix.c
credential_ocs_credhelper_SOURCES += path.c
credential_ocs_credhelper_SOURCES += logging.c
credential_ocs_credhelper_SOURCES += time_zone.c
endif
if RUN_DARWIN
credential_ocs_credhelper_SOURCES += exe_info_darwin.c
credential_ocs_credhelper_SOURCES += l10n_user_posix.c
credential_ocs_credhelper_SOURCES += fd_io.c
credential_ocs_credhelper_SOURCES += token_gen_ui_i_posix.c
credential_ocs_credhelper_SOURCES += path.c
credential_ocs_credhelper_SOURCES += logging.c
credential_ocs_credhelper_SOURCES += time_zone.c
endif


if RUN_WIN
credential_ocs_credhelper_SOURCES += credhelper_res.rc
endif

if !RUN_DARWIN
ECHO_NO_NEWLINE = echo $(ECHO_N)
else
ECHO_NO_NEWLINE = /bin/echo -n 
endif

credential_ocs_credhelper_CFLAGS= -g -O0
credential_ocs_credhelper_LDFLAGS= -g -static $(OC_XLINKER_OPTION)

AM_LDFLAGS = $(OC_XLINKER_OPTION)

if RUN_DARWIN
LN_S_R = $(LN_S)
else
LN_S_R = $(LN_S) -r
endif

credential_ocs_t01_token_gen_ui_exec_SOURCES = t01_token_gen_ui_exec.c \
	token_gen_ui_exec.c \
	exec_names.c

if RUN_WIN
credential_ocs_t01_token_gen_ui_exec_SOURCES += path_win.c exe_info_win.c
endif
if RUN_DARWIN
credential_ocs_t01_token_gen_ui_exec_SOURCES += path.c exe_info_darwin.c
else
credential_ocs_t01_token_gen_ui_exec_SOURCES += path.c exe_info_linux.c
endif

credential_ocs_creddef_SOURCES = cred_default.c

t01_l10n_SOURCES = t01_l10n.c \
	l10n_i.c

if RUN_WIN
t01_l10n_SOURCES += path_win.c exe_info_win.c
else
t01_l10n_SOURCES += path.c exe_info_linux.c
endif

if !TARGET_WASM
credhelper_lib_flags=$(LTLIBINTL) -ljson-c -lcurl -lsqlite3 -luuid -lm
else
credhelper_lib_flags=$(LTLIBINTL) -ljson-c -lsqlite3 -luuid -lm
endif



credential_ocs_credhelper_LDADD= \
	$(top_builddir)/oclib/buffer/src/libocbuffer.la \
	$(top_builddir)/oclib/cstr/src/liboccstr.la \
	$(top_builddir)/src/core/liboccredcore.la \
	$(credhelper_lib_flags)

credential_ocs_creddef_LDADD=$(top_builddir)/src/core/liboccredcore.la


if RUN_WIN
credential_ocs_credhelper_LDFLAGS += -Wl,--dependency-file -Wl,credhhelper-dpd.txt -specs=$(srcdir)/ucrt.specs
endif

SH_LOG_DRIVER = env AM_TAP_AWK='$(AWK)' $(SHELL) $(top_srcdir)/tap-driver.sh
TEST_EXTENSIONS = .sh
SH_LOG_COMPILER = $(SHELL)
TESTS = t_store_01.sh t_site_01.sh t_erase_01.sh t_erase_02.sh t_l10n_01.sh
AM_TESTS_ENVIRONMENT = HOME=$(builddir);
CLIENT_ID_TXT=$(top_srcdir)/src/config/client-id.txt
CLIENT_SECRET_TXT=$(top_srcdir)/src/config/client-secret.txt
CLEAN_FOR_DEBUG =
pkgdatadir=$(datadir)/$(PACKAGE_NAME)

localedir = @localedir@

UI_TOKEN_GENERATOR_NAME = @GUI_TOKEN_GENERATOR_NAME@

textdomain_dir=$(pkgdatadir)/locale

ui_exe_name=$(UI_TOKEN_GENERATOR_NAME)$(EXEEXT)


ui_builddir=$(builddir)/../ui
ui_exe_dir=$(ui_builddir)/pkg
ui_exe_subdir=$(GUI_EXE_SUBDIR)
if !RUN_DARWIN
ui_exe_relative_path=$(ui_exe_subdir)/$(ui_exe_name)
else
ui_exe_relative_path=$(ui_exe_subdir)/$(UI_TOKEN_GENERATOR_NAME).app/Contents/MacOS/$(ui_exe_name)
endif

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
			gsub(/!CREDHELPER_APP_FILE_NAME!/, \
				"\"$(APP_FILE_NAME)\""); \
			gsub(/!CREDHELPER_APP_PRODUCT_NAME!/, \
				"\"$(PRODUCT_NAME)\""); \
			gsub(/!CREDHELPER_APP_FILE_DESC!/, \
				"\"$(APP_FILE_DESC)\""); \
			print; \
		} \
	'

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

credential_ocs_credhelper_CPPFLAGS=-I$(top_srcdir)/oclib/buffer/include \
	-I$(top_srcdir)/oclib/cstr/include \
	-I$(top_srcdir)/src/core

credential_ocs_creddef_CPPFLAGS= -I$(top_srcdir)/src/core


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

all-local: rt_get_01.sh rt_get_02.sh rt_ui_01.sh 

t_site_01.sh : t_site_01.sh.in
	sed -e 's!%CLIENT_ID_PATH%!$(top_srcdir)/src/config/client-id.txt!' \
		-e 's!%CLIENT_ID_DBG_PATH%!$(builddir)/client-id-dbg.txt!' \
		-e 's!%CLIENT_SECRET_PATH%!$(top_srcdir)/src/config/client-secret.txt!' \
		-e 's!%CLIENT_SECRET_DBG_PATH%!$(builddir)/client-secret-dbg.txt!' \
		-e 's!%CLI%!$(abs_builddir)/credential-ocs/credhelper$(EXEEXT)!' \
		-e 's!%AWK%!$(AWK)!' $< \
	> $@
rt_get_01.sh : rt_get_01.sh.in
	sed -e 's!%CLIENT_ID_PATH%!$(abs_top_srcdir)/src/config/client-id.txt!' \
		-e 's!%CLI%!$(abs_builddir)/credential-ocs/credhelper$(EXEEXT)!' \
		-e 's!%AWK%!$(AWK)!' $< >$@
	chmod u+x $@

rt_get_02.sh : rt_get_02.sh.in
	sed -e 's!%CLI%!$(abs_builddir)/credential-ocs/credhelper$(EXEEXT)!' \
		-e 's!%CLIENT_ID_PATH%!$(abs_top_srcdir)/src/config/client-id.txt!' \
		-e 's!%AWK%!$(AWK)!' $< >$@
	chmod u+x $@


rt_ui_01.sh : rt_ui_01.sh.in | link-ui
	sed -e 's!%UI_EXE_RELPATH%!credential-ocs/$(ui_exe_relative_path)!' $< >$@
	chmod u+x $@


t_site_01.log: t_store_01.log

t_erase_01.log: t_site_01.log

t_erase_02.log: t_erase_01.log

client_secret.gpf : client_secret.gpf.in $(srcdir)/../config/client-secret.txt $(builddir)/client-secret-dbg.txt 
	awk -v in_file=$< -e $(awk_cmd_0) \
		$(srcdir)/../config/client-secret.txt \
		$(builddir)/client-secret-dbg.txt >$@

client_id.gpf : client_id.gpf.in $(srcdir)/../config/client-id.txt $(builddir)/client-id-dbg.txt
	awk -v in_file=$< -e $(awk_cmd_0) \
		$(srcdir)/../config/client-id.txt \
		$(builddir)/client-id-dbg.txt >$@

exec_names.c: exec_names.c.in
	exe_name=`$(ECHO_NO_NEWLINE) '$(ui_exe_name)' | \
		$(HEXDUMP) -v -e '1/1 "%02x "'`'00' \
	exe_rel_path=`$(ECHO_NO_NEWLINE) '$(ui_exe_relative_path)' | \
		$(HEXDUMP) -v -e '1/1 "%02x "'`'00' :; \
	$(AWK) -v token_exec="$$exe_name" \
		-v token_exec_rel_path="$$exe_rel_path" \
		' \
		BEGIN { \
			token_exec = create_replacement_value(token_exec); \
			token_exec_rel_path = create_replacement_value(token_exec_rel_path); \
		} \
		{ \
			gsub(/!UI_TOKEN_GEN_EXEC!/, token_exec); \
			gsub(/!UI_TOKEN_GEN_EXEC_REL_PATH!/, token_exec_rel_path); \
			print $$0; \
		} \
		function create_replacement_value(src_value) { \
			result = ""; \
			chars_len = split(src_value, chars); \
			idx = 0; \
			line_idx = 0; \
			line_padding = "        "; \
			char_count_in_line = 12; \
			line_count = (chars_len + (char_count_in_line - 1)) / char_count_in_line; \
			line_count = int(line_count); \
			for (idx = 0; idx < chars_len; idx++) { \
				a_char = chars[idx + 1]; \
				if (line_idx == 0 && result) { \
					result = result line_padding; \
				} else if (result) { \
					result = result " "; \
				} \
				result = result "0x" a_char; \
				if (idx < chars_len - 1) { \
					result = result ","; \
				} \
				line_idx = (idx + 1) % char_count_in_line; \
				if (line_idx == 0) { \
					if (int(idx / char_count_in_line) < line_count - 1) { \
						result = result "\n"; \
					} \
				} \
			} \
			return result; \
		} \
		' \
		$< >$@

l10n_i.c : l10n_i.c.in
	sed -e 's/!LOCPATH_NAME!/$(LOCPATH_NAME)/' $< >$@

credhelper_const.h : credhelper_const.h.in $(srcdir)/Makefile.am
	awk $(awk_cmd_1) $< >$@

credhelper_res.o:credhelper_const.h

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

link-ui: credential-ocs/$(am__dirstamp)
	if  [ ! -h credential-ocs/$(ui_exe_subdir) ]; then \
		cd credential-ocs; \
		$(LN_S) ../$(ui_exe_dir) $(ui_exe_subdir) ; \
	fi


install-libexecPROGRAMS: $(libexec_PROGRAMS)
	@$(NORMAL_INSTALL)
	@for p in $(libexec_PROGRAMS); do \
		src=$$p \
		dir=`dirname $$p` \
		dst_item=`basename $$p | sed -e '$(transform)'`; \
		if [ "$$dir" = "." ]; then \
			dir=; \
		else \
			dir=/$$dir; \
		fi; \
		dst_dir=$(DESTDIR)$(libexecdir)$$dir \
		dst=$$dst_dir/$$dst_item; \
		echo " $(MKDIR_P) \"$$dst_dir\""; \
		$(MKDIR_P) "$$dst_dir"; \
	    echo " $(INSTALL_PROGRAM_ENV) $(LIBTOOL) $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=install $(INSTALL_PROGRAM) $$src $$dst"; \
		$(INSTALL_PROGRAM_ENV) $(LIBTOOL) $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=install $(INSTALL_PROGRAM) $$src $$dst_dir; \
		ins_res=$$?; \
		if [ $$ins_res -ne 0 ]; then \
			exit $$ins_res; \
		fi \
	done

install-exec-hook:
	@$(POST_INSTALL)
	$(MKDIR_P) $(DESTDIR)$(bindir)
	rm -f $(DESTDIR)$(bindir)/credential-ocs$(EXEEXT)
	pushd $(DESTDIR)/$(bindir); \
		$(LN_S_R) ../$(libexecdir)/credhelper$(EXEEXT) credential-ocs$(EXEEXT)
	if [ -z "$(SKIP_LOCPATH_INSTALL)" ]; then \
		echo $(DESTDIR)$(localedir) \
			>$(DESTDIR)$(libexecdir)/credential-ocs/$(LOCPATH_NAME) ; \
	else \
		: > $(DESTDIR)$(libexecdir)/credential-ocs/$(LOCPATH_NAME) ; \
	fi


preuninstall:
	@$(PRE_UNINSTALL)
	rm -f $(DESTDIR)$(libexecdir)/credential-ocs/$(LOCPATH_NAME)
	rm -f $(DESTDIR)$(bindir)/credential-ocs$(EXEEXT)

uninstall-libexecPROGRAMS:
	@$(NORMAL_UNINSTALL)
	for p in $(libexec_PROGRAMS); do \
		dir=`dirname $$p` \
		dst_item=`basename $$p | sed -e '$(transform)'`; \
		if [ "$$dir" = "." ]; then \
			dir=; \
		else \
			dir=/$$dir; \
		fi; \
		dst_dir=$(DESTDIR)$(libexecdir)$$dir \
		dst=$$dst_dir/$$dst_item; \
		rm -f $$dst; \
		rm -d $$dst_dir >/dev/null | :; \
	done

uninstall-am: preuninstall uninstall-libexecPROGRAMS

clean-local:
	rm -f *.rc	
	rm -f t_site_01.sh rt_ui_01.sh 
	rm -f rt_get_01.sh rt_get_02.sh
	rm -f mem_dbg_on.c mem_dbg_off.c
	rm -f exec_names.c
	rm -f l10n_i.c
	rm -f credhelper_const.h
	rm -f credhelper_res.rc
	rm -f client_secret.gpf
	rm -f client_secret.c
	rm -f client_id.gpf
	rm -f client_id.c
	if [ -z "$(CLEAN_FOR_DEBUG)" ]; then \
		rm -f mem_dbg.c; \
	fi
	rm -f client-id-dbg.txt
	rm -f client-secret-dbg.txt
	rm -f mem_trace_erasing
	rm -f mem_trace_writing
	if [ -x $(builddir)/credential-ocs/creddef ]; then \
		rm -f `$(AM_TESTS_ENVIRONMENT) $(builddir)/credential-ocs/creddef -d`; \
		rm -f -r \
			`$(AM_TESTS_ENVIRONMENT) $(builddir)/credential-ocs/creddef -r`; \
	fi
	if [ -h credential-ocs/$(ui_exe_subdir) ]; then \
		rm -f credential-ocs/$(ui_exe_subdir); \
	fi

.rc.o:
	WINDRES_CPP_CMD=`echo $(CPP) $(WINDRES_CPP_OPTION) | awk '\
		{ \
			cpp_cmd = $$1; \
			if (NF > 1) { \
				cpp_args = " "; \
				for (i = 2; i <= NF; i++) { \
					cpp_args = cpp_args	" --preprocessor-arg=" $$i; \
				} \
			} \
			print "--preprocessor=" cpp_cmd cpp_args; \
		}'`; \
		echo $$WINDRES_CPP_CMD; \
	$(LIBTOOL) $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --tag=RC --mode=compile \
		$(RC) $(RCFLAGS) \
			$$WINDRES_CPP_CMD \
			--preprocessor-arg=-D \
			--preprocessor-arg=RC_INVOKED \
			--preprocessor-arg=-I$(top_builddir)/include \
			--preprocessor-arg=-I$(builddir) \
			--preprocessor-arg=-I$(srcdir) \
			-i $< -o $@


.gpf.c:
	$(GPERF) --output-file=$@ $< 
# vi: se ts=4 sw=4 noet:
