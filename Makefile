# Copyright something
NAME = MakeIt-2020
BUILD = build
OUTNAME = MakeIt
CC = g++

CFLAGS = -O3 -std=c++17
LIBS = -lme
INCS = 
LPATHS = -L./libme
IPATHS = -I./libme/include -I./src/util
DEFS = -DLOCALEDIR=\"/usr/share/locale\" -DPACKAGE=\"makeit\"

PKG_CONFIG_PATH = ./libme:./libme/include:./src/util:
CPKG = $$(env PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --cflags libme)
LPKG = $$(env PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --static --libs libme)

EXTERN = ./libme/libme.a

COPTS = $(CFLAGS) \
	$(IPATHS) \
	$(INCS) \
	$(DEFS) \
	$(CPKG)
LOPTS = $(LPATHS) $(LIBS) $(LPKG)

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
	./src/makeit/functions/config/FuncPkgConfig.cpp \
	./src/makeit/functions/config/FuncYCMConfig.cpp \
	./src/makeit/functions/project/FuncConfigure.cpp \
	./src/makeit/functions/project/FuncDefine.cpp \
	./src/makeit/functions/project/FuncExtern.cpp \
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
	./src/makeit/configs/make/Makefile.cpp \
	./src/makeit/configs/pc/PkgConfig.cpp \
	./src/makeit/configs/ycm/YCMConfig.cpp \
	./src/util/Common.cpp \
	./src/util/Util.cpp

OBJECTS = $(SOURCES:%=$(BUILD)/%.o)
DEPENDS = $(OBJECTS:%.o=%.d)

.PHONY: $(NAME)
$(NAME): $(EXTERN) $(OUTNAME)

$(OUTNAME): $(OBJECTS)
	@$(CC) -o $@ $^ $(LOPTS)

-include $(DEPENDS)

$(BUILD)/%.o: %
	@echo "[32m==> compiling source [33m[$<][0m"
	@mkdir -p $(dir $@)
	@$(CC) -c -o $@ $< $(COPTS) -MMD

# [./libme/libme.a]
./libme/libme.a: ./libme/MIBuild
	@echo "[97mexternal target '$<'[0m"
	(cd ./libme;makeit -f MIBuild;make -f Makefile)

.PHONY: clean
clean:
	rm -f $(OUTNAME) $(OBJECTS) $(DEPENDS)

.PHONY: install
install:
	cp makeit /bin/makeit
