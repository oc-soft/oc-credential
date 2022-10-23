
bin_PROGRAMS = lmdcli

lmdcli_src = main.c \
	lmd.c \
	lmd_parser.c \
	client_secret.c \
	client_id.c \
	lmd_connections.c \
	lmd_requests.c \
	lmd_i.c 

if RUN_WIN
lmdcli_SOURCES = $(lmdcli_src) \
	lmd_res.rc
else
lmdcli_SOURCES = $(lmdcli_src) 
endif

AM_FLAGS= -g -O0
lmdcli_LDFLAGS= -g
lmdcli_LDADD= $(top_builddir)/oclib/buffer/src/libocbuffer.la \
	-lintl -ljson-c -lcurl -lm

CLIENT_ID_TXT=$(top_srcdir)/src/config/client-id.txt
CLIENT_SECRET_TXT=$(top_srcdir)/src/config/client-secret.txt

pkgdatadir=$(datadir)/$(PACKAGE_NAME)

textdomain_dir=$(pkgdatadir)/locale

awk_cmd_0='BEGIN { \
			getline client_id <"$(CLIENT_ID_TXT)"; \
			getline client_secret <"$(CLIENT_SECRET_TXT)"; \
			textdomain_dir = "$(textdomain_dir)" \
		} \
		{ \
			gsub(/@CLIENT_ID@/, client_id); \
			gsub(/@CLIENT_SECRET@/, client_secret); \
			gsub(/@TEXTDOMAIN_DIR@/, textdomain_dir); \
			print \
		} \
	' 
awk_cmd_1 = '{ \
			gsub(/@LMTD_APP_FILE_NAME@/, \
				"\"lmdcli\""); \
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

lmdcli_CPPFLAGS=-I$(top_srcdir)/oclib/buffer/include


client_secret.c : client_secret.in.c
	awk $(awk_cmd_0) $< >$@


client_id.c : client_id.in.c
	awk $(awk_cmd_0) $< >$@

lmd_const.h : lmd_const.h.in
	awk $(awk_cmd_1) $< >$@

lmd_res.rc : lmd_res.rc.in lmd_const.h
	awk $(awk_cmd_2) $(top_srcdir)/win/lang_map.txt > $@

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

# vi: se ts=4 sw=4 noet:
