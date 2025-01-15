
GMSGFMT_ = @GMSGFMT@
XGETTEXT_ = @XGETTEXT@
MSGMERGE = @MSGMERGE@
MSGMERGE_UPDATE = @MSGMERGE@ --update
MSGMERGE_FOR_MSGFMT_OPTION = @MSGMERGE_FOR_MSGFMT_OPTION@
MSGINIT = msginit
MSGCONV = msgconv
MSGFILTER = msgfilter

LOCALE_DIR = $(srcdir)/locale

XGET_OPTION = -L JavaScript -kIntl.gettext

UI_MENU_POT_SRC = $(srcdir)/../electron/src/menu-linux.ts

EXTRA_DIST = catalogs-vars.mk \
	catalogs-depends.mk \
	ui-menu.pot \
	ja/ui-menu.po 

include $(srcdir)/catalogs-vars.mk

$(srcdir)/ui-menu.pot: $(UI_MENU_POT_SRC)
	$(XGETTEXT) $(XGET_OPTION) -o $@ $^

make-pot: $(srcdir)/ui-menu.pot
.PHONY: make-pot


$(srcdir)/ja/ui-menu.po: $(srcdir)/ui-menu.pot
	$(MSGMERGE_UPDATE) $@ $<


msg-catalog: $(CATALOGS)
.PHONY: msg-catalog

all-local: msg-catalog

include $(srcdir)/catalogs-depends.mk


clean-local:
	rm -f -r $(LOCALE_DIR)


# vi: se ts=4 sw=4 noet:
