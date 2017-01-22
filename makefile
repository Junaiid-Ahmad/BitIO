PACKAGE_NAME        := libBitIO
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
SANITIZER           := -fsanitize=undefined -fsanitize=address
REL_FLAGS           := $(CFLAGS) -ofast $(REL_ERROR_OPTIONS) $(LDFLAGS)
BUILD_LIB           := $(BUILD_DIR)/libBitIO

.PHONY: all check distclean CheckVer release debug test install clean uninstall

all: release
	$(release)
	$(test)
check: test
	$(test)
distclean: clean
	$(clean)
CheckVer:
	$(shell echo ${VERSION})
release: $(CURDIR)/libBitIO/src/BitIO.c
	mkdir -p   $(BUILD_LIB)
	$(CC)      $(REL_FLAGS) -c $(CURDIR)/libBitIO/src/BitIO.c -o $(BUILD_LIB)/BitIO.o
	$(CC)      $(REL_FLAGS) -c $(CURDIR)/libBitIO/src/UUID.c -o $(BUILD_LIB)/UUID.o
	$(CC)      $(REL_FLAGS) -c $(CURDIR)/libBitIO/src/Deflate.c -o $(BUILD_LIB)/Deflate.o
	$(CC)      $(REL_FLAGS) -c $(CURDIR)/libBitIO/src/UTF8String.c -o $(BUILD_LIB)/UTF8String.o
	$(CC)      $(REL_FLAGS) -c $(CURDIR)/libBitIO/src/MD5.c -o $(BUILD_LIB)/MD5.o
	ar -crsu   $(BUILD_LIB)/libBitIO.a $(BUILD_LIB)/*.o
	ranlib -sf $(BUILD_LIB)/libBitIO.a
	strip	   $(BUILD_LIB)/libBitIO.a
debug: $(CURDIR)/libBitIO/src/BitIO.c
	mkdir -p   $(BUILD_LIB)
	$(CC)      $(DEB_FLAGS) -c $(CURDIR)/libBitIO/src/BitIO.c -o $(BUILD_LIB)/BitIO.o
	$(CC)      $(DEB_FLAGS) -c $(CURDIR)/libBitIO/src/UUID.c -o $(BUILD_LIB)/UUID.o
	$(CC)      $(DEB_FLAGS) -c $(CURDIR)/libBitIO/src/Deflate.c -o $(BUILD_LIB)/Deflate.o
	ar -crsu   $(BUILD_LIB)/libBitIO.a $(BUILD_LIB)/*.o
	ranlib -sf $(BUILD_LIB)/libBitIO.a
test: $(CURDIR)/libBitIO/test/UnitTest.c
	mkdir -p   $(BUILD_DIR)/test
	$(CC)      $(DEB_FLAGS) -c $(CURDIR)/libBitIO/test/UnitTest.c -o $(BUILD_DIR)/test/UnitTest.o
	$(CC)      $(BUILD_DIR)/test/UnitTest.o $(BUILD_LIB)/libBitIO.a -o $(BUILD_DIR)/test/Test-BitIO
	strip      $(BUILD_DIR)/test/Test-BitIO
install:
	install -d -m 777 $(DESTINATION)/lib
	install -d -m 777 $(DESTINATION)/bin
	install -d -m 777 $(DESTINATION)/include
	install -C -v -m 444 $(BUILD_LIB)/libBitIO.a $(DESTINATION)/lib/libBitIO.a
	install -C -v -m 444 $(CURDIR)/libBitIO/include/BitIO.h $(DESTINATION)/include/BitIO.h
	install -C -v -m 444 $(CURDIR)/libBitIO/include/UUID.h $(DESTINATION)/include/UUID.h
	install -C -v -m 444 $(CURDIR)/libBitIO/include/UTF8String.h $(DESTINATION)/include/UTF8String.h
	install -C -v -m 444 $(CURDIR)/libBitIO/include/MD5.h $(DESTINATION)/include/MD5.h
	install -C -v -m 444 $(BUILD_DIR)/test/Test-BitIO $(DESTINATION)/bin/Test-BitIO
	chmod +x $(DESTINATION)/bin/Test-BitIO
	ln -i $(DESTINATION)/bin/Test-BitIO /usr/bin/Test-BitIO
	chmod +x /usr/bin/Test-BitIO
	install -C -v -m 444 $(CURDIR)/libBitIO.pc /usr/share/pkgconfig/libBitIO.pc
clean:
	cd $(BUILD_LIB)/
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
