
bin_SCRIPTS = credential-oc.sh
dist_bin_SCRIPTS=credential-oc.sh

CLEANFILES = credential-oc.sh

CLIENT_ID_TXT=$(top_srcdir)/src/config/client-id.txt
CLIENT_SECRET_TXT=$(top_srcdir)/src/config/client-secret.txt

pkgdatadir=$(datadir)/$(PACKAGE_NAME)
bindir=$(datadir)/$(PACKAGE_NAME)/bin

textdomain_dir=$(pkgdatadir)/locale


credential-oc.sh : credential-oc.sh.in $(CLIENT_ID_TXT) $(CLIENT_SECRET_TXT)
	awk 'BEGIN { \
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
	' $< >$@; \
	chmod +x $@

# vi: se ts=4 sw=4 noet:
