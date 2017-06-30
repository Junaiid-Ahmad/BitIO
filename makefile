PACKAGE_NAME        := BitIO
CC                  := $(shell whereis cc)
CURDIR              := $(shell "pwd")
DEBUG_CFLAGS        := -DDEBUG -fsanitize=address,undefined -Wformat -Werror=format-security -Werror=array-bounds -g -O0
RELEASE_CFLAGS      := -DNODEBUG -fvectorize -loop-vectorize -funroll-loops -Os
UNAME_ARCH          := $(shell uname -m)

ifeq ($(BUILDTYPE), "")
BUILDTYPE = RELEASE
endif

ifeq ($(ARCH), "")
ARCH = $(UNAME_ARCH)
endif

CFLAGS              := -std=c11 -ferror-limit=1024 -Wall -pedantic -march=$(ARCH) $($(BUILDTYPE)_CFLAGS)
LDFLAGS             := -flto=thin

BUILD_DIR            = $(CURDIR)/BUILD/$(ARCH)
LIBBITIO_SOURCES    := $(wildcard CURDIR/libBitIO/src/*.c)
LIBBITIO_HEADERS    := $(wildcard CURDIR/libBitIO/include/*.h)
LIBBITIO_OBJECTS    := $(wildcard $(BUILD_DIR)/CURDIR/libBitIO/src/*.c:.o)
LIBBITIO_STATICLIB  := $(BUILD_DIR)/libBitIO.a

.DEFAULT_GOAL: $(all)

.PHONY: all clean

# OLD below

all: $(LIBBITIO_OBJECTS) $(LIBBITIO_STATICLIB)

$(LIBBITIO_STATICLIB): $(LIBBITIO_OBJECTS); $(LIBBITIO_HEADERS)
	ar -crsu $@ $<
	ranlib -sf $@

$(LIBBITIO_OBJECTS): $(LIBBITIO_SOURCES) $(LIBBITIO_HEADERS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS) -lm

clean: $(LIBBITIO_STATICLIB) $(LIBBITIO_OBJECTS)
	rm -f -v -r $(LIBBITIO_OBJECTS)
	rm -f -v -r *.a
	rm -f -v -r .DS_Store
	rm -f -v -r Thumbs.db
	rm -f -v -r desktop.ini
	rmdir $(BUILD_DIR)
