PACKAGE_NAME        := BitIO
FILE                := $(CURDIR)/include/BitIO.h
VERSION             := $(shell cat ${FILE} | grep -e "@version")
CC                  := cc
DESTINATION         := /usr/local/Packages/$(PACKAGE_NAME)
BUILD_DIR           := $(CURDIR)/BUILD
CFLAGS              := -std=c11 -march=native
LDFLAGS             := -flto=thin
DEB_ERROR_OPTIONS   := -Wno-unused-parameter -Wno-unused-variable -Wno-int-conversion
REL_ERROR_OPTIONS   := -Weverything -Wunreachable-code -Wno-conversion
DEB_FLAGS           := $(CFLAGS) -g -o0 $(DEB_ERROR_OPTIONS) $(LDFLAGS)
SANITIZER           := -fsanitize=undefined -fsanitize=address
REL_FLAGS           := $(CFLAGS) -ofast $(REL_ERROR_OPTIONS) $(LDFLAGS)

.PHONY: all check distclean CheckVer release debug test install clean uninstall

all: release
	$(release)
check: test
	$(test)
distclean: clean
	$(clean)
CheckVer:
	$(shell echo ${VERSION})
release: $(CURDIR)/src/BitIO.c
	mkdir -p   $(BUILD_DIR)
	mkdir -p   $(BUILD_DIR)/libBitIO
	$(CC)      $(REL_FLAGS) -c $(CURDIR)/src/BitIO.c -o $(BUILD_DIR)/libBitIO/BitIO.o
	$(CC)      $(REL_FLAGS) -c $(CURDIR)/src/UUID.c -o $(BUILD_DIR)/libBitIO/UUID.o
	ar -crsu   $(BUILD_DIR)/libBitIO/libBitIO.a $(BUILD_DIR)/libBitIO/BitIO.o $(BUILD_DIR)/libBitIO/UUID.o 
	ranlib -sf $(BUILD_DIR)/libBitIO/libBitIO.a
debug: $(CURDIR)/src/BitIO.c
	mkdir -p   $(BUILD_DIR)
	mkdir -p   $(BUILD_DIR)/libBitIO
	$(CC)      $(DEB_FLAGS) -c $(CURDIR)/src/BitIO.c -o $(BUILD_DIR)/libBitIO/BitIO.o
	$(CC)      $(DEB_FLAGS) -c $(CURDIR)/src/UUID.c -o $(BUILD_DIR)/libBitIO/UUID.o
	ar -crsu   $(BUILD_DIR)/libBitIO/libBitIO.a $(BUILD_DIR)/libBitIO/BitIO.o $(BUILD_DIR)/libBitIO/UUID.o
	ranlib -sf $(BUILD_DIR)/libBitIO/libBitIO.a
test: $(CURDIR)/test/UnitTest.c
	mkdir -p   $(BUILD_DIR)
	mkdir -p   $(BUILD_DIR)/test
	$(CC)      $(DEB_FLAGS) -c $(CURDIR)/test/UnitTest.c -o $(BUILD_DIR)/test/UnitTest.o
	$(CC)      $(BUILD_DIR)/test/UnitTest.o $(BUILD_DIR)/libBitIO/libBitIO.a -o $(BUILD_DIR)/test/Test-BitIO
install:
	install -d -m 777 $(DESTINATION)/lib
	install -d -m 777 $(DESTINATION)/bin
	install -d -m 777 $(DESTINATION)/include
	install -C -v -m 444 $(BUILD_DIR)/libBitIO/libBitIO.a $(DESTINATION)/lib/libBitIO.a
	install -C -v -m 444 $(BUILD_DIR)/../include/BitIO.h $(DESTINATION)/include/BitIO.h
	install -C -v -m 444 $(BUILD_DIR)/test/Test-BitIO $(DESTINATION)/bin/Test-BitIO
	chmod +x $(DESTINATION)/bin/Test-BitIO
	ln -i $(DESTINATION)/bin/Test-BitIO /usr/bin/Test-BitIO
	chmod +x /usr/bin/Test-BitIO
clean:
	cd $(BUILD_DIR)/libBitIO/
	rm -f -v -r BitIO.o
	rm -f -v -r UUID.o
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
uninstall:
	rm -d -i $(DESTINATION)
