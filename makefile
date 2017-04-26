PACKAGE_NAME        := BitIO
CC                  := $(shell cc)
CURDIR              := $(shell pwd)
CFLAGS               = -std=c11 -ferror-limit=1024 -Wall -pedantic
LDFLAGS             := -flto=thin
BUILD_DIR            = $(CURDIR)/BUILD
LIBBITIO_SOURCE_DIR := $(CURDIR)/libBitIO/src
LIBBITIO_SOURCES    := $(CURDIR)/libBitIO/src/%.c
LIBBITIO_HEADERS    := $(CURDIR)/libBitIO/include/%.h

.DEFAULT_GOAL: $(LIBBITIO_ARCHIVE)

.PHONY: all debug release arm64 x86_64 clean

debug:   $(CFLAGS)    += -DDEBUG -g -O0
release: $(CFLAGS)    += -DNODEBUG -fvectorize -loop-vectorize -funroll-loops -Ofast
arm64:   $(BUILD_DIR) += /arm64 : $(CFLAGS) += -march=arm64 $(all)
x86_64:  $(BUILD_DIR) += /x86_64 : $(CFLAGS) += -march=x86_64 $(all)

LIBBITIO_OBJECTS     = $(BUILD_DIR)/%.o
LIBBITIO_ARCHIVE     = $(BUILD_DIR)/libBitIO.a

$(LIBBITIO_OBJECTS): $(LIBBITIO_SOURCES) $(LIBBITIO_HEADERS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(BUILD_FLAGS) -c $< -o $@ $(LDFLAGS)

$(LIBBITIO_ARCHIVE): $(LIBBITIO_OBJECTS)
	ar -crsu $@ $<
	ranlib -sf $@

clean:
	rm -f -v -r $(LIBBITIO_OBJECTS)
	rm -f -v -r *.a
	rm -f -v -r .DS_Store
	rm -f -v -r Thumbs.db
	rm -f -v -r desktop.ini
	rmdir $(BUILD_DIR)
