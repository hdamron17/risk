ROOT:=$(dir $(lastword $(MAKEFILE_LIST)))
SRC=$(ROOT)/src
INCLUDE=$(ROOT)/include
BUILD=build
BIN=bin
TEST=$(ROOT)/test
BINTEST=$(BIN)/test
DIST=dist
GENDIRS=$(BUILD) $(BIN) $(BINTEST) $(DIST)

VERSION:=$(firstword $(shell cat $(ROOT)/VERSION.txt))
TARFILE=$(DIST)/risk-$(VERSION).tar.gz

DEBUGFLAGS=-DDEBUG -g
CFLAGS+= -I$(INCLUDE) -pedantic -Wall -Wextra -DCOLOR $(DEBUGFLAGS)
LFLAGS+= -lcurses -lreadline

SOURCES:=$(wildcard $(SRC)/*.c)
BUILDFILES:=$(patsubst $(SRC)/%.c,$(BUILD)/%.o,$(SOURCES))
TESTS:=$(wildcard $(TEST)/test_*.c)
BINTESTS:=$(patsubst $(TEST)/test_%.c,$(BINTEST)/test_%,$(TESTS))

.PHONY: all test clean distclean tar version
all: $(BIN)/risk

version:
	@echo $(VERSION)

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

$(GENDIRS):
	mkdir -p $@

clean:
	$(RM) -r $(BUILD) $(BIN) $(BINTEST)

distclean: clean
	$(RM) -r $(DIST)

tar: | $(DIST)
	tar -C $(ROOT)/.. $(foreach d,$(GENDIRS), --exclude=$(d)) -czvf $(TARFILE) $(notdir $(realpath $(ROOT)))
