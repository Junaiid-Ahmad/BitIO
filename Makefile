.POSIX:
.SUFFIXES:
.PHONY: all clean BUILD_DIR LIBBITIO_SOURCE_FLDR LIBBITIO_STATICLIB LIBBITIO_SOURCES LIBBITIO_NAMES LIBBITIO_OBJECTS

CC                    = $(shell whereis cc)
CURDIR                = $(shell pwd)
DEBUG_CFLAGS          = -DDEBUG -fsanitize="address,memory,leak,undefined" -Werror="format-security,array-bounds" -Wformat -g -O0
FUZZ_CFLAGS           = -fsanitize=fuzzer -g -O0
RELEASE_CFLAGS        = -DNODEBUG -fshort-enums -vectorize-loops -funroll-loops -Os
CFLAGS               += -std=c11 -ferror-limit 1024 -Weverything -fcolor-diagnostics -fno-gnu-inline-asm -fconst-strings $(BUILDTYPE_CFLAGS) -arch=$(ARCH)
LDFLAGS              += -flto
LDLIBS                = -lm
BUILD_DIR             = $(CURDIR)/BUILD/$(BUILDTYPE)/$(ARCH)
LIBBITIO_SOURCE_FLDR  = $(CURDIR)/libBitIO/src
LIBBITIO_STATICLIB    = $(BUILD_DIR)/libBitIO.a
LIBBITIO_SOURCES      = $(wildcard $(CURDIR)/libBitIO/src/*.c)
LIBBITIO_NAMES        = $(notdir $(LIBBITIO_SOURCES))
LIBBITIO_OBJECTS      = $(patsubst $(notdir $(LIBBITIO_SOURCES)), $(BUILD_DIR)/%.o, $(LIBBITIO_SOURCES))

#ifeq ($(BYTEORDER),$(filter $(BYTEORDER),LittleEndian,LSByte))
#	LIBBITIO_BYTEORDER = 1
#elif ($(BYTEORDER),$(filter $(BYTEORDER),BigEndian,MSByte))
#	LIBBITIO_BYTEORDER = 2
#else
#	LIBBITIO_BYTEORDER = 1 # Default to LSByte
#endif
#ifeq ($(BITORDER),LSBit)
#	LIBBITIO_BITORDER = 1
#elif ($(BITORDER),MSBit)
#	LIBBITIO_BITORDER = 2
#else
#	LIBBITIO_BITORDER = 1 #Default to LSBit
#endif

# wait why are we doing it this way? why not just extract it from the arch triple?

all : $(wildcard BUILD_DIR/%.d)

$(wildcard BUILD_DIR/%.d) : $(LIBBITIO_SOURCE_FLDR/*.c)
	mkdir -p $(BUILD_DIR)
	$(CC) -MT -o $@ -c $<
$(BUILD_DIR)/$(LIBBITIO_NAMES.o) : $(BUILD_DIR)/%.d
	$(CC) $(CFLAGS) -o $@ -c $< $(LDFLAGS) $(LDLIBS)
LIBBITIO_STATICLIB(wildcard *.o) : $(BUILD_DIR)/$(LIBBITIO_NAMES.o)
	ar -c $(LIBBITIO_STATICLIB)
	ar -crsu $@ $<
	ranlib -sf $@
