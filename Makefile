NAME = MakeIt

SRC = ./src/MakeIt.c ./src/MakeItFunc.c ./src/MakeFile.c ./src/MemPool.c ./src/Config.c ./src/utils/FileUtils.c ./src/utils/Map.c ./src/utils/String.c ./src/utils/Time.c
HEADERS = ./src/utils/Time.h ./src/utils/Map.h ./src/utils/String.h ./src/utils/Type.h ./src/utils/FileUtils.h ./src/MakeFile.h ./src/texts.h ./src/MemPool.h ./src/MakeItParser.h ./src/Config.h ./src/MakeItFunc.h ./src/MakeIt.h
OBJ = ./MakeItFiles/src/MakeIt.o ./MakeItFiles/src/MakeItFunc.o ./MakeItFiles/src/MakeFile.o ./MakeItFiles/src/MemPool.o ./MakeItFiles/src/Config.o ./MakeItFiles/src/utils/FileUtils.o ./MakeItFiles/src/utils/Map.o ./MakeItFiles/src/utils/String.o ./MakeItFiles/src/utils/Time.o

CC = gcc
CFLAGS = 
LIBS = 
DEFS = 
BDIR = ./MakeItFiles
IDIR = 
LDIR = 

$(BDIR)/%.o: %.* $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS) $(IDIR) $(DEFS)

$(NAME): $(OBJ)
	$(CC) -o $@ $^ $(LDIR) $(LIBS)

.PHONY: clean

clean:
	rm -f $(OBJ)
