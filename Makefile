LIB=libdastruct
VERSION=0.0.1

CC=gcc
CFLAGS=-Wall -Werror -g -D_REENTRANT
STRIPPED=

SRC_DIR=src
TST_DIR=test
OBJ_DIR=obj
BIN_DIR=bin
RELEASE_DIR=release-$(VERSION)

# Source files
SRCS=$(wildcard $(SRC_DIR)/*.c)
TST_SRCS=$(wildcard $(TST_DIR)/*.c)
HDRS=$(wildcard $(SRC_DIR)/*.h)

# Object files
OBJS=$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
TST_OBJS=$(patsubst $(TST_DIR)/%.c, $(OBJ_DIR)/t_%.o, $(TST_SRCS))

# Targets
SO=$(BIN_DIR)/$(LIB).so
A=$(BIN_DIR)/$(LIB).a
TEST_BIN=$(BIN_DIR)/test
TARGETS=$(SO) $(A) $(TEST_BIN)

# Test files
WORDS=words
S_WORDS=words-short
R_WORDS=random-words

MEMTEST_UTIL=valgrind

.PHONY: all test clean release

all: $(TARGETS)

release: CFLAGS=-Wall -O2 -DNDEBUG -D_REENTRANT
release: STRIPPED=-s
release: clean $(SO) $(A) | $(RELEASE_DIR)
	cp $(HDRS) $(SO) $(A) $(RELEASE_DIR)

package: release
	tar -czvf $(LIB)-$(VERSION).tar.gz $(RELEASE_DIR)

test: $(TEST_BIN) | $(WORDS) $(S_WORDS) $(R_WORDS)
	./$(TEST_BIN) $(S_WORDS) 500000
	./$(TEST_BIN) $(WORDS) 500000
	./$(TEST_BIN) $(R_WORDS) 500000

memtest: $(TEST_BIN) | $(R_WORDS)
	$(MEMTEST_UTIL) ./$(TEST_BIN) $(R_WORDS) 500000

# Shared library depends on object files and bin directory
$(SO): $(OBJS) | $(BIN_DIR)
	$(CC) $(STRIPPED) $(CFLAGS) -shared -fPIC $(OBJS) -o $@

# Static library depends on object files and bin directory
$(A): $(OBJS) | $(BIN_DIR)
	ar -cvrs $@ $(OBJS)

# Test executable depends on test object files, static library and bin directory
$(TEST_BIN): $(TST_OBJS) $(A) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(TST_OBJS) -o $@ $(A) -I$(SRC_DIR)

# Object files depends on source c-files, header files and bin directory
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HDRS) | $(OBJ_DIR) 
	$(CC) $(CFLAGS) -c $< -o $@ -I$(SRC_DIR)

# Test object files depends on source test c-files and bin directory
$(OBJ_DIR)/t_%.o: $(TST_DIR)/%.c | $(OBJ_DIR) 
	$(CC) $(CFLAGS) -c $< -o $@ -I$(SRC_DIR)

# Bin directory creation
$(OBJ_DIR):
	mkdir -p ./$(OBJ_DIR)

# Bin directory creation
$(BIN_DIR):
	mkdir -p ./$(BIN_DIR)

# Release directory creation
$(RELEASE_DIR):
	mkdir -p ./$(RELEASE_DIR)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(RELEASE_DIR) *.gz
