CC             := cc
PACKAGENAME    := libBitIO
DEST           := /usr/local/Packages/$(PACKAGE_NAME)
VERSION         = `grep @version $(CURDIR)/libBitIO/include/BitIO.h | echo | grep -o '[0-9]\.[0-9]\.[0-9]'`
CFLAGS         := -std=c11 -march=native -Ofast -funroll-loops -ferror-limit=10240 -Wall
LDFLAGS        := -flto=thin
CURDIR         := $(shell pwd)
BUILD_DIR      := $(CURDIR)/BUILD
LIB_DIR        := $(CURDIR)/libBitIO/src
UTILITY_DIR    := $(CURDIR)/Test-BitIO
BUILD_LIB      := $(BUILD_DIR)/libBitIO
BUILD_UTILITY  := $(BUILD_DIR)/Test-BitIO

.PHONY: all install uninstall clean

LIB_OBJ_FILES := \
	$(BUILD_LIB)/BitIO.o   \
	$(BUILD_LIB)/Deflate.o \
	$(BUILD_LIB)/MD5.o

LIB_SRC_FILES := \
	$(LIB_DIR)/BitIO.c   \
	$(LIB_DIR)/Deflate.c \
	$(LIB_DIR)/MD5.c

$(BUILD_LIB)/libBitIO.a: $(LIB_OBJ_FILES)
	mkdir -p $(BUILD_LIB)
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)
	libtool -current_version $(VERSION) -o $(BUILD_LIB)/libBitIO.a $(BUILD_LIB)*.o

$(LIB_OBJ_FILES) : $(LIB_SRC_FILES)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $^

$(UTILITY_DIR)/UnitTest.o:
	mkdir -p $(BUILD_UTILITY)
	$(CC) -c $(UTILITY_DIR)/UnitTest.c -o $(UTILITY_DIR)/UnitTest.o $(CFLAGS) $(LDFLAGS)

$(BUILD_UTILITY)/Test-BitIO: $(UTILITY_DIR)/UnitTest.o
	mkdir -p $(BUILD_UTILITY)
	$(CC) -c $(UTILITY_DIR)/UnitTest.c -o $(UTILITY_DIR)/UnitTest.o $(CFLAGS) $(LDFLAGS)

all: $(BUILD_LIB)/libBitIO.a $(BUILD_UTILITY)/Test-BitIO
