PACKAGE_NAME    := BitIO
VERSION         := 1.0
CC              := clang
DESTINATION     := /usr/local/Packages/$(PACKAGE_NAME)
BUILD_DIR       := $(CURDIR)/BUILD
CFLAGS          := -std=c11 -march=native
LDFLAGS         := -lm -flto
DEB_FLAGS       := $(CFLAGS) -g -o0 -Weverything -Wunreachable-code $(LDFLAGS)
REL_FLAGS       := $(CFLAGS) -Weverything -Wunreachable-code -ofast $(LDFLAGS)
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
	mkdir -p $(BUILD_DIR)/test
	$(CC) $(REL_FLAGS) -c $(CURDIR)/libBitIO/src/BitIO.c -o $(BUILD_DIR)/libBitIO/libBitIO.$(OBJ_EXT)
	$(CC) $(REL_FLAGS) -c $(CURDIR)/test/test.c -o $(BUILD_DIR)/test/main.$(OBJ_EXT)
	ar -crsu $(BUILD_DIR)/libBitIO/libBitIO.$(LIB_EXT) $(BUILD_DIR)/libBitIO/libBitIO.$(OBJ_EXT)
	$(CC) -o $(BUILD_DIR)/test/Test-BitIO$(EXE_EXT) $(BUILD_DIR)/test/main.$(OBJ_EXT) $(BUILD_DIR)/libBitIO/libBitIO.$(LIB_EXT)
	strip $(BUILD_DIR)/test/Test-BitIO$(EXE_EXT)
debug:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/libBitIO
	mkdir -p $(BUILD_DIR)/test
	$(CC) $(DEB_FLAGS) -c $(CURDIR)/libBitIO/src/BitIO.c -o $(BUILD_DIR)/libBitIO/libBitIO.$(OBJ_EXT)
	$(CC) $(DEB_FLAGS) -c $(CURDIR)/test/test.c -o $(BUILD_DIR)/test/main.$(OBJ_EXT)
	$(CC) -o $(BUILD_DIR)/test/Test-BitIO$(EXE_EXT) $(BUILD_DIR)/test/main.$(OBJ_EXT) $(BUILD_DIR)/libBitIO/libBitIO.$(OBJ_EXT)
	ar -crsu $(BUILD_DIR)/libBitIO.$(LIB_EXT) $(BUILD_DIR)/libBitIO.$(OBJ_EXT)
install:
	install -d -m 777 $(DESTINATION)/lib
	install -d -m 777 $(DESTINATION)/bin
	install -d -m 777 $(DESTINATION)/include
	install -C -v -m 444 $(BUILD_DIR)/libBitIO/libBitIO.$(LIB_EXT) $(DESTINATION)/lib/libBitIO.$(LIB_EXT)
	install -C -v -m 444 $(BUILD_DIR)/../libBitIO/include/BitIO.h $(DESTINATION)/include/BitIO.h
	install -C -v -m 444 $(BUILD_DIR)/test/Test-BitIO$(EXE_EXT) $(DESTINATION)/bin/Test-BitIO$(EXE_EXT)
	chmod +x $(DESTINATION)/bin/Test-BitIO$(EXE_EXT)
	ln -i $(DESTINATION)/bin/Test-BitIO$(EXE_EXT) /usr/bin/Test-BitIO
	chmod +x /usr/bin/Test-BitIO$(EXE_EXT)
uninstall:
	rmdir -f $(DESTINATION)
clean:
	rm -f -v -r $(BUILD_DIR)/test/test$(OBJ_EXT)
	rm -f -v -r $(BUILD_DIR)/test/Test-BitIO$(EXE_EXT)
	rm -f -v -r $(BUILD_DIR)/test/.DS_Store
	rm -f -v -r $(BUILD_DIR)/test/Thumbs.db
	rm -f -v -r $(BUILD_DIR)/libBitIO/libBitIO.$(OBJ_EXT)
	rm -f -v -r $(BUILD_DIR)/libBitIO/libBitIO.$(LIB_EXT)
	rm -f -v -r $(BUILD_DIR)/libBitIO/.DS_Store
	rm -f -v -r $(BUILD_DIR)/libBitIO/Thumbs.db
	rm -f -v -r $(BUILD_DIR)/.DS_Store
	rm -f -v -r $(BUILD_DIR)/Thumbs.db
	rmdir $(BUILD_DIR)/test
	rmdir $(BUILD_DIR)/libBitIO
	rmdir $(BUILD_DIR)
