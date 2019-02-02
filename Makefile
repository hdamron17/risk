ROOT:=$(dir $(lastword $(MAKEFILE_LIST)))
SRC=$(ROOT)/src
INCLUDE=$(ROOT)/include
BUILD=build
BIN=bin

CFLAGS+= -I$(INCLUDE)
LFLAGS+= -lcurses

SOURCES:=$(wildcard $(SRC)/*.c)
BUILDFILES:=$(patsubst $(SRC)/%.c,$(BUILD)/%.o,$(SOURCES))

all: $(BIN)/risk | Makefile

# For out of source build
Makefile:
	echo "include $(ROOT)/Makefile" > Makefile

$(BIN)/risk: $(BUILDFILES) | $(BIN)
	$(CC) $(LFLAGS) $^ -o $@

$(BUILD)/%.o: $(SRC)/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: $(INCLUDE)/%.h

$(BUILD) $(BIN):
	mkdir -p $@

clean:
	$(RM) -r $(BUILD) $(BIN)
