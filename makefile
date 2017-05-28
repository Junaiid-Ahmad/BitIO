PACKAGE_NAME        := BitIO
CC                  := $(shell cc)
CURDIR              := $(shell pwd)
DEBUG_CFLAGS        := -DDEBUG -g -O0
RELEASE_CFLAGS      := -DNODEBUG -fvectorize -loop-vectorize -funroll-loops -Os
CFLAGS              := -std=c11 -ferror-limit=1024 -Wall -pedantic -march=$(ARCH) $($(BUILDTYPE)_CFLAGS)
LDFLAGS             := -flto=thin
BUILD_DIR            = $(CURDIR)/BUILD
LIBBITIO_SOURCES    := $(wildcard CURDIR/libBitIO/src/*.c)
LIBBITIO_HEADERS    := $(wildcard CURDIR/libBitIO/include/*.h)
LIBBITIO_OBJECTS    := $(wildcard $(BUILD_DIR)/$(ARCH)/*.o)
LIBBITIO_STATIC     := $(BUILD_DIR)/$(ARCH)/libBitIO.a

.DEFAULT_GOAL: $(.all)

.PHONY: .all debug release arm64 x86_64 clean

# OLD below

.all: $(LIBBITIO_STATIC) $(LIBBITIO_OBJECTS)

$(LIBBITIO_OBJECTS): $(LIBBITIO_SOURCES) $(LIBBITIO_HEADERS)
	mkdir -p $(BUILD_DIR)/$(ARCH)
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

$(LIBBITIO_STATIC): $(LIBBITIO_OBJECTS) $(LIBBITIO_HEADERS)
	ar -crsu $@ $<
	ranlib -sf $@

clean:
	rm -f -v -r $(LIBBITIO_OBJECTS)
	rm -f -v -r *.a
	rm -f -v -r .DS_Store
	rm -f -v -r Thumbs.db
	rm -f -v -r desktop.ini
	rmdir $(BUILD_DIR)
