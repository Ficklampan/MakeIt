NAME = libme
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

SOURCES = ./src/file.cpp ./src/util.cpp

OBJECTS = $(SOURCES:%=$(BUILD)/%.o)
DEPENDS = $(OBJECTS:%.o=%.d)

.PHONY: build
build: $(EXT) $(NAME)

$(NAME): $(OBJECTS)
	@ar rcs $@.a $^

$(BUILD)/%.o: %
	@echo "[32m==> compiling source [33m[$<][0m"
	@mkdir -p $(dir $@)
	@$(CC) -c -o $@ $< $(COPTS) -MMD

.PHONY: clean
clean:
	rm -f libme.a $(OBJECTS) $(DEPENDS)
