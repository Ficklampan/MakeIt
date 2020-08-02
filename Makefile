NAME = MurderEngine
SRC = ./src/MakeIt.c
HEADERS = ./src/MakeIt.c ./src/utils/Map.c ./src/utils/Map.h ./src/utils/String.h ./src/utils/String.c ./src/utils/Type.h ./src/utils/FileUtils.h ./src/utils/FileUtils.c ./src/MakeFile.c ./src/MakeFile.h ./src/texts.h ./src/MemPool.h ./src/MakeItFunc.h ./src/MakeItFunc.c ./src/MakeIt.h ./src/MemPool.c
OBJ = .MakeItFiles//src/MakeIt.o .MakeItFiles//src/MakeItFunc.o .MakeItFiles//src/MakeFile.o .MakeItFiles//src/utils/FileUtils.o .MakeItFiles//src/utils/Map.o .MakeItFiles//src/utils/String.o 
CC = g++
CFLAGS = 
OUTD = MakeItFiles
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
