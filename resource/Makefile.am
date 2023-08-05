

EXTRA_DIST= \
	token-generator-0.svg \
	mac-token-generator.png

if RUN_DARWIN
token_generator_icon = token-generator.ics
else
token_generator_icon = 
endif


if RUN_DARWIN
token-generator.ics: mac-token-generator.png
	$(srcdir)/icns-gen.sh -s $< -i $@

endif

token-generator-icon:$(token_generator_icon)


all-local:token-generator-icon


# vi: se ts=4 sw=4 noet:
