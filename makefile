PACKAGE_NAME    := BitIO
FILE			:= $(CURDIR)/libBitIO/include/BitIO.h
VERSION         := $(shell cat ${FILE} | grep -e "@version")
CC              := cc
DESTINATION     := /usr/local/Packages/$(PACKAGE_NAME)
BUILD_DIR       := $(CURDIR)/BUILD
CFLAGS          := -std=c11 -march=native
LDFLAGS         := -flto
ERROR_OPTIONS   := -Weverything -Wunreachable-code
DEB_FLAGS       := $(CFLAGS) -g -o0 -fsanitize=undefined $(ERROR_OPTIONS) $(LDFLAGS)
REL_FLAGS       := $(CFLAGS) -ofast $(ERROR_OPTIONS) $(LDFLAGS)
EXE_EXT         :=
LIB_EXT         := a
OBJ_EXT         := o

.PHONY: all detect_platform Release Debug Install Uninstall Clean distclean

all: release
	$(release)
check: 
	$(test)
distclean: 
	$(clean)

CHECK_VERS:
	$(shell echo ${VERSION})


libBitIO.o: $(CURDIR)/libBitIO/src/BitIO.c $(CURDIR)/libBitIO/include/BitIO.h



release: $(CURDIR)/libBitIO/src/BitIO.c
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/libBitIO
	$(CC) $(REL_FLAGS) -c $(CURDIR)/libBitIO/src/BitIO.c -o $(BUILD_DIR)/libBitIO/libBitIO.$(OBJ_EXT)
	ar -crsu $(BUILD_DIR)/libBitIO/libBitIO.$(LIB_EXT) $(BUILD_DIR)/libBitIO/libBitIO.$(OBJ_EXT)

debug: $(CURDIR)/libBitIO/src/BitIO.c
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/libBitIO
	$(CC) $(DEB_FLAGS) -c $(CURDIR)/libBitIO/src/BitIO.c -o $(BUILD_DIR)/libBitIO/libBitIO.$(OBJ_EXT)
	ar -crsu $(BUILD_DIR)/libBitIO/libBitIO.$(LIB_EXT) $(BUILD_DIR)/libBitIO/libBitIO.$(OBJ_EXT)

test: $(CURDIR)/libBitIO/src/BitIO.c $(CURDIR)/test/UnitTest.c
	$(Debug)
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

