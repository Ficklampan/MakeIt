NAME = MakeIt

SRC = ./src/MakeIt.c ./src/MakeItFunc.c ./src/MakeFile.c ./src/MemPool.c ./src/Config.c ./src/delete/delete1.cpp ./src/delete/delete2.cpp ./src/delete/deleteme.cxx ./src/delete/justtesting.cxx ./src/delete/moretest.cxx ./src/delete/whynot.c ./src/utils/FileUtils.c ./src/utils/Map.c ./src/utils/String.c
HEADERS = ./src/utils/Map.h ./src/utils/String.h ./src/utils/Type.h ./src/utils/FileUtils.h ./src/MakeFile.h ./src/texts.h ./src/MemPool.h ./src/Config.h ./src/MakeItFunc.h ./src/MakeIt.h
OBJ = ./MakeItFiles/src/MakeIt.o ./MakeItFiles/src/MakeItFunc.o ./MakeItFiles/src/MakeFile.o ./MakeItFiles/src/MemPool.o ./MakeItFiles/src/Config.o ./MakeItFiles/src/delete/delete1.o ./MakeItFiles/src/delete/delete2.o ./MakeItFiles/src/delete/deleteme.o ./MakeItFiles/src/delete/justtesting.o ./MakeItFiles/src/delete/moretest.o ./MakeItFiles/src/delete/whynot.o ./MakeItFiles/src/utils/FileUtils.o ./MakeItFiles/src/utils/Map.o ./MakeItFiles/src/utils/String.o

CC = gcc
CFLAGS = 
LIBS = -lGLFW -lGLEW
DEFS = -DDEBUG
BDIR = ./MakeItFiles
IDIR = -I/usr/include
LDIR = -L/usr/lib

$(OUTD)/%.o: %.* $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS) $(INCD) $(DEFS)

$(NAME): $(OBJ)
	$(CC) -o $@ $^ $(LIBD) $(LIBS)

.PHONY: clean

clean:
	rm -f $(OBJ)
