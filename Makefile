ROOT:=$(dir $(lastword $(MAKEFILE_LIST)))
SRC=$(ROOT)/src
INCLUDE=$(ROOT)/include
BUILD=build
BIN=bin

CFLAGS+= -I$(INCLUDE) -pedantic -Wall -Wextra -DDEBUG
LFLAGS+= -lcurses -lreadline

SOURCES:=$(wildcard $(SRC)/*.c)
BUILDFILES:=$(patsubst $(SRC)/%.c,$(BUILD)/%.o,$(SOURCES))

all: $(BIN)/risk #| Makefile

## WARN: this out of source build style is dangerous when using `make -B`
# # For out of source build
# Makefile:
# 	echo "include $(ROOT)/Makefile" > Makefile

$(BIN)/risk: $(BUILDFILES) | $(BIN)
	$(CC) $(LFLAGS) $^ -o $@

$(BUILD)/%.o: $(SRC)/%.c $(INCLUDE)/util.h | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: $(INCLUDE)/%.h

$(BUILD) $(BIN):
	mkdir -p $@

clean:
	$(RM) -r $(BUILD) $(BIN)
