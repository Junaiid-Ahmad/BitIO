PACKAGE_NAME        := BitIO
CC                  := cc
CFLAGS              := -std=c11 -march=native -ferror-limit=1024 -Wall -pedantic
LDFLAGS             := -flto=thin
BUILD_DIR           := $(CURDIR)/BUILD
LIBBITIO_SOURCE_DIR := $(CURDIR)/libBitIO/src
LIBBITIO_HEADER_DIR := $(CURDIR)/libBitIO/include
LIBBITIO_OBJECT_DIR := $(BUILD_DIR)/libBitIO

LIBBITIO_SOURCE_FILES := \
$(LIBBITIO_SOURCE_DIR)/BitIO.c \
$(LIBBITIO_SOURCE_DIR)/Deflate.c \
$(LIBBITIO_SOURCE_DIR)/MD5.c

LIBBITIO_HEADER_FILES := \
$(LIBBITIO_HEADER_DIR)/BitIO.h \
$(LIBBITIO_HEADER_DIR)/Deflate.h \
$(LIBBITIO_HEADER_DIR)/MD5.h

LIBBITIO_ARCHIVE_FILES := \
$(LIBBITIO_OBJECT_DIR)/BitIO.o \
$(LIBBITIO_OBJECT_DIR)/Deflate.o \
$(LIBBITIO_OBJECT_DIR)/MD5.o

.PHONY: all release debug clean

$(LIBBITIO_OBJECT_DIR)/%.o: $(LIBBITIO_SOURCE_DIR)/%.c $(LIBBITIO_HEADER_DIR)/%.h
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

$(LIBBITIO_OBJECT_DIR)/libBitIO.a: $(LIBBITIO_ARCHIVE_FILES)
	ar -crsu $@ $<
	ranlib -sf $@

release: CFLAGS += -DNODEBUG -funroll-loops -Ofast; $(all)
debug: CFLAGS += -DDEBUG -g -O0; $(all)

all:
	mkdir -p $(LIBBITIO_OBJECT_DIR)
	$(LIBBITIO_OBJECT_DIR/%.o)
	$(LIBBITIO_OBJECT_DIR/libBitIO.a)
clean:
	cd $(LIBBITIO_OBJECT_DIR)/
	rm -f -v -r *.o
	rm -f -v -r *.a
	rm -f -v -r .DS_Store
	rm -f -v -r Thumbs.db
	rm -f -v -r desktop.ini
	rmdir $(BUILD_DIR)
