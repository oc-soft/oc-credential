
bin_PROGRAMS = emsize
dist_pkgdata_DATA = library_type_size.js 
emsize_SOURCES = emsize_main.c

if TARGET_WASM
run_prog=$(NODE)
else
run_prog=
endif

awk_cmd_csv_to_json = ' \
	BEGIN { \
		lines[1] = ""; \
	} \
	{ \
		lines[NR] = "\"" $$1 "\": " $$2; \
		line_count = NR; \
	} \
	END { \
		print "{"; \
		for (i = 1; i <= line_count; i++) { \
			line = "  " lines[i]; \
			if (i < line_count) { \
				line = line ","; \
			} \
			print line; \
		} \
		print "}"; \
	} \
'

awk_cmd_gen_type_size_func_body = ' \
	BEGIN { \
		size_map_json = ""; \
		while ((getline line < size_map_json_file) > 0) { \
			if (size_map_json) { \
				size_map_json = size_map_json "\n"; \
			} \
			size_map_json = size_map_json line; \
		} \
	} \
	{ \
		gsub(/@TYPE_SIZE_MAP@/, size_map_json); \
		print $$0; \
	} \
'

awk_cmd_gen_type_size = ' \
	BEGIN { \
		indent=""; \
		for (idx = 0; idx < indent_size; idx++) { \
			indent = " " indent; \
		} \
		doc_indent=""; \
		for (idx = 0; idx < doc_indent_size; idx++) { \
			doc_indent = " " doc_indent; \
		} \
		type_size_func_body = ""; \
		while ((getline line < type_size_func_body_file) > 0) { \
			if (type_size_func_body) { \
				type_size_func_body = type_size_func_body "\n"; \
			} \
			line = indent line; \
			type_size_func_body = type_size_func_body line; \
		} \
		type_size_func_doc = ""; \
		while ((getline line < type_size_func_doc_file) > 0) { \
			if (type_size_func_doc) { \
				 type_size_func_doc = type_size_func_doc "\n"; \
			} \
			line = doc_indent line; \
			type_size_func_doc = type_size_func_doc line; \
		} \
	} \
	{ \
		gsub(/@TYPE_SIZE_DOC@/, type_size_func_doc); \
		gsub(/@TYPE_SIZE_BODY@/, type_size_func_body); \
		print $$0; \
	} \
'

size_map.csv: emsize$(EXEEXT)
	$(run_prog) $(builddir)/$< >$@

size_map.json: size_map.csv
	$(AWK) -e $(awk_cmd_csv_to_json) $< >$@

type-size-func-body.js: type-size-func-body.js.in size_map.json
	$(AWK) -v size_map_json_file=$(builddir)/size_map.json \
		-e $(awk_cmd_gen_type_size_func_body) $< >$@

library_type_size.js : library_type_size.js.in \
	type-size-func-body-doc.txt \
	type-size-func-body.js	
	$(AWK) -v type_size_func_body_file=$(builddir)/type-size-func-body.js \
		-v type_size_func_doc_file=$(srcdir)/type-size-func-body-doc.txt \
		-v doc_indent_size=0 \
		-v indent_size=4 \
		-e $(awk_cmd_gen_type_size) $< >$@

type-size.js : type-size.js.in \
	type-size-func-body-doc.txt \
	type-size-func-body.js	
	$(AWK) -v type_size_func_body_file=$(builddir)/type-size-func-body.js \
		-v type_size_func_doc_file=$(srcdir)/type-size-func-body-doc.txt \
		-v doc_indent_size=0 \
		-v indent_size=2 \
		-e $(awk_cmd_gen_type_size) $< >$@

clean-generic:
	rm -f size_map.csv
	rm -f size_map.json
	rm -f *.js
# vi: se ts=4 sw=4 noet:
