NAME = MakeIt
VERSION = 2020.08.06
NAMEV = MakeIt-2020.08.06


SRC = ./src/MakeIt.c ./src/MakeItParser.c ./src/MakeItFunc.c ./src/MakeFile.c ./src/Config.c ./src/utils/FileUtils.c ./src/utils/Map.c ./src/utils/String.c ./src/utils/Time.c
HEADERS = $(headers)
OBJ = ./MakeItFiles/src/MakeIt.o ./MakeItFiles/src/MakeItParser.o ./MakeItFiles/src/MakeItFunc.o ./MakeItFiles/src/MakeFile.o ./MakeItFiles/src/Config.o ./MakeItFiles/src/utils/FileUtils.o ./MakeItFiles/src/utils/Map.o ./MakeItFiles/src/utils/String.o ./MakeItFiles/src/utils/Time.o

CC = gcc
CFLAGS = 
LIBS = 
DEFS = 
BDIR = ./MakeItFiles
IDIR = 
LDIR = 

$(BDIR)/%.o: %.* $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS) $(IDIR) $(DEFS) && echo [0%]: compiling $<

$(NAMEV): $(OBJ)
	$(CC) -o $@ $^ $(LDIR) $(LIBS)

.PHONY: clean

clean:
	rm -f $(OBJ)
