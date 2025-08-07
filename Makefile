# Directory
INCDIR   = include
SRCDIR   = src
FILTERS  = $(SRCDIR)/filters

# Tools
CC       = gcc
CFLAGS   = -Wall -Wextra -Werror -O2 -std=c11 -I$(INCDIR) -I$(FILTERS)
LDFLAGS  = -lpng -lz 

# Sources
SRC = $(SRCDIR)/main.c $(FILTERS)/lanczos.c
OBJ = $(SRC:.c=.o)
DEP = $(OBJ:.o=.d)

# Target
TARGET = ascii_wallpaper

# Rules
.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

# Compilation + dependencies  
%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# Include eventual .d files
-include $(DEP)

clean:
	rm -f $(TARGET) $(OBJ) $(DEP)
