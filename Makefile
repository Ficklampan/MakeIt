NAME = MakeIt
CC = g++
BUILD = build

FLAGS = -std=c++17

LIBS = -lme
INCS = 
LPATHS = -L./external/melib
IPATHS = -I./external/melib/include -I./src/util

DEFS = -DLOCALEDIR=\"/usr/share/locale\" -DPACKAGE=\"makeit\"

COPTS = $(FLAGS) $(INCS) $(IPATHS) $(DEFS)
LOPTS = $(FLAGS) $(LIBS) $(LPATHS)

SOURCES = ./src/makeit/MakeIt.cpp \
	./src/makeit/Text.cpp \
	./src/makeit/script/Script.cpp \
	./src/makeit/script/Token.cpp \
	./src/makeit/script/Variable.cpp \
	./src/makeit/script/lexer/Lexer.cpp \
	./src/makeit/script/lexer/ConstantLexer.cpp \
	./src/makeit/script/lexer/LiterialLexer.cpp \
	./src/makeit/script/lexer/PunctuatorLexer.cpp \
	./src/makeit/script/lexer/WildcardLexer.cpp \
	./src/makeit/script/parser/Parser.cpp \
	./src/makeit/script/parser/FunctionParser.cpp \
	./src/makeit/script/parser/LiterialParser.cpp \
	./src/makeit/script/parser/StatementParser.cpp \
	./src/makeit/functions/Functions.cpp \
	./src/makeit/functions/config/FuncMakefile.cpp \
	./src/makeit/functions/config/FuncYCM.cpp \
	./src/makeit/functions/project/FuncDefine.cpp \
	./src/makeit/functions/project/FuncFiles.cpp \
	./src/makeit/functions/project/FuncFlags.cpp \
	./src/makeit/functions/project/FuncInclude.cpp \
	./src/makeit/functions/project/FuncIncludePath.cpp \
	./src/makeit/functions/project/FuncLibrary.cpp \
	./src/makeit/functions/project/FuncLibraryPath.cpp \
	./src/makeit/functions/project/FuncProject.cpp \
	./src/makeit/functions/project/FuncSource.cpp \
	./src/makeit/functions/system/FuncPrint.cpp \
	./src/makeit/functions/system/FuncSearch.cpp \
	./src/makeit/functions/system/FuncSystem.cpp \
	./src/makeit/configs/GNUMake.cpp \
	./src/makeit/configs/YCM.cpp \
	./src/util/Common.cpp \
	./src/util/Util.cpp

OBJECTS = $(SOURCES:%=$(BUILD)/%.o)
DEPENDS = $(OBJECTS:%.o=%.d)

$(NAME): $(OBJECTS)
	@$(CC) -o $@ $^ $(LOPTS)

-include $(DEPENDS)

$(BUILD)/%.o: %
	@echo "==> [32mcompiling source [33m[$<][0m"
	@mkdir -p $(@D)
	@$(CC) -c -o $@ $< $(COPTS) -MMD

.PHONY: clean

clean:
	rm -f $(OBJECTS) $(DEPENDS)
