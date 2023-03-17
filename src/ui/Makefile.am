
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
	electron/src/option.ts \
	electron/src/desc.ts \
	electron/src/credential.ts \
	electron/src/ipc.ts \
	electron/src/preload.ts \
	electron/src/credential-ui.ts \
	electron/src/main.ts 

src_dirs = client electron

EXTRA_DIST = $(client_src) $(electron_src) \
	tsconfig.node.json.in \
	electron-package.json.in \
	tsconfig.electron.json.in \
	tsconfig.json.in \
	rollup.config.electron.js.in \
	config/app.json.in \
	config/config.json.in \
	package.json.in \
	vite.config.ts.in 

ELECTRON_TARGET_CPU=@ELECTRON_TARGET_CPU@
ELECTRON_TARGET_OS=@ELECTRON_TARGET_OS@
GUI_EXE_SUBDIR=@GUI_EXE_SUBDIR@
OC_EXE_ROOT_NAME=@OC_EXE_ROOT_NAME@
PKG_DEST_REL_DIR=$(OC_EXE_ROOT_NAME)/$(GUI_EXE_SUBDIR)

if TARGET_MACOS
electron_resource_dir=Electron.app/Conentents/Resources
else
electron_resource_dir=resources
endif

electron_version = v22.1.0
electron_dist_host = https://github.com
electron_dist_path_0 = electron/electron/releases/download
electron_zip = electron-$(electron_version)-$(ELECTRON_TARGET_OS)-$(ELECTRON_TARGET_CPU).zip

electron_dist_url = $(electron_dist_host)/$(electron_dist_path_0)/$(electron_version)/$(electron_zip)

electron_app_dir = pkg/$(electron_resource_dir)/app
electron_package_json = $(electron_app_dir)/package.json

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

tsconfig.electron.json: tsconfig.electron.json.in | symlink-to-core
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
	if [ "$(srcdir)" != "." ] ; then \
		for item in $(src_dirs) ; do \
			if [ ! -h $$item ] ; then \
				$(LN_S) $(srcdir)/$$item; \
			fi \
		done; \
	fi

mkdir-tree:
	for item in config pkg ; do \
		$(MKDIR_P) $$item; \
	done

symlink-to-core:
	if [ ! -h core ] ; then \
		$(LN_S) $(CORE_MODULE_DIR) core ; \
	fi

$(electron_zip): 
	if [ ! -f '$@' ] ; then \
		$(CURL) -L -o $@ '$(electron_dist_url)' ; \
	fi

electron-tmstamp: $(electron_zip) | mkdir-tree
	: > $@
	$(UNZIP) $^ -d pkg

electron-app-dir:
	if [ ! -d $(electron_app_dir) ]; then \
		$(MKDIR_P) $(electron_app_dir) ; \
	fi

electron-resources: electron-tmstamp electron-package-json build-app \
	| mkdir-tree electron-app-dir 
	cp -R dist $(electron_app_dir)
	
.PHONY: electron-resources


$(electron_package_json): electron-package.json.in | electron-app-dir
	$(AWK) -v package_name=$(PACKAGE_NAME) \
		-v package_version=$(PACKAGE_VERSION) \
		-e $(awk_cmd_0) $^ > $@

electron-package-json: $(electron_package_json)

.PHONY: electron-package-json

electron-dbg:
	echo $(electron_package_json)

.PHONY: electron-dbg

download_electron: | $(electron_zip)

.PHONY: download_electron

dist/index.html : mksrc-tree $(client_src) \
	package-lock.json vite.config.ts 
	$(NPM) run build-client

dist/main.js: mksrc-tree $(electron_src) \
	package-lock.json tsconfig.electron.json rollup.config.electron.js
	$(NPM) run build-electron

build-app: package-lock.json dist/index.html dist/main.js

.PHONY: build-app

config/app.json: config/app.json.in | mkdir-tree
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
	config/app.json \
	| mksrc-tree mkdir-tree build-app electron-resources

clean-local:
	rm -f *.json
	rm -f *.ts
	rm -r -f pkg
	rm -f electron-tmstamp
	for item in $(src_dirs) ; do \
		if [ -h $$item ] ; then \
			rm $$item; \
		fi \
	done

install-exec-local: 
	@$(NORMAL_INSTALL)
	insdir='$(DESTDIR)$(libexecdir)/$(PKG_DEST_REL_DIR)'; \
	for f in `find pkg -type f`; do \
		relpath=`echo $$f | sed -e 's/^pkg\///'` \
		child=`basename $$relpath` \
		dstreld=`echo $$relpath | sed -e "s/$${child}$$//"`; \
		if [ ! -d $$insdir/$$dstreld ]; then \
			$(MKDIR_P) $$insdir/$$dstreld; \
		fi; \
		cp $$f $$insdir/$$relpath; \
	done


uninstall-local:
	@$(NORMAL_UNINSTALL)
	insdir='$(DESTDIR)$(libexecdir)/$(PKG_DEST_REL_DIR)'; \
	rm -rf $$insdir


# vi: se ts=4 sw=4 noet:
