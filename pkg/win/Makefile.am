
SUBDIRS = ja

if BUILD_MS
noinst_PROGRAMS = cabeng cabhash
endif

if MINGW_HOST
SUBDIRS += cabx
endif
EXTRA_DIST = ServiceControl.idt \
	_Validation.idt \
	MsiLockPermissionsEx.idt \
	Condition.idt \
	PublishComponent.idt \
	Environment.idt \
	CustomAction.idt \
	ODBCTranslator.idt \
	MsiFileHash.idt \
	Property.idt.in \
	MsiDigitalSignature.idt \
	ComboBox.idt \
	CCPSearch.idt \
	File_org.idt \
	AdvtUISequence.idt \
	ODBCDataSource.idt \
	Upgrade.idt \
	Dialog.idt \
	Shortcut.idt \
	Class.idt \
	ActionText.idt \
	CompLocator.idt \
	BindImage.idt \
	Component.idt.in \
	Billboard.idt \
	InstallUISequence.idt \
	RemoveRegistry.idt \
	File.idt.in \
	LockPermissions.idt \
	MsiPatchMetadata.idt \
	Verb.idt \
	PatchPackage.idt \
	TextStyle.idt \
	MsiServiceConfig.idt \
	Error.idt \
	MsiAssemblyName.idt \
	MsiPatchHeaders.idt \
	MoveFile.idt \
	MIME.idt \
	ControlEvent.idt \
	MsiShortcutProperty.idt \
	IniFile.idt \
	SFPCatalog.idt \
	AdminUISequence.idt \
	Binary.idt \
	ServiceInstall.idt \
	FileSFPCatalog.idt \
	MsiAssembly.idt \
	ReserveCost.idt \
	Registry.idt \
	ODBCDriver.idt \
	Control.idt \
	MsiPatchOldAssemblyName.idt \
	Directory.idt \
	DrLocator.idt \
	MsiDigitalCertificate.idt \
	Icon.idt \
	Signature.idt \
	ODBCSourceAttribute.idt \
	ODBCAttribute.idt \
	AppId.idt \
	Feature.idt \
	RemoveFile.idt \
	CheckBox.idt \
	MsiPatchOldAssemblyFile.idt \
	AppSearch.idt \
	MsiServiceConfigFailureActions.idt \
	RadioButton.idt \
	AdvtExecuteSequence.idt \
	RegLocator.idt \
	BBControl.idt \
	MsiPatchCertificate.idt \
	MsiEmbeddedUI.idt \
	IsolatedComponent.idt \
	EventMapping.idt \
	ListView.idt \
	UIText.idt \
	ControlCondition.idt \
	Media.idt \
	TypeLib.idt \
	MsiSFCBypass.idt \
	AdminExecuteSequence.idt \
	Font.idt \
	DuplicateFile.idt \
	IniLocator.idt \
	Complus.idt \
	Extension.idt \
	InstallExecuteSequence.idt \
	MsiPackageCertificate.idt \
	LaunchCondition.idt \
	RemoveIniFile.idt \
	Binary/Up.ibd \
	Binary/New.ibd \
	ListBox.idt \
	SelfReg.idt \
	MsiEmbeddedChainer.idt \
	CreateFolder.idt \
	MsiPatchSequence.idt \
	ProgId.idt \
	Patch.idt \
	FeatureComponents-x86_64.idt.in \
	FeatureComponents-i386.idt.in \
	File-x86_64.idt.in \
	File-i386.idt.in \
	Component-x86_64.idt.in \
	Component-i386.idt.in


MSIDB = msidb.exe
MSIINFO = msiinfo.exe
MSICERT = msicert.exe

credhelper_prog_exe = credhelper.exe
credhelper_prog = $(top_builddir)/src/credential-helper/credential-ocs/credhelper.exe
lang_map = $(top_srcdir)/win/lang_map.txt
upgrade_code_txt = $(top_srcdir)/win/upgrade-code.txt
link_csv = $(top_srcdir)/url/url.csv

dll_sys_root=@OC_DLL_SYS_ROOT@
company_name=@OC_COMPANY_NAME@

staging_dir=/tmp/staging
credhelper_dist_name=credential-ocs

src_app_dir=App


if BUILD_MS
cabeng_SOURCES=cabeng_main.c
cabeng_CPPFLAGS=-I$(top_srcdir)/oclib/col/include
cabeng_LDFLAGS= -static -municode -specs=$(srcdir)/ucrt.specs -lmsi -lpathcch
cabeng_LDADD=$(top_builddir)/oclib/col/src/liboccol.la


cabhash_CPPFLAGS = -I$(top_srcdir)/oclib/col/include \
	-I$(top_srcdir)/oclib/buffer/include \
	-I$(top_srcdir)/oclib/cstr/include \
	-I$(top_srcdir)/oclib/csv/include

cabhash_SOURCES = cabhash_main.c \
	number_parser.c \
	str_conv_win.c

cabhash_LDFLAGS= -static -municode -specs=${srcdir}/ucrt.specs -lmsi -lpathcch -lcrypt32

cabhash_LDADD=$(top_builddir)/oclib/col/src/liboccol.la \
	$(top_builddir)/oclib/cstr/src/liboccstr.la \
	$(top_builddir)/oclib/buffer/src/libocbuffer.la \
	$(top_builddir)/oclib/csv/src/liboccsv.la

endif

MSI_TABLES_0 = \
	ActionText.idt \
	AdminExecuteSequence.idt \
	AdminUISequence.idt \
	AdvtExecuteSequence.idt \
	AdvtUISequence.idt \
	AppId.idt \
	AppSearch.idt \
	BBControl.idt \
	Billboard.idt \
	Binary.idt \
	BindImage.idt \
	CCPSearch.idt \
	CheckBox.idt \
	Class.idt \
	ComboBox.idt \
	CompLocator.idt \
	Complus.idt \
	Condition.idt \
	Control.idt

MSI_TABLES_1 = \
	ControlCondition.idt \
	ControlEvent.idt \
	CreateFolder.idt \
	CustomAction.idt \
	Dialog.idt \
	Directory.idt \
	DrLocator.idt \
	DuplicateFile.idt \
	Environment.idt \
	Error.idt \
	EventMapping.idt \
	Extension.idt \
	Feature.idt \
	FileSFPCatalog.idt \
	Font.idt 

MSI_TABLES_2 = \
	Icon.idt \
	IniFile.idt \
	IniLocator.idt \
	InstallExecuteSequence.idt \
	InstallUISequence.idt \
	IsolatedComponent.idt \
	LaunchCondition.idt \
	ListBox.idt \
	ListView.idt \
	MIME.idt \
	Media.idt \
	MoveFile.idt \
	MsiAssembly.idt \
	MsiAssemblyName.idt \
	MsiDigitalCertificate.idt \
	MsiDigitalSignature.idt \
	MsiEmbeddedChainer.idt \
	MsiEmbeddedUI.idt \
	MsiFileHash.idt 

MSI_TABLES_3 = \
	MsiLockPermissionsEx.idt \
	MsiPackageCertificate.idt \
	MsiPatchCertificate.idt \
	MsiPatchHeaders.idt \
	MsiPatchMetadata.idt \
	MsiPatchOldAssemblyFile.idt \
	MsiPatchOldAssemblyName.idt \
	MsiPatchSequence.idt \
	MsiSFCBypass.idt \
	MsiServiceConfig.idt \
	MsiServiceConfigFailureActions.idt \
	MsiShortcutProperty.idt 

MSI_TABLES_4 = \
	ODBCAttribute.idt \
	ODBCDataSource.idt \
	ODBCDriver.idt \
	ODBCSourceAttribute.idt \
	ODBCTranslator.idt \
	Patch.idt \
	PatchPackage.idt \
	ProgId.idt \
	PublishComponent.idt \
	RadioButton.idt \
	RegLocator.idt \
	Registry.idt \
	RemoveIniFile.idt \
	RemoveRegistry.idt \
	ReserveCost.idt 

MSI_TABLES_5 = \
	SFPCatalog.idt \
	SelfReg.idt \
	ServiceControl.idt \
	ServiceInstall.idt \
	Shortcut.idt \
	Signature.idt \
	TextStyle.idt \
	TypeLib.idt \
	UIText.idt \
	Upgrade.idt \
	Verb.idt \
	_Validation.idt 



awk_cmd_0 = 'BEGIN { \
		langs_count=0; \
	} \
	{ \
		langs[langs_count] = $$1; \
		langs_count++; \
	} \
	END { \
		langs_str="0x0"; \
		for (i = 0; i < langs_count; i++) { \
			langs_str=langs_str "," langs[i]; \
		} \
		print langs_str; \
	} \
'
awk_cmd_1='BEGIN {\
		  map_size = 0; \
	} \
	{ \
		langs[map_size] = strtonum($$1); \
		map_size++;	\
	} \
	END { \
		if (map_size >0) { \
			lang_ids=langs[0]; \
			for (i = 1; i < map_size; i++) { \
				lang_ids = lang_ids "," langs[i]; \
			} \
		} else { \
			lang_ids="0"; \
		} \
		print lang_ids; \
	} \
'

awk_cmd_2 = ' \
	BEGIN { \
		FS=","; \
	} \
	{ \
		kv[$$1] = $$2; \
	} \
	END { \
		print kv["help-link-msi"]; \
	} \
'
awk_cmd_3 = ' \
	{ \
		sub("%HELP_LINK%", help_link); \
		print; \
	} \
'

copy_depends=IFS=: ; while read dll dll_dir; do \
		if [ -n "$$dll_dir" ]; then \
			cp $$dll_dir/$$dll $$destdir; \
		fi ; \
	done 


if RUN_WIN
all-local: Makefile $(PROGRAMS)
endif

staging-install:
	$(MAKE) -C $(top_builddir) DESTDIR=$(staging_dir) install
	mv $(staging_dir)$(libexecdir)/credential-ocs/credhelper$(EXEEXT) \
		$(staging_dir)$(libexecdir)/credential-ocs/$(credhelper_dist_name)$(EXEEXT) 
	$(MAKE) staging-install-post

staging-uninstall:
	rm -r $(staging_dir)$(libexecdir)/credential-ocs/local
	rm -r -f $(staging_dir)$(libexecdir)/credential-ocs/*.dll
	mv $(staging_dir)$(libexecdir)/credential-ocs/$(credhelper_dist_name)$(EXEEXT) \
		$(staging_dir)$(libexecdir)/credential-ocs/credhelper$(EXEEXT) 
	$(MAKE) -C $(top_builddir) DESTDIR=$(staging_dir) uninstall

staging-install-post:
	dstdir=$(staging_dir)$(libexecdir)/credential-ocs \
	inputs=$(staging_dir)$(libexecdir)/credential-ocs/$(credhelper_dist_name)$(EXEEXT) \
	dll_search_dir=`$(CC) -print-sysroot`/mingw/bin ; \
		$(srcdir)/depends-dll.sh -s $$dll_search_dir -i $$inputs \
		| (IFS=: ; while read dll dll_path; do \
			if [ -n "$$dll_path" ]; then \
				cp $$dll_path/$$dll $$dstdir; \
			fi; \
		done) 
	rm $(staging_dir)$(libexecdir)/credential-ocs/locpath.txt 
	cp -r $(staging_dir)$(localedir) $(staging_dir)$(libexecdir)/credential-ocs 


staging-uninstall-post:

.PHONY: staging-install staging-uninstall staging-install-post

clean-old-inst-sources:
	rm -f -r /tmp/old-inst

.PHONY: clean-old-inst-sources

prepare-old-inst-sources:
	pushd $(top_srcdir); \
		./pkg/win/inst-log.sh -t 'v*$(host_cpu)' \
			-f pkg/win/File-$(host_cpu).idt.in \
			-f pkg/win/Component-$(host_cpu).idt.in \
			-f pkg/win/FeatureComponents-$(host_cpu).idt.in \
			-p /tmp/old-inst; \
		popd

.PHONY: prepare-old-inst-sources

gen-comp-file-remove : prepare-old-inst-sources
	pushd $(top_srcdir); \
		file_idts=`./pkg/win/inst-log.sh -i -t 'v*$(host_cpu)' \
			-f pkg/win/File-$(host_cpu).idt.in \
			-p /tmp/old-inst | \
			sort -V | sed -e 's/^\(.\)/-f \1/'` \
		comp_idts=`./pkg/win/inst-log.sh -i -t 'v*$(host_cpu)' \
			-f pkg/win/Component-$(host_cpu).idt.in \
			-p /tmp/old-inst | \
			sort -V | sed -e 's/^\(.\)/-c \1/'`; \
		./pkg/win/cmpfl-gen.sh -t ./pkg/win/Directory.idt \
			-o ./pkg/win/Component-$(host_cpu).idt.in \
			-q ./pkg/win/File-$(host_cpu).idt.in \
			-r ./pkg/win/RemoveFile-$(host_cpu).idt.in \
			-m -n -s \
			-d $(staging_dir)/usr/local/libexec \
			-e ./pkg/win/RemoveFileHeader.idt \
			$$file_idts $$comp_idts


.PHONY: gen-comp-file-remove

gen-feat-comp: 
	if [ -f $(srcdir)/Component-$(host_cpu).idt.in ] ; then \
		src_ft_opt=`$(srcdir)/inst-log.sh -i -t 'v*$(host_cpu)' \
			-f pkg/win/FeatureComponents-$(host_cpu).idt.in \
			-p /tmp/old-inst | \
				sort -V -r | while read ;do echo "-f $$REPLY"; break; done`; \
		$(srcdir)/ftcmp-gen.sh $$src_ft_opt \
			-c $(srcdir)/Component-$(host_cpu).idt.in -n credential_ocs \
			-o $(srcdir)/FeatureComponents-$(host_cpu).idt.in; \
	else \
		echo "You need to run make gen-template"; \
	fi


.PHONY:gen-feat-comp

gen-template: gen-comp-file-remove gen-feat-comp

.PHONY: gen-template


clean-stmp:
	rm -f *.stmp

clean-local: clean-stmp
	rm -f *.msi
	rm -f *.idt

Property.idt : Property.idt.in $(upgrade_code_txt) $(link_csv) 
	upgrade_code=`cat $(upgrade_code_txt)`; \
	help_link=`awk -e $(awk_cmd_2) $(link_csv)`; \
	sed -e " \
		s/%VERSION%/$(VERSION)/; \
		s/%PRODUCT_NAME%/$(PACKAGE)/; \
		s/%MANUFACTURER%/$(company_name)/; \
		s/%UPGRADE_CODE%/$$upgrade_code/" $< | \
	awk -e $(awk_cmd_3) -v help_link=$$help_link - > $@

Component.idt : Component-$(host_cpu).idt.in
	cp $< $@

File.idt : File-$(host_cpu).idt.in 
	cp $< $@

FeatureComponents.idt : FeatureComponents-$(host_cpu).idt.in
	cp $< $@

RemoveFile.idt : RemoveFile-$(host_cpu).idt.in
	cp $< $@

oc-credential_msi_pre: $(pkg_src)


credential-ocs.msi: oc-credential_msi_pre $(MSI_TABLES_0) $(MSI_TABLES_1) $(MSI_TABLES_2) $(MSI_TABLES_3) $(MSI_TABLES_4) $(MSI_TABLES_5) File.idt Component.idt Property.idt FeatureComponents.idt RemoveFile.idt
	msi_date=`date +%Y/%m/%d\ %H:%M:%S`; \
	pkg_code=`uuidgen | tr [:lower:] [:upper:]`; \
	msi_platform=`case $(host_cpu) in \
	x86_64*) echo x64;; \
	*) echo Intel;; \
	esac`; \
	msi_langs=`awk $(awk_cmd_1) $(lang_map)`; \
	$(MSIDB) -c -i -f `cygpath -a -w -d $(srcdir)` -d $@ $(MSI_TABLES_0) && \
	$(MSIDB) -i -f `cygpath -a -w -d $(srcdir)` -d $@ $(MSI_TABLES_1) && \
	$(MSIDB) -i -f `cygpath -a -w -d $(srcdir)` -d $@ $(MSI_TABLES_2) && \
	$(MSIDB) -i -f `cygpath -a -w -d $(srcdir)` -d $@ $(MSI_TABLES_3) && \
	$(MSIDB) -i -f `cygpath -a -w -d $(srcdir)` -d $@ $(MSI_TABLES_4) && \
	$(MSIDB) -i -f `cygpath -a -w -d $(srcdir)` -d $@ $(MSI_TABLES_5) && \
	$(MSIDB) -i -f `cygpath -a -w -d $(builddir)` -d $@ File.idt \
		Component.idt Property.idt FeatureComponents.idt && \
	$(MSIINFO) $@ -a "$(company_name)" -c 20127 -t "Installation Database" \
		-o "This installer database contains the logic and data required to install $(PACKAGE)" \
		-k "installer,git,git-credential" \
		-v "{$$pkg_code}" \
		-r "$$msi_date" -s "$$msi_date" \
		-g 500 \
		-w 8 \
		-p "$${msi_platform};$${msi_langs}"

$(staging_dir)$(libexecdir)/credential-ocs.msi:credential-ocs.msi
	cp $< $@

staging-msi:$(staging_dir)$(libexecdir)/credential-ocs.msi



.PHONY: staging-msi


credential-ocs-ja.msi: credential-ocs.msi \
	$(srcdir)/ja/Control.idt \
	$(srcdir)/ja/UiText.idt \
	$(srcdir)/ja/Error.idt \
	$(builddir)/ja/Property.idt
	cp $< $@
	pkg_code=`uuidgen | tr [:lower:] [:upper:]`; \
	$(MSIDB) -i -f `cygpath -a -w -d $(srcdir)/ja` -d $@ Control.idt && \
	$(MSIDB) -i -f `cygpath -a -w -d $(srcdir)/ja` -d $@ UiText.idt && \
	$(MSIDB) -i -f `cygpath -a -w -d $(srcdir)/ja` -d $@ Error.idt && \
	$(MSIDB) -i -f `cygpath -a -w -d $(builddir)/ja` -d $@ Property.idt && \
	$(MSIINFO) $@ -v "{$$pkg_code}"
	

$(staging_dir)$(libexecdir)/credential-ocs-ja.msi:credential-ocs-ja.msi
	cp $< $@

staging-msi-ja:$(staging_dir)$(libexecdir)/credential-ocs-ja.msi

.PHONY: staging-msi-ja

clean-staging-msi:
	rm -f $(staging_dir)$(libexecdir)/*.msi

.PHONY:clean-staging-msi

if BUILD_MS

create-cabs.stmp:cabeng$(EXEEXT) staging-msi
	pushd  $(staging_dir)$(libexecdir);  \
		$(abs_builddir)/cabeng$(EXEEXT) -m credential-ocs.msi \
		| $(abs_builddir)/cabx/src/cabx.exe -c 'appmod-%d.cab' && popd
	: >$@

create-cabs:create-cabs.stmp


create-cabs-input:cabeng$(EXEEXT) staging-msi
	cd $(staging_dir)$(libexecdir);  \
		$(abs_builddir)/cabeng$(EXEEXT) -m credential-ocs.msi >cabex-in.csv

setup:
	$(MKDIR_P) setup
		
setup/credential-ocs.msi: staging-msi | setup
	cp $(staging_dir)$(libexecdir)/credential-ocs.msi setup

setup/credential-ocs-ja.msi: staging-msi-ja | setup
	cp $(staging_dir)$(libexecdir)/credential-ocs-ja.msi setup


setup/appmod-0.cab: create-cabs setup/credential-ocs.msi | setup
	cp $(staging_dir)$(libexecdir)/appmod-0.cab setup

setup/appmod-1.cab: create-cabs setup/credential-ocs.msi | setup
	cp $(staging_dir)$(libexecdir)/appmod-1.cab setup

code-sign-cabs.stmp:setup/appmod-0.cab \
	setup/appmod-1.cab
	$(abs_srcdir)/oc-code-sign.sh setup/appmod-0.cab setup/appmod-1.cab
	: >$@

code-sign-cabs:code-sign-cabs.stmp

include-cert-hash.stmp: code-sign-cabs.stmp \
	setup/credential-ocs.msi \
	setup/credential-ocs-ja.msi 
	for msi_path in setup/*.msi; do \
		for idx in `seq 0 1`; do \
			media_id=idx; \
			let media_id+=1; \
			$(MSICERT) -d $$msi_path -m $$media_id \
			-c setup/appmod-$${idx}.cab; \
		done; \
	done
	: >$@

include-cert-hash: include-cert-hash.stmp

create-compressed-msi.stmp: include-cert-hash.stmp
	cd setup; for fms in *.msi; do \
		$(MSIINFO) $$fms /W 10 ; \
	done
	: >$@

create-compressed-msi: create-compressed-msi.stmp


sign-compressed-msi.stmp: create-compressed-msi.stmp
	for msi_path in setup/*.msi; do \
		$(abs_srcdir)/oc-code-sign.sh $$msi_path; \
	done
	: >$@

sign-compressed-msi: sign-compressed-msi.stmp
	
endif

.PHONY: create-cabs
.PHONY: create-cabs-input
.PHONY: update-code-sign-entry
.PHONY: code-sign-cabs
.PHONY: include-cert-hash
.PHONY: create-compressed-msi
.PHONY: sign-compressed-msi

# vi: se ts=4 sw=4 noet:
