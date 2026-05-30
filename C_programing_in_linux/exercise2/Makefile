CC      = gcc
CFLAGS  = -Wall -fPIC -I include
LDFLAGS = -L$(BIN_DIR) -lstringutils

SRC_DIR   = src
BUILD_DIR = build
BIN_DIR   = bin

LIB_SRCS = $(filter-out $(SRC_DIR)/main.c, $(wildcard $(SRC_DIR)/*.c))
LIB_OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(LIB_SRCS))

LIB    = $(BIN_DIR)/libstringutils.so
TARGET = $(BIN_DIR)/app

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRC_DIR)/main.c $(LIB) | $(BIN_DIR)
	$(CC) -Wall -I include -o $@ $< $(LDFLAGS) -Wl,-rpath,'$$ORIGIN'

$(LIB): $(LIB_OBJS) | $(BIN_DIR)
	$(CC) -shared -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

$(BIN_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

run: all
	$(TARGET) "Hello World"
