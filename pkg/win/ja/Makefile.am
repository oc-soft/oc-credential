EXTRA_DIST = Property.idt.in \
	Error.idt \
	Control.idt \
	UIText.idt

lang = ja
base_lang_txt = $(top_srcdir)/i18n/$(lang)/base.txt
upgrade_code_txt = $(top_srcdir)/win/upgrade-code.txt
link_csv = $(top_srcdir)/url/url.csv

all-am: Makefile $(PROGRAMS) Property.idt

awk_cmd_1=' BEGIN { \
	FS = "\t"; \
	kv_count = 0; \
} \
{ \
	keys[kv_count] = $$1; \
	values[kv_count] = $$2; \
	kv_count++; \
} \
END { \
	getline upgrade_code < "$(upgrade_code_txt)"; \
	src_file="$(srcdir)/Property.idt.in"; \
	while ((getline line < src_file) > 0) { \
		for (i = 0; i < kv_count; i++) { \
			sub("%" keys[i] "%", values[i], line); \
		} \
		sub("%UPGRADE_CODE%", upgrade_code, line); \
		sub("%VERSION%", "$(VERSION)", line); \
		sub("%HELP_LINK%", help_link, line); \
		print line; \
	} \
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

Property.idt: Property.idt.in $(base_lang_txt) $(upgrade_code_txt) $(link_csv)
	help_link=`awk -e $(awk_cmd_2) $(link_csv)`; \
	awk -e $(awk_cmd_1) -v help_link=$$help_link $(base_lang_txt) > $@

clean-generic:
	rm -f *.idt

# vi: se ts=4 sw=4 noet:
