
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


ELECTRON_TARGET_CPU=@ELECTRON_TARGET_CPU@
ELECTRON_TARGET_OS=@ELECTRON_TARGET_OS@


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
	for item in $(src_dirs) ; do \
		if [ ! -h $$item ] ; then \
			$(LN_S) $(srcdir)/$$item; \
		fi \
	done

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

electron-resources: electron-tmstamp electron-package-json\
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

build-app:
	$(NPM) run build-app

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

# vi: se ts=4 sw=4 noet:
