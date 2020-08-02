gcc -c src/MakeIt.c src/MakeItFunc.c src/MakeFile.c src/MemPool.c src/utils/FileUtils.c src/utils/String.c src/utils/Map.c -Wall
gcc -o makeit MakeIt.o MakeItFunc.o MakeFile.o MemPool.o FileUtils.o String.o Map.o
