NAME = MakeIt
BUILD = ./build
FLAGS = -std=c++17 -Wall -g
CC = g++

LIBS = -lme
INCS = 
LDIR = -Lexternal/lib
IDIR = -Iexternal/include
DEFS = 

# <-- sources -->
SRC = 	./src/MakeIt.cpp \
	./src/configs/GNUMake.cpp \
	./src/configs/YCM.cpp \
	./src/script/Script.cpp \
	./src/script/lexer/Lexer.cpp \
	./src/script/lexer/TokenLexer.cpp \
	./src/script/parser/Parser.cpp \
	./src/script/vm/MiVm.cpp \
	./src/functions/Print.cpp \
	./src/functions/System.cpp \
	./src/functions/Search.cpp \
	./src/functions/Project.cpp \
	./src/functions/Script.cpp \
	./src/functions/MakeConfig.cpp \
	./src/functions/YCMConfig.cpp \
	./src/functions/Functions.cpp

# <-- headers -->
HEADERS =   ./src/script/parser/Parser.hpp \
	    ./src/script/vm/MiVm.hpp \
	    ./src/script/vm/Instruction.hpp \
	    ./src/script/Script.hpp \
	    ./src/script/lexer/Lexer.hpp \
	    ./src/script/lexer/TokenLexer.hpp \
	    ./src/Texts.h \
	    ./src/configs/GNUMake.hpp \
	    ./src/configs/YCM.hpp \
	    ./src/configs/Config.hpp \
	    ./src/MakeIt.hpp \
	    ./src/utils/Time.h \
	    ./src/Functions.hpp

# <-- objects -->
OBJ = 	./build/src/MakeIt.o \
	./build/src/configs/GNUMake.o \
	./build/src/configs/YCM.o \
	./build/src/script/Script.o \
	./build/src/script/lexer/Lexer.o \
	./build/src/script/lexer/TokenLexer.o \
	./build/src/script/parser/Parser.o \
	./build/src/script/vm/MiVm.o \
	./build/src/functions/Print.o \
	./build/src/functions/System.o \
	./build/src/functions/Search.o \
	./build/src/functions/Project.o \
	./build/src/functions/Script.o \
	./build/src/functions/MakeConfig.o \
	./build/src/functions/YCMConfig.o \
	./build/src/functions/Functions.o

$(BUILD)/%.o: %.* $(HEADERS)
	@$(CC) -c -o $@ $< $(FLAGS) $(IDIR) $(INCS) $(DEFS) && \
	echo [32mcompiling [$<][0m

$(NAME): $(OBJ)
	@$(CC) -o $@ $^ $(LDIR) $(LIBS)

.PHONY: clean

clean:
	rm -f $(OBJ)
