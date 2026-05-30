CC      = gcc
CFLAGS  = -Wall -I include
AR      = ar
ARFLAGS = rcs

SRC_DIR = src
OBJ_DIR = obj
LIB_DIR = lib
APP_DIR = app

SRCS   = $(wildcard $(SRC_DIR)/*.c)
OBJS   = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
LIB    = $(LIB_DIR)/libmathutils.a
TARGET = main

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(APP_DIR)/main.c $(LIB)
	$(CC) $(CFLAGS) -o $@ $< -L$(LIB_DIR) -lmathutils

$(LIB): $(OBJS) | $(LIB_DIR)
	$(AR) $(ARFLAGS) $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

$(LIB_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR) $(TARGET)

run: all
	./$(TARGET)
