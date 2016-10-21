noinst_LIBRARIES=libdes-cbc.a
libdes_cbc_a_SOURCES=des.cpp cbc.cpp common.h des.h

#noinst_PROGRAMS=cbc
#cbc_SOURCES=cbc.cpp des.cpp des.h
#cbc_LDADD=$(top_srcdir)/libdes.a

noinst_PROGRAMS=main
main_SOURCES=main.cpp moc_mainwindow.cc mainwindow.cpp ui_mainwindow.h
main_LDADD=$(top_srcdir)/libdes-cbc.a
main_CPPFLAGS=$(QT_CFLAGS) $(QT_LIBS) -fPIC

CLEANFILES = \
	moc_mainwindow.cc ui_mainwindow.h 

moc_mainwindow.cc: mainwindow.h
	@MOC@ -o$@ $(DEFS) $(DEFAAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(MOC_CPPFLAGS) $<

ui_mainwindow.h: mainwindow.ui
	@UIC@ -o $@ $<
