NAME = MakeIt

# compiler
CC = g++
FLAGS = -std=c++17
# definitions
DEFS = 

# libraries
LIBS = -lme
# library paths
LPATHS = -L./external/melib
# includes
INCS = 
# include paths
IPATHS = -I./external/melib/include

OBJECTS = ./build/src/MakeIt.o \
	./build/src/script/Common.o \
	./build/src/script/Lexer.o \
	./build/src/script/Parser.o \
	./build/src/script/Script.o \
	./build/src/script/Token.o \
	./build/src/script/Variable.o \
	./build/src/functions/Functions.o \
	./build/src/functions/MakeConfig.o \
	./build/src/functions/Print.o \
	./build/src/functions/Project.o \
	./build/src/functions/Search.o \
	./build/src/functions/System.o \
	./build/src/functions/YCMConfig.o \
	./build/src/configs/GNUMake.o \
	./build/src/configs/YCM.o

HEADERS = ./src/script/Script.hpp \
	./src/script/Parser.hpp \
	./src/script/Token.hpp \
	./src/script/Common.hpp \
	./src/script/Lexer.hpp \
	./src/script/Variable.hpp \
	./src/script/Function.hpp \
	./src/script/Storage.hpp \
	./src/configs/YCM.hpp \
	./src/configs/GNUMake.hpp \
	./src/MakeIt.hpp \
	./src/functions/Common.hpp \
	./src/functions/Functions.hpp \
	./src/System.hpp \
	./src/utils/Time.hpp \
	./src/Project.hpp


# linking
$(NAME): $(OBJECTS)
	@$(CC) -o $@ $^ $(LPATHS) $(LIBS) $(FLAGS)

.PHONY: clean

clean:
	rm -f $(OBJECTS)

# compiling sources

./build/src/MakeIt.o: ./src/MakeIt.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/script/Common.o: ./src/script/Common.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/script/Lexer.o: ./src/script/Lexer.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/script/Parser.o: ./src/script/Parser.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/script/Script.o: ./src/script/Script.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/script/Token.o: ./src/script/Token.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/script/Variable.o: ./src/script/Variable.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/functions/Functions.o: ./src/functions/Functions.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/functions/MakeConfig.o: ./src/functions/MakeConfig.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/functions/Print.o: ./src/functions/Print.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/functions/Project.o: ./src/functions/Project.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/functions/Search.o: ./src/functions/Search.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/functions/System.o: ./src/functions/System.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/functions/YCMConfig.o: ./src/functions/YCMConfig.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/configs/GNUMake.o: ./src/configs/GNUMake.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/configs/YCM.o: ./src/configs/YCM.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

