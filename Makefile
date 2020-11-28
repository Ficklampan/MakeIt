NAME = melib
CC = g++
BUILD = build

FLAGS = -std=c++17
LIBS = 
INCS = 
LPATHS = 
IPATHS = 
DEFS = 
EXT = 

COPTS = $(FLAGS) $(INCS) $(IPATHS) $(DEFS)
LOPTS = $(FLAGS) $(LIBS) $(LPATHS)

SOURCES = ./external/melib/src/file.cpp ./external/melib/src/util.cpp

OBJECTS = $(SOURCES:%=$(BUILD)/%.o)
DEPENDS = $(OBJECTS:%.o=%.d)

$(NAME): $(OBJECTS) $(EXT)
	@ar rcs $@.a $^

$(BUILD)/%.o: %
	@echo "[32m==> compiling source [33m[$<][0m"
	@mkdir -p $(@D)
	@$(CC) -c -o $@ $< $(COPTS) -MMD

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(DEPENDS)
