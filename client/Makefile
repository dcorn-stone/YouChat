# Directories
BUILD_DIR = make
SRC_DIR   = src
INC_DIR   = inc

# Compiler and flags
CC      = gcc
CFLAGS  = -Wall -Wextra -O2 -g -I$(INC_DIR) -fPIC -I/opt/homebrew/include
LDFLAGS = -shared -L/opt/homebrew/lib -ljansson

# List all C source files in src folder
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Set library name according to platform
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
  LIB_EXT = dylib
else
  LIB_EXT = so
endif

TARGET = $(BUILD_DIR)/libclient.$(LIB_EXT)

all: $(TARGET)

$(TARGET): $(BUILD_DIR) $(OBJ_FILES)
	$(CC) $(LDFLAGS) $(OBJ_FILES) -o $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
