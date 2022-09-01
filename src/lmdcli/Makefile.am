
bin_PROGRAMS = lmdcli


lmdcli_SOURCES = main.c \
	lmd.c \
	lmd_parser.c \
	client_secret.c \
	client_id.c \
	lmd_connections.c \
	lmd_requests.c \
	lmd_i.c 

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

lmdcli_CPPFLAGS=-I$(top_srcdir)/oclib/buffer/include


client_secret.c : client_secret.in.c
	awk $(awk_cmd_0) $< >$@


client_id.c : client_id.in.c
	awk $(awk_cmd_0) $< >$@




# vi: se ts=4 sw=4 noet:
