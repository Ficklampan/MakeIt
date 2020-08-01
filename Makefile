NAME = "MurderEngine"
SRC = src/Test.cpp src/Engine.cpp src/stuff/Maths.cpp  
HEADERS = src/include/Keyboard.hpp src/include/Header.hpp  
OBJ = ./MakeItFiles/c/Test.o ./MakeItFiles/c/Engine.o ./MakeItFiles/c/stuff/Maths.o 

CC = g++
CFLAGS = -std=c++17
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
