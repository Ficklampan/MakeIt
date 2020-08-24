NAME = MakeIt
VERSION = 2020.08.06
NAMEV = MakeIt-2020.08.06

SRC = ./src/MakeIt.c ./src/script/mlexer.c ./src/script/mvm.c ./src/script/mconf.c ./src/MakeItFunc.c ./src/MakeFile.c ./src/Config.c ./src/utils/FileUtils.c ./src/utils/Arrays.c ./src/utils/String.c 
HEADERS = ./src/utils/Arrays.h ./src/utils/String.h ./src/utils/Type.h ./src/utils/FileUtils.h ./src/MakeFile.h ./src/script/mlexer.h ./src/Texts.h ./src/Config.h ./src/MakeItFunc.h ./src/MakeIt.h ./src/script/mtoken.h ./src/script/mvar.h ./src/script/mfunc.h ./src/script/mfunctions.h ./src/script/mconf.h ./src/script/mtype.h
OBJ = ./MakeItFiles/src/MakeIt.o ./MakeItFiles/src/script/mlexer.o ./MakeItFiles/src/script/mvm.o ./MakeItFiles/src/MakeItFunc.o ./MakeItFiles/src/MakeFile.o ./MakeItFiles/src/Config.o ./MakeItFiles/src/utils/FileUtils.o ./MakeItFiles/src/utils/Arrays.o ./MakeItFiles/src/utils/String.o 

CC = gcc
CFLAGS = 
LIBS = 
INCS = 
DEFS = 
BDIR = ./MakeItFiles
IDIR = 
LDIR = 

$(BDIR)/%.o: %.* $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS) $(IDIR) $(INCS) $(DEFS) && echo [32m$<[0m

$(NAMEV): $(OBJ)
	$(CC) -o $@ $^ $(LDIR) $(LIBS)

.PHONY: clean

clean:
	rm -f $(OBJ)
