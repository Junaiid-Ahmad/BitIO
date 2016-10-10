PACKAGE_NAME        := BitIO
FILE                := $(CURDIR)/libBitIO/include/BitIO.h
VERSION             := $(shell cat ${FILE} | grep -e "@version")
CC                  := cc
DESTINATION         := /usr/local/Packages/$(PACKAGE_NAME)
BUILD_DIR           := $(CURDIR)/BUILD
CFLAGS              := -std=c11 -march=native
LDFLAGS             := -flto=thin
DEB_ERROR_OPTIONS   := -Wno-unused-parameter -Wno-unused-variable -Wno-int-conversion
REL_ERROR_OPTIONS   := -Weverything -Wunreachable-code -Wno-conversion
DEB_FLAGS           := $(CFLAGS) -g -o0 $(DEB_ERROR_OPTIONS) $(LDFLAGS)
SANITIZER           := -fsanitize=undefined
REL_FLAGS           := $(CFLAGS) -ofast $(REL_ERROR_OPTIONS) $(LDFLAGS)
EXE_EXT             :=
LIB_EXT             := a
OBJ_EXT             := o

.PHONY: all detect_platform Release Debug Install Uninstall Clean distclean

all: release
	$(release)
check: 
	$(test)
distclean: 
	$(clean)

CHECK_VERS:
	$(shell echo ${VERSION})

release: $(CURDIR)/libBitIO/src/BitIO.c
	mkdir -p   $(BUILD_DIR)
	mkdir -p   $(BUILD_DIR)/libBitIO
	$(CC)      $(REL_FLAGS) -c $(CURDIR)/libBitIO/src/BitIO.c -o $(BUILD_DIR)/libBitIO/libBitIO.o
	ar -crsu   $(BUILD_DIR)/libBitIO/libBitIO.a $(BUILD_DIR)/libBitIO/libBitIO.o

debug: $(CURDIR)/libBitIO/src/BitIO.c
	mkdir -p   $(BUILD_DIR)
	mkdir -p   $(BUILD_DIR)/libBitIO
	$(CC)      $(DEB_FLAGS) -c $(CURDIR)/libBitIO/src/BitIO.c -o $(BUILD_DIR)/libBitIO/libBitIO.o
	ar -crsu   $(BUILD_DIR)/libBitIO/libBitIO.a $(BUILD_DIR)/libBitIO/libBitIO.o
	ranlib -sf $(BUILD_DIR)/libBitIO/libBitIO.a

test: $(CURDIR)/libBitIO/test/UnitTest.c
	mkdir -p    $(BUILD_DIR)/test
	$(CC) -v -c $(CURDIR)/libBitIO/test/UnitTest.c -o $(BUILD_DIR)/test/UnitTest.o $(DEB_FLAGS)
	$(CC) -v    $(BUILD_DIR)/test/UnitTest.o -L$(BUILD_DIR)/libBitIO -o $(BUILD_DIR)/test/Test-BitIO

install:
	install -d -m 777 $(DESTINATION)/lib
	install -d -m 777 $(DESTINATION)/bin
	install -d -m 777 $(DESTINATION)/include
	install -C -v -m 444 $(BUILD_DIR)/libBitIO/libBitIO.a $(DESTINATION)/lib/libBitIO.a
	install -C -v -m 444 $(BUILD_DIR)/../libBitIO/include/BitIO.h $(DESTINATION)/include/BitIO.h
	install -C -v -m 444 $(BUILD_DIR)/test/Test-BitIO $(DESTINATION)/bin/Test-BitIO
	chmod +x $(DESTINATION)/bin/Test-BitIO
	ln -i $(DESTINATION)/bin/Test-BitIO /usr/bin/Test-BitIO
	chmod +x /usr/bin/Test-BitIO

uninstall:
	rm -d -i $(DESTINATION)

clean:
	cd $(BUILD_DIR)/libBitIO/
	rm -f -v -r libBitIO.o
	rm -f -v -r libBitIO.a
	rm -f -v -r .DS_Store
	rm -f -v -r Thumbs.db
	rm -f -v -r desktop.ini
	cd ../test/
	rm -f -v -r UnitTest.o
	rm -f -v -r Test-BitIO
	rm -f -v -r .DS_Store
	rm -f -v -r Thumbs.db
	rm -f -v -r desktop.ini
	rmdir $(BUILD_DIR)
