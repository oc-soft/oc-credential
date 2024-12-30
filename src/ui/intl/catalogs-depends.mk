$(srcdir)/locale/ja/LC_MESSAGES/ui-menu.mo: $(srcdir)/ja/ui-menu.po
	if test ! -d $(@D); then \
		$(MKDIR_P) $(@D); \
	fi
	$(MSGFMT) -o $@ $< 
$(srcdir)/locale/ja_JP/LC_MESSAGES/ui-menu.mo: $(srcdir)/ja/ui-menu.po
	if test ! -d $(@D); then \
		$(MKDIR_P) $(@D); \
	fi
	$(MSGFMT) -o $@ $< 
