CC      = gcc
CFLAGS  = -Wall -fPIC -I include
AR      = ar
ARFLAGS = rcs
LDFLAGS = -L$(LIB_DIR) -lcalc -llogger

SRC_DIR = src
OBJ_DIR = obj
LIB_DIR = lib
BIN_DIR = bin

ALL_SRCS    = $(wildcard $(SRC_DIR)/*.c)
CALC_SRCS   = $(filter $(SRC_DIR)/calc_%.c, $(ALL_SRCS))
LOGGER_SRCS = $(filter $(SRC_DIR)/logger%.c, $(ALL_SRCS))
CALC_OBJS   = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(CALC_SRCS))
LOGGER_OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(LOGGER_SRCS))

STATIC_LIB = $(LIB_DIR)/libcalc.a
SHARED_LIB = $(LIB_DIR)/liblogger.so
TARGET     = $(BIN_DIR)/app

.PHONY: all libs static shared run clean rebuild

all: $(TARGET)

$(TARGET): $(SRC_DIR)/main.c $(STATIC_LIB) $(SHARED_LIB) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) -Wl,-rpath,'$$ORIGIN/../lib'

libs: static shared

static: $(STATIC_LIB)

shared: $(SHARED_LIB)

$(STATIC_LIB): $(CALC_OBJS) | $(LIB_DIR)
	$(AR) $(ARFLAGS) $@ $^

$(SHARED_LIB): $(LOGGER_OBJS) | $(LIB_DIR)
	$(CC) -shared -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

$(LIB_DIR):
	mkdir -p $@

$(BIN_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR) $(BIN_DIR) app.log

rebuild: clean all

run: all
	$(TARGET)
