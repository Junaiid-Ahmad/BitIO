CC             = cc
CURDIR         = $(shell pwd)
LIB            = libBitIO
PREFIX         = /usr/local/Packages/$(LIB)
VERSION        = $(shell grep @version $(CURDIR)/libBitIO/include/BitIO.h | echo | grep -o '[0-9]\.[0-9]\.[0-9]')
CFLAGS         = -std=c11 -march=native -funroll-loops -ferror-limit=1024 -Wall
LDFLAGS        = -flto=thin
BUILD_DIR      = $(CURDIR)/BUILD
LIB_DIR        = $(CURDIR)/libBitIO/src
LIB_INC        = $(CURDIR)/libBitIO/include
UTILITY_DIR    = $(CURDIR)/Test-BitIO
BUILD_LIB      = $(BUILD_DIR)/libBitIO
BUILD_UTILITY  = $(BUILD_DIR)/Test-BitIO

LIB_SOURCE_FILES  = $(LIB_DIR)/BitIO.c $(LIB_DIR)/Deflate.c $(LIB_DIR)/MD5.c
LIB_OBJECT_FILES  = $(BUILD_LIB)/BitIO.o $(BUILD_LIB)/Deflate.o $(BUILD_LIB)/MD5.o
LIB_INCLUDE_FILES = $(LIB_INC)/BitIO.h $(LIB_INC)/Deflate.h $(LIB_INC)/MD5.h

.PHONY: all install uninstall clean DEBUG

.DEFAULT_GOAL := all

debug:   CFLAGS += -g -O0 -DDEBUG
release: CFLAGS += -Ofast -DNDEBUG

all: $(BUILD_LIB)/libBitIO.a $(BUILD_UTILITY)/Test-BitIO

#TODO: Make a target for the documentation to be built and installed to $(prefix)/docs as well

$(BUILD_LIB)/libBitIO.a($(LIB_OBJECT_FILES)): $(LIB_SOURCE_FILES) $(LIB_INCLUDE_FILES)
	$(CC) -c $< -o $@ $(CFLAGS) $(LDFLAGS)
	ar -r -c -s $(BUILD_LIB)/libBitIO.a $(BUILD_LIB)/*.o

$(BUILD_UTILITY)/Test-BitIO: $(UTILITY_DIR)/UnitTest.c $(LIB_INCLUDE_FILES)
	$(CC) -c $(UTILITY_DIR)/UnitTest.c -lBitIO.a -o $(BUILD_UTILITY)/Test-BitIO $(CFLAGS) $(LDFLAGS)
	strip $(BUILD_UTILITY)/Test-BitIO

install: $(PREFIX)/bin/Test-BitIO $(PREFIX)/lib/libBitIO.a $(PREFIX)/include/BitIO.h $(PREFIX)/include/Deflate.h $(PREFIX)/include/MD5.h
	install -c -d -f 0555 $(BUILD_UTILITY)/Test-BitIO $(PREFIX)/bin/Test-BitIO
	install -c -d -f 0444 $(BUILD_LIB)/libBitIO.a $(PREFIX)/lib/libBitIO.a
	install -c -d -f 0444 $(LIB_INC)/BitIO.h $(PREFIX)/include/BitIO.h
	install -c -d -f 0444 $(LIB_INC)/Deflate.h $(PREFIX)/include/Deflate.h
	install -c -d -f 0444 $(LIB_INC)/MD5.h $(PREFIX)/include/MD5.h

uninstall:
	rm -d -i -R $(PREFIX)/bin/Test-BitIO
	rm -d -i -R $(PREFIX)/lib/libBitIO.a
	rm -d -i -R $(PREFIX)/include/BitIO.h
	rm -d -i -R $(PREFIX)/include/Deflate.h
	rm -d -i -R $(PREFIX)/include/MD5.h
	rm -d -i -R $(PREFIX)/*/.DS_Store

clean:
	rm -d -i -R $(BUILD_UTILITY)/Test-BitIO
	rm -d -i -R $(BUILD_UTILITY)/*.o
	rm -d -i -R $(BUILD_LIB)/libBitIO.a
	rm -d -i -R $(BUILD_LIB)/*.o
	rm -d -i -R $(BUILD_DIR)/*/.DS_Store
	rm -d -i -R $(BUILD_DIR)
