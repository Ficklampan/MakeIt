NAME = MakeIt
BUILD = ./build
FLAGS = -std=c++17 -Wall -g
CC = g++

LIBS = -lme
INCS = 
LDIR = -Lexternal/melib
IDIR = -Iexternal/melib/include -Iexternal/toml11
DEFS = 

# <-- sources -->
SRC = 	./src/MakeIt.cpp \
	./src/configs/GNUMake.cpp \
	./src/configs/YCM.cpp \
	./src/script/Lexer.cpp \
	./src/script/Parser.cpp \
	./src/script/Variable.cpp \
	./src/functions/Print.cpp \
	./src/functions/System.cpp \
	./src/functions/Search.cpp \
	./src/functions/Project.cpp \
	./src/functions/MakeConfig.cpp \
	./src/functions/YCMConfig.cpp \
	./src/functions/Functions.cpp

# <-- headers -->
HEADERS =   ./src/Texts.h \
	    ./src/configs/GNUMake.hpp \
	    ./src/configs/YCM.hpp \
	    ./src/script/Function.hpp \
	    ./src/script/Lexer.hpp \
	    ./src/script/Parser.hpp \
	    ./src/script/Storage.hpp \
	    ./src/script/Token.hpp \
	    ./src/script/Variable.hpp \
	    ./src/MakeIt.hpp \
	    ./src/Project.hpp \
	    ./src/utils/Time.hpp \
	    ./src/Functions.hpp

# <-- objects -->
OBJ = 	./build/src/MakeIt.o \
	./build/src/configs/GNUMake.o \
	./build/src/configs/YCM.o \
	./build/src/script/Lexer.o \
	./build/src/script/Parser.o \
	./build/src/script/Variable.o \
	./build/src/functions/Print.o \
	./build/src/functions/System.o \
	./build/src/functions/Search.o \
	./build/src/functions/Project.o \
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
