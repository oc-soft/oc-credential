SUBDIRS = po oclib em-setting resource src pkg 

EXTRA_DIST = rc/rc.json \
	i18n/ja/base.txt \
	win/lang_map.txt \
	win/upgrade-code.txt \
	url/url.csv

PRIVATE_DISTFILES = src/config/client-id-dbg.txt \
	src/config/client-secret.txt \
	src/config/client-secret-dbg.txt \
	src/config/client-id.txt 

private_distname= private-$(PACKAGE)-$(VERSION)

remove-private-distdir:
	if [ -d $(distdir) ]; then \
		rm -rf $(distdir); \
	fi

.PHONY: remove-private-distdir

create-private-distdir:
	test -d "$(distdir)" || mkdir $(distdir)
	for dstf in $(PRIVATE_DISTFILES) ; do \
		dstdir=$(distdir)/`dirname $$dstf`; \
		test -d $$dstdir || mkdir -p $$dstdir; \
		cp $(top_srcdir)/$$dstf $$dstdir; \
	done

.PHONY: create-private-distdir

private-dist-gzip: create-private-distdir
	tardir=$(distdir); $(am__tar) | \
		eval GZIP= gzip $(GZIP_ENV) -c >$(private_distname).tar.gz
	$(MAKE) $(AM_MAKEFLAGS) remove-private-distdir	

.PHONY: private-dist-gzip

private-dist-bzip2: create-private-distdir
	tardir=$(distdir) $(am__tar) | \
		BZIP2=$${BZIP2--9} bzip2 $(GZIP_ENV) -c >$(private_distname).tar.bz2
	$(MAKE) $(AM_MAKEFLAGS) remove-private-distdir	

.PHONY: private-dist-bzip2

private-dist-lzip: create-private-distdir
	tardir=$(distdir) $(am__tar) | \
		lzip -c $${LZIP_OPT--9} >$(private_distname).tar.lz
	$(MAKE) $(AM_MAKEFLAGS) remove-private-distdir	

.PHONY: private-dist-lzip


private-dist-xz: crerate-private-distdir
	tardir=$(distdir) $(am__tar) | \
		XZ_OPT=$${XZ_OPT--e} xz -c >$(private_distname).tar.xz
	$(MAKE) $(AM_MAKEFLAGS) remove-private-distdir	

.PHONY: private-dist-xz

private-dist-zstd: create-private-distdir
	tardir=$(distdir) $(am__tar) | \
		zstd -c $${ZSTD_CLEVEL-$${ZSTD_OPT--19}} >$(private_distname).tar.zst
	$(MAKE) $(AM_MAKEFLAGS) remove-private-distdir	

.PHONY: private-dist-zstd

private-dist-zip: create-private-distdir
	-rm -f $(private_distname).zip
	zip -rq $(private_distname).zip $(distdir)
	$(MAKE) $(AM_MAKEFLAGS) remove-private-distdir	

.PHONY: private-dist-zip


# vi: se ts=4 sw=4 noet:
