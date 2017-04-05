PACKAGE_NAME        := BitIO
CC                  := cc
CFLAGS              := -std=c11 -ferror-limit=1024 -Wall -pedantic
LDFLAGS             := -flto=thin
BUILD_DIR           := $(CURDIR)/BUILD
LIBBITIO_SOURCE_DIR := $(CURDIR)/libBitIO/src
LIBBITIO_HEADER_DIR := $(CURDIR)/libBitIO/include
LIBBITIO_OBJECT_DIR := $(BUILD_DIR)/libBitIO

LIBBITIO_ARCHIVE_FILES := \
$(LIBBITIO_OBJECT_DIR)/BitIO.o \
$(LIBBITIO_OBJECT_DIR)/Deflate.o \
$(LIBBITIO_OBJECT_DIR)/MD5.o

.PHONY: all release debug clean

$(LIBBITIO_OBJECT_DIR)/%.o: $(LIBBITIO_SOURCE_DIR)/%.c $(LIBBITIO_HEADER_DIR)/%.h
	mkdir -p $(LIBBITIO_OBJECT_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

$(LIBBITIO_OBJECT_DIR)/libBitIO.a: $(LIBBITIO_ARCHIVE_FILES)
	ar -crsu $@ $<
	ranlib -sf $@

release: CFLAGS += -DNODEBUG -funroll-loops -Ofast; $(all)
debug: CFLAGS += -DDEBUG -g -O0; $(all)

x86_64: CFLAGS += -march=x86_64; $(all)
arm64: CFLAGS += -march=aarch64l $(all)

all:
	mkdir -p $(LIBBITIO_OBJECT_DIR)
	$(LIBBITIO_OBJECT_DIR/libBitIO.a)
clean:
	cd $(LIBBITIO_OBJECT_DIR)/
	rm -f -v -r *.o
	rm -f -v -r *.a
	rm -f -v -r .DS_Store
	rm -f -v -r Thumbs.db
	rm -f -v -r desktop.ini
	rmdir $(BUILD_DIR)
