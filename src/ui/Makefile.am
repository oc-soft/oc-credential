
CORE_MODULE_DIR=@CORE_MODULE_DIR@

client_src = \
	client/src/option.ts \
	client/src/App.vue \
	client/src/components/Message.vue \
	client/src/components/Oauth.vue \
	client/src/io/oauth.ts \
	client/src/io/codec.ts \
	client/src/io/index.ts \
	client/src/main.ts \
	client/index.html 

electron_src = \
	electron/src/option.ts \
	electron/src/desc.ts \
	electron/src/credential.ts \
	electron/src/ipc.ts \
	electron/src/preload.ts \
	electron/src/credential-ui.ts \
	electron/src/main.ts 

src_dirs = client electron common


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
PRODUCT_NAME=@PRODUCT_NAME@

TOKEN_GENERATOR_NAME=@GUI_TOKEN_GENERATOR_NAME@
TOKEN_GENERATOR_ID=@GUI_TOKEN_GENERATOR_ID@
TOKEN_GENERATOR_HELPER_ID=@GUI_TOKEN_GENERATOR_HELPER_ID@
TOKEN_GENERATOR_FILE_DESC=@GUI_TOKEN_GENERATOR_FILE_DESC@
WINDRES_CPP_OPTION=@WINDRES_CPP_OPTION@

TOKEN_GENERATOR_HELPER_NAME=$(TOKEN_GENERATOR_NAME) Helper
TOKEN_GENERATOR_HELPER_GPU_NAME=$(TOKEN_GENERATOR_NAME) Helper (GPU)
TOKEN_GENERATOR_HELPER_PLUGIN_NAME=$(TOKEN_GENERATOR_NAME) Helper (Plugin)
TOKEN_GENERATOR_HELPER_RENDERER_NAME=$(TOKEN_GENERATOR_NAME) Helper (RENDERER)

if RUN_DARWIN
TOKEN_GENERATOR_ICON=$(top_builddir)/resource/token-generator.icns
else
TOKEN_GENERATOR_ICON=
endif

AWK = @AWK@

if TARGET_MACOS
electron_resource_dir = Electron.app/Contents/Resources
token_generator_resource_dir = $(TOKEN_GENERATOR_NAME).app/Contents/Resources
else
electron_resource_dir = resources
token_generator_resource_dir = resources
endif

CLEAN_FOR_DEBUG =

electron_version = v22.1.0
electron_dist_host = https://github.com
electron_dist_path_0 = electron/electron/releases/download
electron_zip = electron-$(electron_version)-$(ELECTRON_TARGET_OS)-$(ELECTRON_TARGET_CPU).zip

electron_dist_url = $(electron_dist_host)/$(electron_dist_path_0)/$(electron_version)/$(electron_zip)

electron_extract_dir = pkg

electron_app_dir = $(electron_extract_dir)/$(electron_resource_dir)/app
token_generator_app_dir = $(electron_extract_dir)/$(token_generator_resource_dir)/app
electron_package_json = $(electron_app_dir)/package.json
token_generator_package_json = $(token_generator_app_dir)/package.json

awk_cmd_0=' \
	{ \
		gsub(/!PACKAGE_NAME!/, package_name); \
		gsub(/!PACKAGE_VERSION!/, package_version); \
		gsub(/!SRCDIR!/, srcdir); \
		gsub(/!ABS_BUILDDIR!/, abs_builddir); \
		gsub(/!ABS_SRCDIR!/, abs_srcdir); \
		print; \
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
		$(awk_cmd_0) $< >$@

rollup.config.electron.js: rollup.config.electron.js.in \
	tsconfig.electron.json
	$(AWK) -v srcdir=$(srcdir) \
		-v abs_builddir=$(abs_builddir) \
		-v abs_srcdir=$(abs_srcdir) \
		-v package_name=$(PACKAGE_NAME) \
		-v package_version=$(PACKAGE_VERSION) \
		$(awk_cmd_0) $< >$@

tsconfig.electron.json: tsconfig.electron.json.in 
	$(AWK) -v srcdir=$(srcdir) \
		-v abs_builddir=$(abs_builddir) \
		-v abs_srcdir=$(abs_srcdir) \
		-v package_name=$(PACKAGE_NAME) \
		-v package_version=$(PACKAGE_VERSION) \
		$(awk_cmd_0) $< >$@


tsconfig.json: tsconfig.json.in
	$(AWK) -v srcdir=$(srcdir) \
		-v abs_builddir=$(abs_builddir) \
		-v abs_srcdir=$(abs_srcdir) \
		-v package_name=$(PACKAGE_NAME) \
		-v package_version=$(PACKAGE_VERSION) \
		$(awk_cmd_0) $< >$@

tsconfig.node.json: tsconfig.node.json.in
	$(AWK) -v srcdir=$(srcdir) \
		-v abs_builddir=$(abs_builddir) \
		-v abs_srcdir=$(abs_srcdir) \
		-v package_name=$(PACKAGE_NAME) \
		-v package_version=$(PACKAGE_VERSION) \
		$(awk_cmd_0) $< >$@

vite.config.ts: vite.config.ts.in
	$(AWK) -v srcdir=$(srcdir) \
		-v abs_builddir=$(abs_builddir) \
		-v abs_srcdir=$(abs_srcdir) \
		-v package_name=$(PACKAGE_NAME) \
		-v package_version=$(PACKAGE_VERSION) \
		$(awk_cmd_0) $< >$@


ui_const.h: ui_const.h.in
	$(AWK) ' \
	{ \
		gsub(/!TOKEN_GEN_FILE_NAME!/, \
			"\"$(TOKEN_GENERATOR_NAME)$(EXEEXT)\""); \
		gsub(/!TOKEN_GEN_PRODUCT_NAME!/, \
			"\"$(PRODUCT_NAME)\""); \
		gsub(/!TOKEN_GEN_FILE_DESC!/, \
			"\"$(TOKEN_GENERATOR_FILE_DESC)\""); \
		print; \
	} \
' $< >$@





mksrc-tree:
if !COPY_JS_SRC_FOR_BUILD
	if [ "$(srcdir)" != "." ] ; then \
		for item in $(src_dirs) ; do \
			if [ ! -h $$item ] ; then \
				$(LN_S) $(srcdir)/$$item; \
			fi \
		done; \
	fi
else
	if [ "$(srcdir)" != "." ] ; then \
		for item in $(src_dirs) ; do \
			cp -r $(srcdir)/$$item $(builddir); \
		done; \
	fi
endif

mkdir-tree:
	for item in config $(electron_extract_dir) ; do \
		$(MKDIR_P) $$item; \
	done

$(electron_zip): 
	if [ ! -f '$@' ] ; then \
		$(CURL) -L -o $@ '$(electron_dist_url)' ; \
	fi


electron-tmstamp: $(electron_zip) | mkdir-tree
	$(UNZIP) $^ -d $(electron_extract_dir)
if RUN_WIN
	for f in `find $(electron_extract_dir) -type f`; do \
		chmod og-w $$f; \
		ft=`file -b $$f | grep '^PE32'`; \
		if [ -n "$$ft" ]; then \
			chmod +x $$f; \
		fi \
	done
endif
	: > $@

electron-app-dir:
	if [ ! -d $(token_generator_app_dir) ] \
		&& [ ! -d $(electron_app_dir) ]; then \
		$(MKDIR_P) $(electron_app_dir) ; \
	fi


electron-resources: electron-tmstamp electron-package-json build-app \
	| mkdir-tree electron-app-dir 
	cp -R dist $(token_generator_app_dir) 

.PHONY: electron-resources

if !RUN_WIN
rebrand-resource:
else
rebrand-resource: ui_const.h
endif	

rebrand-electron-contents-tmstamp: electron-resources rebrand-resource
if RUN_DARWIN
	if [ -d $(builddir)/$(electron_extract_dir)/Electron.app ]; then \
		branding_dir=$(builddir)/$(electron_extract_dir)/Electron.app; \
	else \
		branding_dir=$(builddir)/$(electron_extract_dir)/$(TOKEN_GENERATOR_NAME).app; \
	fi; \
	$(srcdir)/rebranding.rb --rebrand \
		-s $$branding_dir \
		-n '$(TOKEN_GENERATOR_NAME)' \
		-e '$(TOKEN_GENERATOR_NAME)' \
		-l '$(TOKEN_GENERATOR_HELPER_NAME)' \
		-m '$(TOKEN_GENERATOR_HELPER_GPU_NAME)' \
		-o '$(TOKEN_GENERATOR_HELPER_PLUGIN_NAME)' \
		-p '$(TOKEN_GENERATOR_HELPER_RENDERER_NAME)' \
		-d '$(TOKEN_GENERATOR_ID)' \
		-f '$(TOKEN_GENERATOR_HELPER_ID)' \
		-i '$(TOKEN_GENERATOR_ICON)'
endif
if RUN_WIN
	REBRAND_CPP_OPTION=`echo $(CPP) $(WINDRES_CPP_OPTION) | awk '\
		{ \
			cpp_cmd = $$1; \
			if (NF > 1) { \
				cpp_args = " "; \
				for (i = 2; i <= NF; i++) { \
					cpp_args = cpp_args	" -e " $$i; \
				} \
			} \
			print "-p " cpp_cmd cpp_args; \
		}'`; \
	$(srcdir)/rebranding-win.pl \
		-s $(builddir)/$(electron_extract_dir)/electron.exe \
		-d $(builddir)/$(electron_extract_dir)/$(TOKEN_GENERATOR_NAME)$(EXEEXT) \
		-r $(srcdir)/rebranding-win.rc \
		$$REBRAND_CPP_OPTION -e -D -e RC_INVOKED \
		-e -I$(top_builddir)/include \
		-e -I$(builddir) \
	  	-i $(top_srcdir)/resource/win-token-generator-16x16.png \
		-i $(top_srcdir)/resource/win-token-generator-24x24.png \
		-i $(top_srcdir)/resource/win-token-generator-32x32.png \
		-i $(top_srcdir)/resource/win-token-generator-48x48.png \
		-i $(top_srcdir)/resource/win-token-generator-256x256.png \
		-c 16,24,32,48,256
endif
	: >$@

	
rebrand-electron-contents: rebrand-electron-contents-tmstamp

.PHONY: rebrand-electron-contents

rebrand-electron-tmstamp: rebrand-electron-contents
if RUN_DARWIN
	if [ -d $(electron_extract_dir)/Electron.app ]; then \
		mv $(electron_extract_dir)/Electron.app \
			$(electron_extract_dir)/$(TOKEN_GENERATOR_NAME).app; \
	fi
endif
if RUN_WIN
	if [ -f $(builddir)/$(electron_extract_dir)/$(TOKEN_GENERATOR_NAME)$(EXEEXT) ]; then \
   		if [ -f $(builddir)/$(electron_extract_dir)/electron.exe ] ; then \
			rm $(builddir)/$(electron_extract_dir)/electron.exe; \
		fi \
	fi
endif
if RUN_LINUX
	if [ ! -f $(builddir)/$(electron_extract_dir)/$(TOKEN_GENERATOR_NAME)$(EXEEXT) ]; then \
		mv $(builddir)/$(electron_extract_dir)/electron$(EXEEXT) \
			$(builddir)/$(electron_extract_dir)/$(TOKEN_GENERATOR_NAME)$(EXEEXT); \
	fi
endif
	: > $@

rebrand-electron: rebrand-electron-tmstamp

.PHONY: rebrand-electron

electron-package-json-tmstamp: electron-package.json.in | electron-app-dir
	if [ -d $(builddir)/$(electron_extract_dir)/Electron.app ]; then \
		dest_path=$(electron_package_json); \
	else \
		dest_path=$(token_generator_package_json); \
	fi; \
	$(AWK) -v package_name=$(PACKAGE_NAME) \
		-v package_version=$(PACKAGE_VERSION) \
		$(awk_cmd_0) $< >$$dest_path
	: >$@


electron-package-json: electron-package-json-tmstamp

.PHONY: electron-package-json


download-electron: | $(electron_zip)

.PHONY: download-electron


dist/index.html : mksrc-tree $(client_src) \
	package-lock.json vite.config.ts 
	$(NPM) run build-client

dist/main.js: mksrc-tree $(electron_src) \
	package-lock.json tsconfig.electron.json rollup.config.electron.js
	$(NPM) run build-electron

dist/config.json: mksrc-tree
	mkdir -p dist		
	cp $(srcdir)/electron/src/config.json $@

build-app: package-lock.json dist/index.html dist/main.js dist/config.json

.PHONY: build-app

config/app.json: config/app.json.in | mkdir-tree
	$(AWK) -v name=credential-helper-ui-oc \
		-v version=$(PACKAGE_VERSION) \
		-e ' { \
			gsub(/!NAME!/, name); \
			gsub(/!VERSION!/, version); \
			print $$0; \
		}' $< >$@	

config/service-id.json: ../config/service-id.txt \
	key-values-to-json-1.awk | mkdir-tree
	$(AWK) -f $(srcdir)/key-values-to-json-1.awk -- -i 2 -k 1 <$< >$@

config/service-url-client.json: ../config/service-url-client-web.txt \
	key-values-to-json-1.awk | mkdir-tree
	$(AWK) -f $(srcdir)/key-values-to-json-1.awk -- -i 2 -k 1 <$< >$@

config/url-id.json: ../config/url-id.txt \
	key-values-to-json-1.awk | mkdir-tree
	$(AWK) -f $(srcdir)/key-values-to-json-1.awk -- -i 2 -k 1 <$< >$@


config/url-services.json: ../config/url-service-web.txt \
	url-service-json.awk  | mkdir-tree
	$(AWK) -f $(srcdir)/url-service-json.awk -- -i 2 <$< >$@



all-local: package-lock.json \
	package.json \
	rollup.config.electron.js \
	tsconfig.json \
	tsconfig.node.json \
	vite.config.ts \
	config/app.json \
	| mksrc-tree mkdir-tree build-app rebrand-electron

clean-unziped-electron:
	rm -r -f $(electron_extract_dir)
	rm electron-tmstamp

clean-local:
	rm -f *.json
	rm -f *.ts
	rm -f *.js
	rm -r -f $(electron_extract_dir)
	rm -f electron-tmstamp
	rm -f rebrand-electron-contents-tmstamp
	rm -f rebrand-electron-tmstamp
	rm -f config/app.json
	rm -f config/service-id.json
	rm -f config/service-url-client.json
	rm -f config/url-id.json
	rm -f config/url-service.json
	if [ -z "$(CLEAN_FOR_DEBUG)" ]; then \
		rm $(electron_zip); \
		rm -r -f $(electron_app_dir); \
		rm -r -f node_modules; \
		rm -r -f dist; \
	fi
if !COPY_JS_SRC_FOR_BUILD
	for item in $(src_dirs) ; do \
		if [ -h $$item ] ; then \
			rm $$item; \
		fi \
	done
else
	for item in $(src_dirs) ; do \
		rm -r -f $$item; \
	done
endif


install-exec-local: 
	@$(NORMAL_INSTALL)
	insdir='$(DESTDIR)$(libexecdir)/$(PKG_DEST_REL_DIR)'; \
	$(MKDIR_P) $$insdir; \
	cp -R $(electron_extract_dir)/* $$insdir



uninstall-local:
	@$(NORMAL_UNINSTALL)
	insdir='$(DESTDIR)$(libexecdir)/$(PKG_DEST_REL_DIR)'; \
	rm -rf $$insdir


# vi: se ts=4 sw=4 noet:
