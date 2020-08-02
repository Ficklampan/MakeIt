NAME = MakeIt
SRC = ./src/MakeIt.c ./src/MakeItFunc.c ./src/MakeFile.c ./src/MemPool.c ./src/Config.c ./src/utils/FileUtils.c ./src/utils/Map.c ./src/utils/String.c
HEADERS = ./src/utils/Map.h ./src/utils/String.h ./src/utils/Type.h ./src/utils/FileUtils.h ./src/MakeFile.h ./src/texts.h ./src/MemPool.h ./src/Config.h ./src/MakeItFunc.h ./src/MakeIt.h
OBJ = ./MakeItFiles/src/MakeIt.o ./MakeItFiles/src/MakeItFunc.o ./MakeItFiles/src/MakeFile.o ./MakeItFiles/src/MemPool.o ./MakeItFiles/src/Config.o ./MakeItFiles/src/utils/FileUtils.o ./MakeItFiles/src/utils/Map.o ./MakeItFiles/src/utils/String.o 
CC = g++
CFLAGS = 

OUTD = ./MakeItFiles
LIBD = -L/usr/lib 
INCD = -I/usr/include 

DEFS = -DDEBUG 
LIBS = -lGLFW -lGLEW 

$(OUTD)/%.o: %.cpp $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS) $(INCD) $(DEFS)

$(NAME): $(OBJ)
	$(CC) -o $@ $^ $(LIBD) $(LIBS)

.PHONY: clean

clean:
	rm -f $(OBJ)
