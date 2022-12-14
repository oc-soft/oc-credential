SUBDIRS = ja


MSIDB = msidb.exe
MSIINFO = msiinfo.exe

lmdprog_exe = lmdcli.exe
lmdprog = $(top_builddir)/src/lmdcli/.libs/lmdcli.exe
lang_map = $(top_srcdir)/win/lang_map.txt
upgrade_code_txt = $(top_srcdir)/win/upgrade-code.txt
link_csv = $(top_srcdir)/url/url.csv

dll_sys_root=@OC_DLL_SYS_ROOT@
company_name=@OC_COMPANY_NAME@

libcurl_dll=libcurl-4.dll
libintl_dll=libintl-8.dll
libjson_c_dll=libjson-c-2.dll

src_libcurl_dll=$(dll_sys_root)/$(libcurl_dll)
src_libintl_dll=$(dll_sys_root)/$(libintl_dll)
src_libjson_c_dll=$(dll_sys_root)/$(libjson_c_dll)

src_app_dir=App

pkg_src= \
	$(src_app_dir)/$(libcurl_dll) \
	$(src_app_dir)/$(libintl_dll) \
	$(src_app_dir)/$(libjson_c_dll) \
	$(src_app_dir)/$(lmdprog_exe)

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
	FeatureComponents.idt \
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
	RemoveFile.idt \
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


if RUN_WIN
all-am: Makefile $(PROGRAMS) oc-credential.msi oc-credential-ja.msi
endif


clean-generic:
	rm -f *.msi
	rm -f *.idt

Component.idt : Component.idt.in
	sed -e " \
		s/%LMDCLI_EXE%/$(notdir $(lmdprog))/; \
		s/%LIBCURL_DLL%/$(libcurl_dll)/; \
		s/%LIBINTL_DLL%/$(libintl_dll)/; \
		s/%LIBJSON_C_DLL%/$(libjson_c_dll)/" $< > $@ 

Property.idt : Property.idt.in $(upgrade_code_txt) $(link_csv) 
	upgrade_code=`cat $(upgrade_code_txt)`; \
	help_link=`awk -e $(awk_cmd_2) $(link_csv)`; \
	sed -e " \
		s/%VERSION%/$(VERSION)/; \
		s/%PRODUCT_NAME%/$(PACKAGE)/; \
		s/%MANUFACTURER%/$(company_name)/; \
		s/%UPGRADE_CODE%/$$upgrade_code/" $< | \
	awk -e $(awk_cmd_3) -v help_link=$$help_link - > $@


File.idt : File.idt.in $(lang_map)
	lmd_size=`du -b $(lmdprog) | awk -e '{ print $$1 }'` ; \
	libcurl_dll_size=`du -b $(src_libcurl_dll) | awk -e '{print $$1 }'`; \
	libintl_dll_size=`du -b $(src_libintl_dll) | awk -e '{print $$1 }'`; \
 	libjson_c_dll_size=`du -b $(src_libjson_c_dll) | awk -e '{print $$1 }'`; \
	langs=`awk -e $(awk_cmd_0) $(lang_map)`; \
	sed -e " \
		s/%LMDCLI_SIZE%/$$lmd_size/; \
	   	s/%LMDCLI_VERSION%/$(PACKAGE_VERSION)/; \
		s/%LMDCLI_LANGS%/$$langs/; \
		s/%LMDCLI_EXE%/$(notdir $(lmdprog))/; \
		s/%LIBCURL_DLL%/$(libcurl_dll)/; \
		s/%LIBINTL_DLL%/$(libintl_dll)/; \
		s/%LIBJSON_C_DLL%/$(libjson_c_dll)/; \
		s/%LIBCURL_SIZE%/$$libcurl_dll_size/; \
		s/%LIBINTL_SIZE%/$$libintl_dll_size/; \
		s/%LIBJSON_C_SIZE%/$$libjson_c_dll_size/" $< > $@; 


oc-credential_msi_pre: $(pkg_src)


oc-credential.msi: oc-credential_msi_pre $(MSI_TABLES_0) $(MSI_TABLES_1) $(MSI_TABLES_2) $(MSI_TABLES_3) $(MSI_TABLES_4) $(MSI_TABLES_5) File.idt Component.idt Property.idt
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
		Component.idt Property.idt && \
	$(MSIINFO) $@ -a "$(company_name)" -c 20127 -t "Installation Database" \
		-o "This installer database contains the logic and data required to install $(PACKAGE)" \
		-k "installer,git,git-credential" \
		-v "{$$pkg_code}" \
		-r "$$msi_date" -s "$$msi_date" \
		-g 500 \
		-w 8 \
		-p "$${msi_platform};$${msi_langs}"


oc-credential-ja.msi: oc-credential.msi $(srcdir)/ja/Control.idt $(srcdir)/ja/UiText.idt $(srcdir)/ja/Error.idt $(builddir)/ja/Property.idt
	cp $< $@
	pkg_code=`uuidgen | tr [:lower:] [:upper:]`; \
	$(MSIDB) -i -f `cygpath -a -w -d $(srcdir)/ja` -d $@ Control.idt && \
	$(MSIDB) -i -f `cygpath -a -w -d $(srcdir)/ja` -d $@ UiText.idt && \
	$(MSIDB) -i -f `cygpath -a -w -d $(srcdir)/ja` -d $@ Error.idt && \
	$(MSIDB) -i -f `cygpath -a -w -d $(builddir)/ja` -d $@ Property.idt && \
	$(MSIINFO) $@ -v "{$$pkg_code}"
	

$(src_app_dir)/$(libcurl_dll) : $(src_libcurl_dll)
	$(MKDIR_P) $(src_app_dir)
	cp $< $@

$(src_app_dir)/$(libintl_dll) : $(src_libintl_dll)
	$(MKDIR_P) $(src_app_dir)
	cp $< $@

$(src_app_dir)/$(libjson_c_dll) : $(src_libjson_c_dll)
	$(MKDIR_P) $(src_app_dir)
	cp $< $@

$(src_app_dir)/$(lmdprog_exe) : $(lmdprog)
	$(MKDIR_P) $(src_app_dir)
	cp $< $@



# vi: se ts=4 sw=4 noet:
