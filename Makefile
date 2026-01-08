LIB=libdastruct
VERSION=0.0.1

CC=gcc
CFLAGS=-Wall -Werror -g -D_REENTRANT
STRIPPED=

SRC_DIR=src
TST_DIR=tests
TST_BIN_DIR=$(TST_DIR)/bin
OBJ_DIR=obj
BIN_DIR=bin
RELEASE_DIR=release-$(VERSION)

# Source files
SRCS=$(wildcard $(SRC_DIR)/*.c)
TST_SRCS=$(wildcard $(TST_DIR)/*.c)
HDRS=$(wildcard $(SRC_DIR)/*.h)

# Object files
OBJS=$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Test bins
TST_BINS=$(patsubst $(TST_DIR)/%.c, $(TST_BIN_DIR)/%, $(TST_SRCS))

# Targets
SO=$(BIN_DIR)/$(LIB).so
A=$(BIN_DIR)/$(LIB).a
TARGETS=$(SO) $(A)

# Test files
WORDS=words
S_WORDS=words-short
R_WORDS=random-words

MEMTEST_UTIL=valgrind

.PHONY: all test clean release

all: $(TARGETS) test

release: CFLAGS=-Wall -O2 -DNDEBUG -D_REENTRANT
release: STRIPPED=-s
release: clean $(SO) $(A) $(RELEASE_DIR)
	cp $(HDRS) $(SO) $(A) $(RELEASE_DIR)

package: release
	tar -czvf $(LIB)-$(VERSION).tar.gz $(RELEASE_DIR)

# Shared library depends on object files and bin directory
$(SO): $(OBJS) | $(BIN_DIR)
	$(CC) $(STRIPPED) $(CFLAGS) -shared -fPIC $(OBJS) -o $@

# Static library depends on object files and bin directory
$(A): $(OBJS) | $(BIN_DIR)
	ar -cvrs $@ $(OBJS)

# Object files depends on source c-files, header files and bin directory
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HDRS) | $(OBJ_DIR) 
	$(CC) $(CFLAGS) -c $< -o $@ -I$(SRC_DIR)

test: $(SO) $(TST_BINS)
	for test in $(TST_BINS) ; do ./$$test --verbose -j1 ; done

# Test bins  depends on source test c-files and bin directory
$(TST_BIN_DIR)/%: $(TST_DIR)/%.c | $(TST_BIN_DIR)
	$(CC) $(CFLAGS) $< $(OBJS) -o $@ -lcriterion

# Test bin directory creation
$(TST_BIN_DIR):
	mkdir -p ./$@

# Bin directory creation
$(OBJ_DIR):
	mkdir -p ./$@

# Bin directory creation
$(BIN_DIR):
	mkdir -p ./$@

# Release directory creation
$(RELEASE_DIR):
	mkdir -p ./$@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(RELEASE_DIR) $(TST_BIN_DIR) *.gz
