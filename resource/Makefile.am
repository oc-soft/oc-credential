
EXTRA_DIST= \
	token-generator-0.svg \
	mac-token-generator.png

if RUN_DARWIN
token-generator.ics: mac-token-generator.png
	$(src_dir)/icns-gen.sh -s $< -i $@

endif
# vi: se ts=4 sw=4 noet:
