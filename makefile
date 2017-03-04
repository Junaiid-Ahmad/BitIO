CC             := cc
CURDIR         := $(shell pwd)
LIB            := libBitIO
PREFIX         ?= /usr/local/Packages/$(LIB)
VERSION         = `grep @version $(CURDIR)/libBitIO/include/BitIO.h | echo | grep -o '[0-9]\.[0-9]\.[0-9]'`
CFLAGS         := -std=c11 -march=native -funroll-loops -ferror-limit=10240 -Wall
LDFLAGS        := -flto=thin
BUILD_DIR      := $(CURDIR)/BUILD
LIB_DIR        := $(CURDIR)/libBitIO/src
LIB_INC        := $(CURDIR)/libBitIO/include
UTILITY_DIR    := $(CURDIR)/Test-BitIO
BUILD_LIB      := $(BUILD_DIR)/libBitIO
BUILD_UTILITY  := $(BUILD_DIR)/Test-BitIO

.PHONY: all install uninstall clean DEBUG

DEBUG ?= 1
.ifeq (DEBUG, 1)
	CFLAGS += -g -O0
else
	CFLAGS += -Ofast
.endif

install : $(PREFIX)/bin/Test-BitIO $(PREFIX)/lib/libBitIO.a $(PREFIX)/include/BitIO.h $(PREFIX)/include/Deflate.h $(PREFIX)/include/MD5.h
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

$(BUILD_LIB)/BitIO.o : $(LIB_DIR)/BitIO.c $(LIB_INC)/BitIO.h
	mkdir -p $(BUILD_LIB)
	$(CC) -c $< -o $@ $(CFLAGS) $(LDFLAGS)

$(BUILD_LIB)/Deflate.o : $(LIB_DIR)/Deflate.c
	mkdir -p $(BUILD_LIB)
	$(CC) -c $< -o $@ $(CFLAGS) $(LDFLAGS)

$(BUILD_LIB)/MD5.o : $(LIB_DIR)/MD5.c
	mkdir -p $(BUILD_LIB)
	$(CC) -c $< -o $@ $(CFLAGS) $(LDFLAGS)

$(BUILD_LIB)/libBitIO.a($(BUILD_LIB)/BitIO.o $(BUILD_LIB)/Deflate.o $(BUILD_LIB)/MD5.o) : $(BUILD_LIB)/BitIO.o $(BUILD_LIB)/Deflate.o $(BUILD_LIB)/MD5.o
	ar -r -c -s $(BUILD_LIB)/libBitIO.a $(BUILD_LIB)/*.o
	libtool -current_version $(VERSION) -o $(BUILD_LIB)/libBitIO.a $(BUILD_LIB)/*.o

$(BUILD_UTILITY)/UnitTest.o : $(UTILITY_DIR)/UnitTest.c $(LIB_INC)/BitIO.h $(LIB_INC)/Deflate.h $(LIB_INC)/MD5.h
	mkdir -p $(BUILD_UTILITY)
	$(CC) -c $(UTILITY_DIR)/UnitTest.c -o $(BUILD_UTILITY)/Test-BitIO $(CFLAGS) $(LDFLAGS)

$(BUILD_UTILITY)/Test-BitIO : $(BUILD_UTILITY)/UnitTest.o
	$(CC) $(BUILD_UTILITY)/UnitTest.o -l$(BUILD_LIB)/libBitIO.a -o $(BUILD_UTILITY)/Test-BitIO $(CFLAGS) $(LDFLAGS)
	strip $(BUILD_UTILITY)/Test-BitIO

all : $(BUILD_LIB)/libBitIO.a $(BUILD_UTILITY)/Test-BitIO
