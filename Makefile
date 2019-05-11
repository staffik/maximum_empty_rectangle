CC=g++
CC_FLAGS=-std=c++14 -g

SRC_DIR=./src
BIN_DIR=./bin
TMP_DIR=./tmp
TEST_DIR=./tests

SRCS=$(wildcard $(SRC_DIR)/*.cpp)
BINS=$(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%, $(SRCS))

all: prepare $(BINS) $(SRC_DIR)/common.h

$(BIN_DIR)/%: $(SRC_DIR)/%.cpp
	$(CC) $(CC_FLAGS) $< -o $@

.PHONY: prepare clean

prepare:
	mkdir -p $(BIN_DIR)
clean:
	rm -r $(BIN_DIR)
