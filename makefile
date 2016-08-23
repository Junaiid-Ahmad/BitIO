PACKAGE_NAME    := BitIO
VERSION         := 1.0
CC              := cc
DESTINATION     := /usr/local/Packages/$(PACKAGE_NAME)
BUILD_DIR       := $(CURDIR)/BUILD
CFLAGS          := -std=c11 -march=native
LDFLAGS         := -flto
ERROR_OPTIONS	:= -Weverything -Wunreachable-code
DEB_FLAGS       := $(CFLAGS) -g -o0 $(ERROR_OPTIONS) $(LDFLAGS)
REL_FLAGS       := $(CFLAGS) -ofast $(ERROR_OPTIONS) $(LDFLAGS)
EXE_EXT         :=
LIB_EXT         := a
OBJ_EXT         := o

.PHONY: all detect_platform release debug install uninstall clean

all: release

detect_platform:
	ifeq ($(OS),(Darwin|FreeBSD|OpenBSD|GhostBSD|PC-BSD))
		$(CC)             := clang
		$(LIB_EXT)        := a
		$(OBJ_EXT)        := o
	elif ($(OS),Linux)
		$(CC)             := gcc
		$(LIB_EXT)        := a
		$(OBJ_EXT)        := o
	elif ($(OS),(Windows_NT|CYGWIN_NT|MINGW32_NT|MSYS_NT))
		$(CC)             := msvc.exe
		$(EXE_EXT)        := .exe
		$(LIB_EXT)        := lib
		$(OBJ_EXT)        := obj
		$(DESTINATION)    := %ProgramFiles%\$(PACKAGE_NAME)
	endif

release:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/libBitIO
	$(CC) $(REL_FLAGS) -c $(CURDIR)/libBitIO/src/BitIO.c -o $(BUILD_DIR)/libBitIO/libBitIO.$(OBJ_EXT)
	ar -crsu $(BUILD_DIR)/libBitIO/libBitIO.$(LIB_EXT) $(BUILD_DIR)/libBitIO/libBitIO.$(OBJ_EXT)

debug:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/libBitIO
	$(CC) $(DEB_FLAGS) -c $(CURDIR)/libBitIO/src/BitIO.c -o $(BUILD_DIR)/libBitIO/libBitIO.$(OBJ_EXT)
	ar -crsu $(BUILD_DIR)/libBitIO/libBitIO.$(LIB_EXT) $(BUILD_DIR)/libBitIO/libBitIO.$(OBJ_EXT)

test:
	mkdir -p $(BUILD_DIR)/test
	$(CC) $(DEB_FLAGS) -c $(CURDIR)/test/UnitTest.c -o $(BUILD_DIR)/test/UnitTest.$(OBJ_EXT)
	$(CC) $(BUILD_DIR)/test/UnitTest.$(OBJ_EXT) $(BUILD_DIR)/libBitIO/libBitIO.$(LIB_EXT) -o $(BUILD_DIR)/test/UnitTest$(EXE_EXT)

install:
	install -d -m 777 $(DESTINATION)/lib
	install -d -m 777 $(DESTINATION)/bin
	install -d -m 777 $(DESTINATION)/include
	install -C -v -m 444 $(BUILD_DIR)/libBitIO/libBitIO.$(LIB_EXT) $(DESTINATION)/lib/libBitIO.$(LIB_EXT)
	install -C -v -m 444 $(BUILD_DIR)/../libBitIO/include/BitIO.h $(DESTINATION)/include/BitIO.h
	install -C -v -m 444 $(BUILD_DIR)/BitIO/Test-BitIO$(EXE_EXT) $(DESTINATION)/bin/Test-BitIO$(EXE_EXT)
	chmod +x $(DESTINATION)/bin/Test-BitIO$(EXE_EXT)
	ln -i $(DESTINATION)/bin/Test-BitIO$(EXE_EXT) /usr/bin/Test-BitIO
	chmod +x /usr/bin/Test-BitIO$(EXE_EXT)

uninstall:
	rmdir -f $(DESTINATION)

clean:
	rm -f -v -r $(BUILD_DIR)/BitIO/BitIO$(OBJ_EXT)
	rm -f -v -r $(BUILD_DIR)/BitIO/Test-BitIO$(EXE_EXT)
	rm -f -v -r $(BUILD_DIR)/BitIO/.DS_Store
	rm -f -v -r $(BUILD_DIR)/BitIO/Thumbs.db
	rm -f -v -r $(BUILD_DIR)/libBitIO/libBitIO.$(OBJ_EXT)
	rm -f -v -r $(BUILD_DIR)/libBitIO/libBitIO.$(LIB_EXT)
	rm -f -v -r $(BUILD_DIR)/libBitIO/.DS_Store
	rm -f -v -r $(BUILD_DIR)/libBitIO/Thumbs.db
	rm -f -v -r $(BUILD_DIR)/.DS_Store
	rm -f -v -r $(BUILD_DIR)/Thumbs.db
	rmdir $(BUILD_DIR)/BitIO
	rmdir $(BUILD_DIR)/libBitIO
	rmdir $(BUILD_DIR)

