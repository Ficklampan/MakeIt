NAME = MakeIt

# compiler
CC = g++
FLAGS = -std=c++17 -g
# definitions
DEFS = -DDEBUG -DLOCALEDIR=\"/usr/share/locale\" -DPACKAGE=\"makeit\"
# libraries
LIBS = -lme
# library paths
LPATHS = -L./external/melib
# includes
INCS = 
# include paths
IPATHS = -I./external/melib/include -I./src/util

OBJECTS = ./build/src/makeit/MakeIt.o \
	./build/src/makeit/script/Script.o \
	./build/src/makeit/script/Token.o \
	./build/src/makeit/script/Variable.o \
	./build/src/makeit/script/lexer/Lexer.o \
	./build/src/makeit/script/lexer/ConstantLexer.o \
	./build/src/makeit/script/lexer/LiterialLexer.o \
	./build/src/makeit/script/lexer/PunctuatorLexer.o \
	./build/src/makeit/script/lexer/WildcardLexer.o \
	./build/src/makeit/script/parser/Parser.o \
	./build/src/makeit/script/parser/FunctionParser.o \
	./build/src/makeit/script/parser/LiterialParser.o \
	./build/src/makeit/script/parser/StatementParser.o \
	./build/src/makeit/functions/Functions.o \
	./build/src/makeit/functions/config/FuncMakefile.o \
	./build/src/makeit/functions/config/FuncYCM.o \
	./build/src/makeit/functions/project/FuncDefine.o \
	./build/src/makeit/functions/project/FuncInclude.o \
	./build/src/makeit/functions/project/FuncIncludePath.o \
	./build/src/makeit/functions/project/FuncLibrary.o \
	./build/src/makeit/functions/project/FuncLibraryPath.o \
	./build/src/makeit/functions/project/FuncProject.o \
	./build/src/makeit/functions/project/FuncSource.o \
	./build/src/makeit/functions/system/FuncPrint.o \
	./build/src/makeit/functions/system/FuncSearch.o \
	./build/src/makeit/functions/system/FuncSystem.o \
	./build/src/makeit/configs/GNUMake.o \
	./build/src/makeit/configs/YCM.o \
	./build/src/util/Common.o \
	./build/src/util/Util.o

HEADERS = ./src/makeit/script/parser/Parser.hpp \
	./src/makeit/script/Script.hpp \
	./src/makeit/script/lexer/Lexer.hpp \
	./src/makeit/script/Token.hpp \
	./src/makeit/script/Wildcard.hpp \
	./src/makeit/script/Variable.hpp \
	./src/makeit/script/Function.hpp \
	./src/makeit/script/Storage.hpp \
	./src/makeit/configs/YCM.hpp \
	./src/makeit/configs/GNUMake.hpp \
	./src/makeit/MakeIt.hpp \
	./src/makeit/functions/FuncUtils.hpp \
	./src/makeit/functions/Functions.hpp \
	./src/makeit/Config.hpp \
	./src/makeit/Text.hpp \
	./src/makeit/System.hpp \
	./src/makeit/Project.hpp \
	./src/util/Common.hpp \
	./src/util/Util.hpp


$(NAME): $(OBJECTS)
	@$(CC) -o $@ $^ $(LPATHS) $(LIBS) $(FLAGS)

./build/src/makeit/MakeIt.o: ./src/makeit/MakeIt.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/makeit/script/Script.o: ./src/makeit/script/Script.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/makeit/script/Token.o: ./src/makeit/script/Token.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/makeit/script/Variable.o: ./src/makeit/script/Variable.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/makeit/script/lexer/Lexer.o: ./src/makeit/script/lexer/Lexer.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/makeit/script/lexer/ConstantLexer.o: ./src/makeit/script/lexer/ConstantLexer.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/makeit/script/lexer/LiterialLexer.o: ./src/makeit/script/lexer/LiterialLexer.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/makeit/script/lexer/PunctuatorLexer.o: ./src/makeit/script/lexer/PunctuatorLexer.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/makeit/script/lexer/WildcardLexer.o: ./src/makeit/script/lexer/WildcardLexer.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/makeit/script/parser/Parser.o: ./src/makeit/script/parser/Parser.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/makeit/script/parser/FunctionParser.o: ./src/makeit/script/parser/FunctionParser.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/makeit/script/parser/LiterialParser.o: ./src/makeit/script/parser/LiterialParser.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/makeit/script/parser/StatementParser.o: ./src/makeit/script/parser/StatementParser.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/makeit/functions/Functions.o: ./src/makeit/functions/Functions.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/makeit/functions/config/FuncMakefile.o: ./src/makeit/functions/config/FuncMakefile.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/makeit/functions/config/FuncYCM.o: ./src/makeit/functions/config/FuncYCM.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/makeit/functions/project/FuncDefine.o: ./src/makeit/functions/project/FuncDefine.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/makeit/functions/project/FuncInclude.o: ./src/makeit/functions/project/FuncInclude.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/makeit/functions/project/FuncIncludePath.o: ./src/makeit/functions/project/FuncIncludePath.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/makeit/functions/project/FuncLibrary.o: ./src/makeit/functions/project/FuncLibrary.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/makeit/functions/project/FuncLibraryPath.o: ./src/makeit/functions/project/FuncLibraryPath.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/makeit/functions/project/FuncProject.o: ./src/makeit/functions/project/FuncProject.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/makeit/functions/project/FuncSource.o: ./src/makeit/functions/project/FuncSource.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/makeit/functions/system/FuncPrint.o: ./src/makeit/functions/system/FuncPrint.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/makeit/functions/system/FuncSearch.o: ./src/makeit/functions/system/FuncSearch.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/makeit/functions/system/FuncSystem.o: ./src/makeit/functions/system/FuncSystem.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/makeit/configs/GNUMake.o: ./src/makeit/configs/GNUMake.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/makeit/configs/YCM.o: ./src/makeit/configs/YCM.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/util/Common.o: ./src/util/Common.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"

./build/src/util/Util.o: ./src/util/Util.cpp $(HEADERS)
	@$(CC) -c -o $@ $< $(IPATHS) $(INCS) $(DEFS) $(FLAGS) && echo "[32mcompileing [$<][0m"


.PHONY: clean

clean:
	rm -f $(OBJECTS)
