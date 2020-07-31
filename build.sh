gcc -c src/MakeIt.c src/utils/FileUtils.c src/utils/String.c src/utils/Map.c
gcc -o makeit MakeIt.o FileUtils.o String.o Map.o
