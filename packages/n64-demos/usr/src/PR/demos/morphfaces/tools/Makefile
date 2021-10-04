CC           := gcc
CXX          := g++
CFLAGS       := -I. -O2 -s
LDFLAGS      := -lm
ALL_PROGRAMS := medit2c

BUILD_PROGRAMS := $(ALL_PROGRAMS)

default: all

medit2c_SOURCES	:= medit2c.c

all: $(BUILD_PROGRAMS)

clean:
	$(RM) $(ALL_PROGRAMS)

define COMPILE
$(1): $($1_SOURCES)
	$$(CC) $(CFLAGS) $($1_CFLAGS) $$^ -o $$@ $($1_LDFLAGS) $(LDFLAGS)
endef

$(foreach p,$(BUILD_PROGRAMS),$(eval $(call COMPILE,$(p))))

.PHONY: all clean default
