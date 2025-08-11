CC       = gcc
CFLAGS   = -Wall -Wextra -Werror -O2 -std=c11 -Iinclude -Isrc/filters
LDFLAGS  = -lpng -lz

SRCDIR   = src
FILTERS  = $(SRCDIR)/filters

SRCS := $(wildcard $(SRCDIR)/*.c $(FILTERS)/*.c)
OBJS := $(SRCS:.c=.o)
DEPS := $(OBJS:.o=.d)

TARGET = ascii_wallpaper

.PHONY: all clean
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

# compila + genera file .d accanto ai .o
%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -MF $(@:.o=.d) -c $< -o $@

-include $(DEPS)

clear:
	rm -f $(TARGET) $(OBJS) $(DEPS)

