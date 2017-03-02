CC             := cc
CURDIR         := $(shell pwd)
PACKAGE_NAME   := libBitIO
DEST           := /usr/local/Packages/$(PACKAGE_NAME)
VERSION         = `grep @version $(CURDIR)/libBitIO/include/BitIO.h | echo | grep -o '[0-9]\.[0-9]\.[0-9]'`
CFLAGS         := -std=c11 -march=native -funroll-loops -ferror-limit=10240 -Wall
LDFLAGS        := -flto=thin
BUILD_DIR      := $(CURDIR)/BUILD
LIB_DIR        := $(CURDIR)/libBitIO/src
UTILITY_DIR    := $(CURDIR)/Test-BitIO
BUILD_LIB      := $(BUILD_DIR)/libBitIO
BUILD_UTILITY  := $(BUILD_DIR)/Test-BitIO

.PHONY: all install uninstall clean

DEBUG ?= 1
ifeq (DEBUG, 1)
	CFLAGS += -g -O0
else
	CFLAGS += -Ofast
endif

$(BUILD_LIB)/BitIO.o: $(LIB_DIR)/BitIO.c
	mkdir -p $(BUILD_LIB)
	$(CC) -c $< -o $@ $(CFLAGS) $(LDFLAGS)

$(BUILD_LIB)/Deflate.o: $(LIB_DIR)/Deflate.c
	mkdir -p $(BUILD_LIB)
	$(CC) -c $< -o $@ $(CFLAGS) $(LDFLAGS)

$(BUILD_LIB)/MD5.o: $(LIB_DIR)/MD5.c
	mkdir -p $(BUILD_LIB)
	$(CC) -c $< -o $@ $(CFLAGS) $(LDFLAGS)

$(BUILD_LIB)/libBitIO.a: $(BUILD_LIB)/BitIO.o\ $(BUILD_LIB)/Deflate.o\ $(BUILD_LIB)/MD5.o
	mkdir -p $(BUILD_LIB)
	$(CC) -c $< -o $@ $(CFLAGS) $(LDFLAGS)
	libtool -current_version $(VERSION) -o $(BUILD_LIB)/libBitIO.a $(BUILD_LIB)/*.o

$(BUILD_UTILITY)/Test-BitIO: BUILD_UTILITY)/UnitTest.o
	mkdir -p $(BUILD_UTILITY)
	$(CC) -c $< -o $@ $(CFLAGS) $(LDFLAGS)
	$(CC) -c $(UTILITY_DIR)/UnitTest.c -o $(BUILD_UTILITY)/Test-BitIO $(CFLAGS) $(LDFLAGS)

all: $(BUILD_LIB)/libBitIO.a\ $(BUILD_UTILITY)/Test-BitIO
