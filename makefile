PACKAGE_NAME        := BitIO
CC                  := cc
CFLAGS              := -std=c11 -ferror-limit=1024 -Wall -pedantic
LDFLAGS             := -flto=thin
BUILD_DIR           := $(CURDIR)/BUILD
BUILDFOLDER          = $(CURDIR)/BUILD
LIBBITIO_SOURCE_DIR := $(CURDIR)/libBitIO/src
LIBBITIO_HEADER_DIR := $(CURDIR)/libBitIO/include
LIBBITIO_OBJECT_DIR := $(BUILD_DIR)/libBitIO

default: all

LIBBITIO_ARCHIVE_FILES = \
$(LIBBITIO_OBJECT_DIR)/BitIO.o \
$(LIBBITIO_OBJECT_DIR)/Deflate.o \
$(LIBBITIO_OBJECT_DIR)/MD5.o

.PHONY: all debug release arm64 x86_64 clean

ifeq (1,$(debug))
	BUILDFOLDER    = $(BUILD_DIR)/debug
	BUILD_FLAGS    = $(CFLAGS) -DDEBUG -g -O0
elif (1,$(release))
	BUILDFOLDER    = $(BUILD_DIR)/release
	BUILD_FLAGS    = $(CFLAGS) -DNODEBUG -funroll-loops -Ofast
else
	BUILDFOLDER    = $(BUILD_DIR)/release
	BUILD_FLAGS    = $(CFLAGS) -DNODEBUG -funroll-loops -Ofast
endif

ifeq (1,$(arm64))
	BUILDFOLDER    = $(BUILD_DIR)/arm64
	BUILD_FLAGS    = $(CFLAGS) -march=aarch64
elif (1,$(x86_64))
	BUILDFOLDER    = $(BUILD_DIR)/x86_64
	BUILD_FLAGS    = $(CFLAGS) -march=x86_64
else
	BUILDFOLDER    = $(BUILD_DIR)/native
	BUILD_FLAGS    = $(CFLAGS) -march=native
endif

$(LIBBITIO_OBJECT_DIR)/%.o: $(LIBBITIO_SOURCE_DIR)/%.c $(LIBBITIO_HEADER_DIR)/%.h
	mkdir -p $(BUILDFOLDER)
	$(CC) $(BUILD_FLAGS) -c $< -o $@ $(LDFLAGS)

$(BUILDFOLDER)/libBitIO.a: $(LIBBITIO_ARCHIVE_FILES)
	ar -crsu $@ $(LIBBITIO_ARCHIVE_FILES)
	ranlib -sf $@

all:
	mkdir -p $(BUILDFOLDER)
	$(BUILDFOLDER/libBitIO.a)
clean:
	cd $(LIBBITIO_OBJECT_DIR)/
	rm -f -v -r *.o
	rm -f -v -r *.a
	rm -f -v -r .DS_Store
	rm -f -v -r Thumbs.db
	rm -f -v -r desktop.ini
	rmdir $(BUILDFOLDER)
