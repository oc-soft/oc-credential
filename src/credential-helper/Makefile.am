
libexec_PROGRAMS = credential-ocs/credhelper
check_PROGRAMS = credential-ocs/t01_token_gen_ui_exec \
	t01_l10n
noinst_PROGRAMS = credential-ocs/creddef

credhelper_hdr = l10n_i.h \
	url_id.h \
	logging.h \
	lmd_requests_i.h \
	dbg.h \
	mem_dbg.h \
	exe_info.h \
	client_id.h \
	lmd_ui_reader.h \
	lmd_types.h \
	token_gen_ui_i.h \
	service_oauth_response_error_parser.h \
	service_id.h \
	service_oauth_token_parser.h \
	lmd_google_connections.h \
	proc_types.h \
	time_zone.h \
	lmd_oauth_token_param_creator.h \
	lmd_ui.h \
	service_secret.h \
	lmd_op_i.h \
	url_path_iterate.h \
	ui_token_gen.h \
	cred_helper_parser.h \
	token_gen_ui_exec.h \
	ui_token_gen_i.h \
	lmd_requests_std.h \
	l10n.h \
	lmd_github_connections.h \
	service_url_lmd_oauth_token_param.h \
	lmd_connections.h \
	lmd_oauth_mail.h \
	url_services.h \
	lmd_requests_google.h \
	cred_helper_i.h \
	path.h \
	lmd.h \
	lmd_oauth_mail_github.h \
	credential_storage_key_id_table.h \
	url.h \
	client_secret.h \
	lmd_parser.h \
	lmd_requests.h \
	service_url_device_user_code_param.h \
	option.h \
	fd_io_win.h \
	service_url_secret.h \
	fd_io.h \
	credential_op.h \
	cred_helper_op.h \
	proc_sched.h \
	lmd_op.h \
	lmd_i.h \
	cred_helper_types.h \
	token_gen_ui.h \
	cred_helper.h \
	proc.h \
	ui_token_gen_types.h \
	http.h \
	exec_names.h \
	lmd_ms_connections.h \
	service_url_client.h

credhelper_src = main.c \
	cred_helper.c \
	cred_helper_i.c \
	cred_helper_op.c \
	cred_helper_parser.c \
	dbg.c \
	lmd.c \
	lmd_parser.c \
	lmd_connections.c \
	lmd_github_connections.c \
	lmd_google_connections.c \
	lmd_ms_connections.c \
	lmd_requests.c \
	lmd_op.c \
	lmd_oauth_mail.c \
	lmd_oauth_mail_github.c \
	lmd_i.c \
	lmd_ui.c \
	lmd_ui_reader.c \
	lmd_oauth_token_param_creator.c \
	l10n_i.c \
	token_gen_ui.c \
	token_gen_ui_exec.c \
	token_gen_ui_i.c \
	ui_token_gen.c \
	ui_token_gen_i.c \
	exec_names.c \
	url_path_iterate.c \
	url_services.c \
	url_id.c \
	service_id.c \
	service_secret.c \
	service_url_client.c \
	service_url_secret.c \
	service_url_device_user_code_param.c \
	service_url_lmd_oauth_token_param.c \
	service_oauth_token_parser.c \
	service_oauth_response_error_parser.c \
	mem_dbg.c

EXTRA_DIST = \
	key_values_replace.awk \
	gpf_names_replace.awk \
	create-git-credential-input.awk \
	url_services_gen.awk \
	lmd_connections.gpf \
	lmd_oauth_mail.gpf \
	service_oauth_token_parser.gpf \
	service_oauth_response_error_parser.gpf \
	service_id.gpf.in \
	service_secret.gpf.in \
	service_url_secret.gpf.in \
	service_url_client.gpf.in \
	service_url_device_user_code_param.gpf.in \
	service_url_lmd_oauth_token_param.gpf.in \
	url_services.gpf.in \
	url_id.gpf.in \
	rt_ui_01.sh.in \
	mem_dbg_on.c.in \
	lmd_oauth_mail_github.c.in \
	rt_get_01.sh.in \
	l10n_i.c.in \
	logging_win.h.in \
	credhelper_const.h.in \
	mem_dbg_off.c.in \
	rt_get_02.sh.in \
	rt_get_03.sh.in \
	exec_names.c.in \
	t_store_01.sh \
	t_erase_01.sh \
	t_erase_02.sh \
	t_l10n_01.sh




if !TARGET_WASM
credential_ocs_credhelper_SOURCES = $(credhelper_src) $(credhelper_hdr)
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
credential_ocs_credhelper_SOURCES += proc_sched_win.c
credential_ocs_credhelper_SOURCES += lmd_ui_win.c
endif
if RUN_LINUX
credential_ocs_credhelper_SOURCES += exe_info_linux.c
credential_ocs_credhelper_SOURCES += l10n_user_posix.c
credential_ocs_credhelper_SOURCES += fd_io.c
credential_ocs_credhelper_SOURCES += token_gen_ui_i_posix.c
credential_ocs_credhelper_SOURCES += path.c
credential_ocs_credhelper_SOURCES += logging.c
credential_ocs_credhelper_SOURCES += time_zone.c
credential_ocs_credhelper_SOURCES += proc_sched_posix.c
credential_ocs_credhelper_SOURCES += lmd_ui_posix.c
endif
if RUN_DARWIN
credential_ocs_credhelper_SOURCES += exe_info_darwin.c
credential_ocs_credhelper_SOURCES += l10n_user_posix.c
credential_ocs_credhelper_SOURCES += fd_io.c
credential_ocs_credhelper_SOURCES += token_gen_ui_i_posix.c
credential_ocs_credhelper_SOURCES += path.c
credential_ocs_credhelper_SOURCES += logging.c
credential_ocs_credhelper_SOURCES += time_zone.c
credential_ocs_credhelper_SOURCES += proc_sched_posix.c
credential_ocs_credhelper_SOURCES += lmd_ui_posix.c
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
credential_ocs_t01_token_gen_ui_exec_SOURCES += logging_win.c
endif
if RUN_DARWIN
credential_ocs_t01_token_gen_ui_exec_SOURCES += path.c exe_info_darwin.c
credential_ocs_t01_token_gen_ui_exec_SOURCES += logging.c 
else
credential_ocs_t01_token_gen_ui_exec_SOURCES += path.c exe_info_linux.c
credential_ocs_t01_token_gen_ui_exec_SOURCES += logging.c 
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
	$(top_builddir)/oclib/col/src/liboccol.la \
	$(top_builddir)/src/core/liboccredcore.la \
	$(credhelper_lib_flags)

credential_ocs_creddef_LDADD=$(top_builddir)/src/core/liboccredcore.la


if RUN_WIN
credential_ocs_credhelper_LDFLAGS += -Wl,--dependency-file -Wl,credhhelper-dpd.txt -specs=$(srcdir)/ucrt.specs
endif

SH_LOG_DRIVER = env AM_TAP_AWK='$(AWK)' $(SHELL) $(top_srcdir)/tap-driver.sh
TEST_EXTENSIONS = .sh
SH_LOG_COMPILER = $(SHELL)
TESTS = t_store_01.sh t_erase_01.sh t_erase_02.sh t_l10n_01.sh
AM_TESTS_ENVIRONMENT = HOME=$(builddir);
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
	-I$(top_srcdir)/oclib/col/include \
	-I$(top_srcdir)/src/core

credential_ocs_creddef_CPPFLAGS= -I$(top_srcdir)/src/core



mem_dbg.c : mem_dbg_off.c.in
	cp $< $@

mem_dbg: mem_dbg_on.c.in
	cp $< mem_dbg.c

.PHONY: mem_dbg


all-local: rt_get_01.sh rt_get_02.sh rt_ui_01.sh rt_cli_get_01.sh rt_cli_get_02.sh rt_cli_get_03.sh


rt_get_01.sh : rt_get_01.sh.in Makefile credential-ocs/credhelper$(EXTEXT)
	sed -e 's!%URL_ID_PATH%!$(abs_top_srcdir)/src/config/url-id.txt!' \
		-e 's!%CLI%!$(abs_builddir)/credential-ocs/credhelper$(EXEEXT)!' \
		-e 's!%AWK%!$(AWK)!' $< >$@
	chmod u+x $@

rt_cli_get_01.sh : rt_get_01.sh.in Makefile credential-ocs/credhelper$(EXTEXT)
	sed -e 's!%URL_ID_PATH%!$(abs_top_srcdir)/src/config/url-id.txt!' \
		-e 's!%CLI%!$(abs_builddir)/credential-ocs/credhelper$(EXEEXT) -e 10 -j off -g on!' \
		-e 's!%AWK%!$(AWK)!' $< >$@
	chmod u+x $@

rt_get_02.sh : rt_get_02.sh.in Makefile credential-ocs/credhelper$(EXTEXT)
	sed -e 's!%CLI%!$(abs_builddir)/credential-ocs/credhelper$(EXEEXT)!' \
		-e 's!%URL_ID_PATH%!$(abs_top_srcdir)/src/config/url-id.txt!' \
		-e 's!%AWK%!$(AWK)!' $< >$@
	chmod u+x $@

rt_cli_get_02.sh : rt_get_02.sh.in Makefile credential-ocs/credhelper$(EXTEXT)
	sed -e 's!%CLI%!$(abs_builddir)/credential-ocs/credhelper$(EXEEXT) -e 10 -j off -g on!' \
		-e 's!%URL_ID_PATH%!$(abs_top_srcdir)/src/config/url-id.txt!' \
		-e 's!%AWK%!$(AWK)!' $< >$@
	chmod u+x $@

rt_cli_get_03.sh : rt_get_03.sh.in Makefile credential-ocs/credhelper$(EXTEXT)
	sed -e 's!%CLI%!$(abs_builddir)/credential-ocs/credhelper$(EXEEXT) -e 10 -j off -g on!' \
		-e 's!%URL_ID_PATH%!$(abs_top_srcdir)/src/config/url-id.txt!' \
		-e 's!%AWK%!$(AWK)!' $< >$@
	chmod u+x $@
	

rt_ui_01.sh : rt_ui_01.sh.in | link-ui
	sed -e 's!%UI_EXE_RELPATH%!credential-ocs/$(ui_exe_relative_path)!' $< >$@
	chmod u+x $@

git-credential-data-1.txt: $(srcdir)/../config/url-id.txt \
	$(srcdir)/create-git-credential-input.awk
	$(AWK) -f $(srcdir)/create-git-credential-input.awk <$< >$@


t_erase_01.log: t_store_01.log

t_erase_02.log: t_erase_01.log


url_id.gpf : url_id.gpf.in \
	$(srcdir)/../config/url-id.txt \
	$(srcdir)/key_values_replace.awk
	awk -f $(srcdir)/key_values_replace.awk -- \
		-i $(srcdir)/../config/url-id.txt -q 0,1 -d 8 $< >$@

service_id.gpf : service_id.gpf.in \
	$(srcdir)/../config/service-id.txt \
	$(srcdir)/key_values_replace.awk
	awk -f $(srcdir)/key_values_replace.awk -- \
		-i $(srcdir)/../config/service-id.txt -q 0,1 -d 8 $< >$@


url_services.gpf : url_services.gpf.in \
	$(srcdir)/../config/url-service-lmd.txt \
	$(srcdir)/url_services_gen.awk
	awk -f $(srcdir)/url_services_gen.awk -- \
		-i $(srcdir)/../config/url-service-lmd.txt $< >$@

service_secret.gpf : service_secret.gpf.in \
	$(srcdir)/../config/service-secret.txt \
	$(srcdir)/gpf_names_replace.awk
	awk -f $(srcdir)/gpf_names_replace.awk -- \
		-n $(srcdir)/../config/service-secret.txt $< >$@

service_url_secret.gpf : service_url_secret.gpf.in \
	$(srcdir)/../config/service-url-secret-lmd.txt \
	$(srcdir)/key_values_replace.awk
	awk -f $(srcdir)/key_values_replace.awk -- \
		-i $(srcdir)/../config/service-url-secret-lmd.txt -q 0,1 -d 8 $< >$@

service_url_client.gpf : service_url_client.gpf.in \
	$(srcdir)/../config/service-url-client-lmd.txt \
	$(srcdir)/key_values_replace.awk
	awk -f $(srcdir)/key_values_replace.awk -- \
		-i $(srcdir)/../config/service-url-client-lmd.txt -q 0,1 -d 8 $< >$@

service_url_device_user_code_param.gpf : \
	service_url_device_user_code_param.gpf.in \
	$(srcdir)/../config/service-url-device-user-code-param.txt \
	$(srcdir)/key_values_replace.awk
	awk -f $(srcdir)/key_values_replace.awk -- \
		-i $(srcdir)/../config/service-url-device-user-code-param.txt \
		-q 0,1,0 -a 0,1,0 -d 8 $< >$@

service_url_lmd_oauth_token_param.gpf : \
	service_url_lmd_oauth_token_param.gpf.in \
	$(srcdir)/../config/service-url-lmd-oauth-token-param.txt \
	$(srcdir)/key_values_replace.awk
	awk -f $(srcdir)/key_values_replace.awk -- \
		-i $(srcdir)/../config/service-url-lmd-oauth-token-param.txt \
		-q 0,1,0,0,0 -a 0,1,0,0,0 -d 8 $< >$@

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

lmd_oauth_mail_github.c : lmd_oauth_mail_github.c.in\
	$(srcdir)/../config/github-mail-bind-url.txt
	read ln <$(srcdir)/../config/github-mail-bind-url.txt; \
		sed -e s%@GITHUB_BIND_URL@%$$ln% $< >$@

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
		$(LN_S_R) $(DESTDIR)/$(libexecdir)/credential-ocs/credhelper$(EXEEXT) credential-ocs$(EXEEXT)
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
	rm -f *.gpf
	rm -f git-credential-data-1.txt
	rm -f rt_ui_01.sh  rt_cli_get_01.sh
	rm -f rt_get_01.sh rt_get_02.sh  rt_cli_get_02.sh
	rm -f rt_cli_get_03.sh 
	rm -f exec_names.c
	rm -f url_id.c
	rm -f url_services.c
	rm -f service_id.c
	rm -f service_secret.c
	rm -f service_url_secret.c
	rm -f service_url_client.c
	rm -f service_url_lmd_oauth_token_param.c
	rm -f service_url_device_user_code_param.c
	rm -f service_url_oauth_token_param.c
	rm -f service_oauth_response_error_parser.c
	rm -f service_oauth_token_parser.c
	rm -f lmd_oauth_mail.c
	rm -f lmd_oauth_mail_github.c 
	rm -f l10n_i.c
	rm -f logging_win.h
	rm -f credhelper_const.h
	rm -f credhelper_res.rc
	if [ -z "$(CLEAN_FOR_DEBUG)" ]; then \
		rm -f mem_dbg.c; \
	fi
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
	$(LIBTOOL) $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --tag=RC --mode=compile \
		$(WINDRES) \
		$(RCFLAGS) \
			-I $(top_builddir)/include \
			-I $(builddir) \
			-I $(srcdir) \
			-i $< -o $@


.gpf.c:
	$(GPERF) --output-file=$@ $< 
# vi: se ts=4 sw=4 noet:
