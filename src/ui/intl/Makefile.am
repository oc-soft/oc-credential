
GMSGFMT_ = @GMSGFMT@
XGETTEXT_ = @XGETTEXT@
MSGMERGE = @MSGMERGE@
MSGMERGE_UPDATE = @MSGMERGE@ --update
MSGMERGE_FOR_MSGFMT_OPTION = @MSGMERGE_FOR_MSGFMT_OPTION@
MSGINIT = msginit
MSGCONV = msgconv
MSGFILTER = msgfilter

XGET_OPTION = -L JavaScript -kIntl.gettext

UI_MENU_POT_SRC = $(srcdir)/../electron/src/menu-linux.ts


$(srcdir)/ui-menu.pot: $(UI_MENU_POT_SRC)
	$(XGETTEXT) $(XGET_OPTION) -o $@ $^

make-pot: $(srcdir)/ui-menu.pot


ja/ui-menu.po: $(srdir)/ui-menu.pot
	$(MSGMERGE_UPDATE) $@ $<		

.PHONY: make-pot

clean-local:



# vi: se ts=4 sw=4 noet:
