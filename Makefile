NAME = MakeIt
BUILD = ./build
FLAGS = 
CC = gcc

LIBS = -lme
INCS = 
LDIR = -Lexternal/lib
IDIR = -Iexternal/include
DEFS = 

# <-- sources -->
SRC = 	./src/MakeIt.c \
	./src/MakeItFunctions.c \
	./src/MakeFile.c \
	./src/Config.c \
	./src/script/MakeItScript.c \
	./src/script/lexer/MakeItLexer.c \
	./src/script/parser/MakeItParser.c

# <-- headers -->
HEADERS = 	./src/**.h

# <-- objects -->
OBJS = 	./build/src/MakeIt.o \
	./build/src/MakeItFunctions.o \
	./build/src/MakeFile.o \
	./build/src/Config.o \
	./build/src/script/MakeItScript.o \
	./build/src/script/lexer/MakeItLexer.o \
	./build/src/script/parser/MakeItParser.o

$(BUILD)/%.o: %.* $(HEADERS)
	@$(CC) -c -o $@ $< $(FLAGS) $(IDIR) $(INCS) $(DEFS) && \
	echo [32mcompiling [$<][0m

$(NAME): $(OBJS)
	@$(CC) -o $@ $^ $(LDIR) $(LIBS)

.PHONY: clean

clean:
	rm -f $(OBJS)
