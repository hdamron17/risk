ROOT:=$(dir $(lastword $(MAKEFILE_LIST)))
SRC=$(ROOT)/src
INCLUDE=$(ROOT)/include
BUILD=build
BIN=bin
TEST=$(ROOT)/test
BINTEST=$(BIN)/test
TARFILE=risk.tar.gz

DEBUGFLAGS=-DDEBUG -g
CFLAGS+= -I$(INCLUDE) -pedantic -Wall -Wextra -DCOLOR $(DEBUGFLAGS)
LFLAGS+= -lcurses -lreadline

SOURCES:=$(wildcard $(SRC)/*.c)
BUILDFILES:=$(patsubst $(SRC)/%.c,$(BUILD)/%.o,$(SOURCES))
TESTS:=$(wildcard $(TEST)/test_*.c)
BINTESTS:=$(patsubst $(TEST)/test_%.c,$(BINTEST)/test_%,$(TESTS))

.PHONY: all test clean distclean tar
all: $(BIN)/risk

$(BIN)/risk: $(BUILDFILES) | $(BIN)
	$(CC) $(LFLAGS) $^ -o $@

$(BUILD)/%.o: $(SRC)/%.c $(INCLUDE)/util.h | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: $(INCLUDE)/%.h

test: $(BINTESTS)
	$(foreach test,$(BINTESTS),./$(test) &&) true

$(BINTEST)/test_%: $(TEST)/test_%.c | $(BINTEST)
	$(CC) $(CFLAGS) -I$(SRC) -I$(TEST) $< -o $@

$(BINTEST)/test_%: $(SRC)/%.c

$(BUILD) $(BIN) $(BINTEST):
	mkdir -p $@

clean:
	$(RM) -r $(BUILD) $(BIN) $(BINTEST)

distclean: clean
	$(RM) $(TARFILE)

tar: clean
	tar -C $(ROOT)/.. -czvf $(TARFILE) $(notdir $(realpath $(ROOT)))
