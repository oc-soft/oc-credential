
CORE_MODULE_DIR=@CORE_MODULE_DIR@

client_src = \
	client/index.html \
	client/src/App.vue \
	client/src/components/Message.vue \
	client/src/components/Oauth.vue \
	client/src/io/oauth.ts \
	client/src/io/index.ts \
	client/src/main.ts

electron_src = \
	electron/src/ipc.ts
	electron/src/io/oauth.ts
	electron/src/preload.ts
	electron/src/main.ts

src_dirs = client electron

awk_cmd_0=' \
	{ \
		gsub("!PACKAGE_NAME!", package_name); \
		gsub("!PACKAGE_VERSION!", package_version); \
		gsub("!SRCDIR!", srcdir); \
		gsub("!ABS_BUILDDIR!", abs_builddir); \
		gsub("!ABS_SRCDIR!", abs_srcdir); \
		print $$0; \
	} \
'

package-lock.json: package.json
	$(NPM) install

package.json: package.json.in
	$(AWK) -v srcdir=$(srcdir) \
		-v abs_builddir=$(abs_builddir) \
		-v abs_srcdir=$(abs_srcdir) \
		-v package_name=$(PACKAGE_NAME) \
		-v package_version=$(PACKAGE_VERSION) \
		-e $(awk_cmd_0) $< >$@

rollup.config.electron.js: rollup.config.electron.js.in \
	tsconfig.electron.json
	$(AWK) -v srcdir=$(srcdir) \
		-v abs_builddir=$(abs_builddir) \
		-v abs_srcdir=$(abs_srcdir) \
		-v package_name=$(PACKAGE_NAME) \
		-v package_version=$(PACKAGE_VERSION) \
		-e $(awk_cmd_0) $< >$@

tsconfig.electron.json: tsconfig.electron.json.in symlink-to-core
	$(AWK) -v srcdir=$(srcdir) \
		-v abs_builddir=$(abs_builddir) \
		-v abs_srcdir=$(abs_srcdir) \
		-v package_name=$(PACKAGE_NAME) \
		-v package_version=$(PACKAGE_VERSION) \
		-e $(awk_cmd_0) $< >$@


tsconfig.json: tsconfig.json.in
	$(AWK) -v srcdir=$(srcdir) \
		-v abs_builddir=$(abs_builddir) \
		-v abs_srcdir=$(abs_srcdir) \
		-v package_name=$(PACKAGE_NAME) \
		-v package_version=$(PACKAGE_VERSION) \
		-e $(awk_cmd_0) $< >$@

tsconfig.node.json: tsconfig.node.json.in
	$(AWK) -v srcdir=$(srcdir) \
		-v abs_builddir=$(abs_builddir) \
		-v abs_srcdir=$(abs_srcdir) \
		-v package_name=$(PACKAGE_NAME) \
		-v package_version=$(PACKAGE_VERSION) \
		-e $(awk_cmd_0) $< >$@

vite.config.ts: vite.config.ts.in
	$(AWK) -v srcdir=$(srcdir) \
		-v abs_builddir=$(abs_builddir) \
		-v abs_srcdir=$(abs_srcdir) \
		-v package_name=$(PACKAGE_NAME) \
		-v package_version=$(PACKAGE_VERSION) \
		-e $(awk_cmd_0) $< >$@

mksrc-tree:
	for item in $(src_dirs) ; do \
		if [ ! -h $$item ] ; then \
			$(LN_S) $(srcdir)/$$item; \
		fi \
	done

.PHONY: mksrc-tree

mkdir-tree:
	for item in config ; do \
		$(MKDIR_P) $$item; \
	done


.PHONY: mkdir-tree

symlink-to-core:
	if [ ! -h core ] ; then \
		$(LN_S) $(CORE_MODULE_DIR) core ; \
	fi
.PHONY: symlink-to-core

config/app.json: config/app.json.in mkdir-tree
	$(AWK) -v name=credential-helper-ui-oc \
		-v version=$(PACKAGE_VERSION) \
		-e ' { \
			gsub(/!NAME!/, name); \
			gsub(/!VERSION!/, version); \
			print $$0; \
		}' $< >$@	


all-local: package-lock.json \
	package.json \
	rollup.config.electron.js \
	tsconfig.json \
	tsconfig.node.json \
	vite.config.ts \
	mksrc-tree \
	config/app.json
	$(NPM) run build-app

clean-local:
	rm -f *.json
	rm -f *.ts
	for item in $(src_dirs) ; do \
		if [ -h $$item ] ; then \
			rm $$item; \
		fi \
	done

# vi: se ts=4 sw=4 noet:
