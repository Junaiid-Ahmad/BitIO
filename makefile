PACKAGE_NAME    := BitIO
VERSION         := 1.0
CC              := clang
FUZZER          := /Users/Marcus/Downloads/afl-2.12b/afl-clang
DESTINATION     := /usr/local/Packages/$(PACKAGE_NAME)
BUILD_DIR       := $(CURDIR)/BUILD
CFLAGS          := -std=c11 -march=native
LDFLAGS         := -lm -flto
DEB_FLAGS       := $(CFLAGS) -g -o0 -Weverything -Wunreachable-code $(LDFLAGS)
REL_FLAGS       := $(CFLAGS) -Weverything -Wunreachable-code -ofast $(LDFLAGS)
LIB_EXTENSION   := .a
OBJ_EXTENSION   := .o

.PHONY: clean fuzz install uninstall release debug

all: release

detect_platform:
	ifeq ($(OS),(Darwin|FreeBSD|OpenBSD|GhostBSD|PC-BSD))
		$(CC)             := clang
		$(LIB_EXTENSION)  := .a
		$(OBJ_EXTENSION)  := .o
	elif ($(OS),Linux)
		$(CC)             := gcc
		$(LIB_EXTENSION)  := .a
		$(OBJ_EXTENSION)  := .o
	elif ($(OS),(Windows_NT|CYGWIN_NT|MINGW32_NT|MSYS_NT))
		$(CC)             := msvc.exe
		$(EXECUTABLE_EXT) := .exe
		$(LIB_EXTENSION)  := .lib
		$(OBJ_EXTENSION)  := .obj
		$(DESTINATION)    := %ProgramFiles%\$(PACKAGE_NAME)
	endif

release:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/libBitIO
	mkdir -p $(BUILD_DIR)/test
	$(CC) $(REL_FLAGS) -c $(CURDIR)/libBitIO/src/BitIO.c -o $(BUILD_DIR)/libBitIO/libBitIO.o
	$(CC) $(REL_FLAGS) -c $(CURDIR)/test/test.c -o $(BUILD_DIR)/test/main.o
	ar -crsu $(BUILD_DIR)/libBitIO/libBitIO$(LIB_EXTENSION) $(BUILD_DIR)/libBitIO/libBitIO.o
	$(CC) -o $(BUILD_DIR)/test/Test-BitIO $(BUILD_DIR)/test/main.o $(BUILD_DIR)/libBitIO/libBitIO$(LIB_EXTENSION)
	strip $(BUILD_DIR)/test/Test-BitIO
debug:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/libBitIO
	mkdir -p $(BUILD_DIR)/test
	$(CC) $(DEB_FLAGS) -c $(CURDIR)/libBitIO/src/BitIO.c -o $(BUILD_DIR)/libBitIO/libBitIO.o
	$(CC) $(DEB_FLAGS) -c $(CURDIR)/test/test.c -o $(BUILD_DIR)/test/main.o
	$(CC) -o $(BUILD_DIR)/test/Test-BitIO $(BUILD_DIR)/test/main.o $(BUILD_DIR)/libBitIO/libBitIO.o
	ar -crsu $(BUILD_DIR)/libBitIO$(LIB_EXTENSION) $(BUILD_DIR)/libBitIO.o
fuzz:
	mkdir -p $(BUILD_DIR)
	$(FUZZER) $(DEB_FLAGS) -c $(CURDIR)/src/BitIO.c -o $(BUILD_DIR)/test/Test-BitIO.o
	ar -crsu $(BUILD_DIR)/BitIO$(LIB_EXTENSION) $(BUILD_DIR)/BitIO.o
install:
	install -d -m 777 $(DESTINATION)/lib
	install -d -m 777 $(DESTINATION)/bin
	install -d -m 777 $(DESTINATION)/include
	install -C -v -m 444 $(BUILD_DIR)/libBitIO/libBitIO$(LIB_EXTENSION) $(DESTINATION)/lib/BitIO$(LIB_EXTENSION)
	install -C -v -m 444 $(CURDIR)/libBitIO/include/BitIO.h $(DESTINATION)/include/BitIO.h
	install -C -v -m 444 $(BUILD_DIR)/BitIO/BitIO $(DESTINATION)/bin/BitIO
	chmod +x $(DESTINATION)/bin/BitIO
	ln -i $(DESTINATION)/bin/BitIO /usr/bin/BitIO
uninstall:
	rmdir -f $(DESTINATION)
clean:
	rm -f -v -r $(BUILD_DIR)/test/test$(OBJ_EXTENSION)
	rm -f -v -r $(BUILD_DIR)/test/Test-BitIO
	rm -f -v -r $(BUILD_DIR)/test/.DS_Store
	rm -f -v -r $(BUILD_DIR)/test/Thumbs.db
	rm -f -v -r $(BUILD_DIR)/libBitIO/libBitIO.o
	rm -f -v -r $(BUILD_DIR)/libBitIO/libBitIO.a
	rm -f -v -r $(BUILD_DIR)/libBitIO/.DS_Store
	rm -f -v -r $(BUILD_DIR)/libBitIO/Thumbs.db
	rm -f -v -r $(BUILD_DIR)/.DS_Store
	rm -f -v -r $(BUILD_DIR)/Thumbs.db
	rmdir $(BUILD_DIR)/test
	rmdir $(BUILD_DIR)/libBitIO
	rmdir $(BUILD_DIR)
