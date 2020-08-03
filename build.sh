gcc -c src/MakeIt.c src/MakeItParser.c src/MakeItFunc.c src/MakeFile.c src/MemPool.c src/Config.c src/utils/FileUtils.c src/utils/String.c src/utils/Map.c src/utils/Time.c -Wall
gcc -o makeit MakeIt.o MakeItParser.o MakeItFunc.o MakeFile.o MemPool.o Config.o FileUtils.o String.o Map.o Time.o
