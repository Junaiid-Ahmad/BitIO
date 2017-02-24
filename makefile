CC            := cc
PACKAGENAME   := libLIVC
DEST          := /usr/local/Packages/$(PACKAGE_NAME)
VERSION       := $(shell cat ${$(CURDIR)/libBitIO/include/BitIO.h} | grep -e "@version")
CFLAGS        := -std=c11 -march=native -Ofast -funroll-loops -ferror-limit=1024 -Wall
LDFLAGS       := -flto=thin -l$(shell pkg-config --libs libBitIO) -l$(shell pkg-config --libs libModernPNG)
LIBDIR        := $(CURDIR)/libBitIO
UTILITYDIR    := $(CURDIR)/Test-BitIO
BUILDDIR      := $(CURDIR)/BUILD
BUILDLIB      := $(BUILDDIR)/libBitIO
BUILDUTILITY  := $(BUILDDIR)/Test-BitIO

.PHONY: install uninstall clean

all: $(BUILDLIB)/libBitIO.a $(BUILDUTILITY)/Test-BitIO
	$(shell mkdir -p $(BUILDUTILITY))
	$(shell mkdir -p $(BUILDLIB))
	$(BUILDLIB)/libLIVC.a
	$(BUILDUTILITY)/Test-BitIO

$(BUILDLIB)/libLIVC.a: $(LIBDIR)/BitIO.c $(LIBDIR)/UUID.c $(LIBDIR)/Deflate.c $(LIBDIR)MD5.c
	$(shell mkdir -p $(BUILDLIB))
	$(CC) -c $(LIBDIR)/BitIO.c -o $(BUILDLIB)/BitIO.o $(CFLAGS) $(LDFLAGS)
	$(CC) -c $(LIBDIR)/UUID.c -o $(BUILDLIB)/UUID.o $(CFLAGS) $(LDFLAGS)
	$(CC) -c $(LIBDIR)/Deflate.c -o $(BUILDLIB)/Deflate.o $(CFLAGS) $(LDFLAGS)
	$(CC) -c $(LIBDIR)/MD5.c -o $(BUILDLIB)/MD5.o $(CFLAGS) $(LDFLAGS)
	libtool -current_version $(VERSION) -o $(BUILDLIB)/libLIVC.a $(BUILDLIB)*.o

$(BUILDUTILITY)/Test-BitIO: $(UTILITYDIR)/UnitTest.c
	$(shell mkdir -p $(BUILDUTILITY))
	$(CC) -c $(UTILITYDIR)/UnitTest.c -o $(UTILITYDIR)/UnitTest.o $(CFLAGS) $(LDFLAGS)
