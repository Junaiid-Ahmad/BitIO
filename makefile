PACKAGE_NAME        := libBitIO
CC                  := $(shell whereis cc)
CURDIR              := $(shell "pwd")
DEBUG_CFLAGS        := -DDEBUG -fsanitize="address,undefined" -Wformat -Werror=format-security -Werror=array-bounds -g -O0
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

BUILD_DIR            = $(CURDIR)/BUILD/$(ARCH)/$(BUILDTYPE)
LIBBITIO_SOURCES    := $(wildcard CURDIR/libBitIO/src/*.c)
LIBBITIO_HEADERS    := $(wildcard CURDIR/libBitIO/include/*.h)
LIBBITIO_OBJECTS    := $(patsubst %.c,%.o,$(LIBBITIO_SOURCES))
LIBBITIO_DEPENDS    := $(patsubst %.c,%.d,$(LIBBITIO_SOURCES))
LIBBITIO_STATICLIB  := $(BUILD_DIR)/libBitIO.a

.DEFAULT_GOAL: all

.PHONY: all clean

all: $(LIBBITIO_SOURCES) $(LIBBITIO_STATICLIB)

$(LIBBITIO_DEPENDS): $(LIBBITIO_SOURCES) $(LIBBITIO_HEADERS)
	$(CC) $(LIBBITIO_SOURCES) $(LIBBITIO_HEADERS) -c $< -o $@

$(LIBBITIO_OBJECTS): $(LIBBITIO_SOURCES) $(LIBBITIO_HEADERS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(LIBBITIO_SOURCES) -o $(LIBBITIO_OBJECTS) $(LDFLAGS) -lm

$(LIBBITIO_STATICLIB): $(LIBBITIO_OBJECTS)
	ar -crsu $@ $<
	ranlib -sf $@

-include $(LIBBITIO_DEPENDS)

clean: $(LIBBITIO_STATICLIB) $(LIBBITIO_OBJECTS)
	rm -f -v -r $(LIBBITIO_OBJECTS)
	rm -f -v -r *.a
	rm -f -v -r .DS_Store
	rm -f -v -r Thumbs.db
	rm -f -v -r desktop.ini
	rmdir $(BUILD_DIR)
