gcc -c src/MakeIt.c src/utils/List.c src/utils/FileUtils.c src/utils/String.c
gcc -o makeit MakeIt.o List.o FileUtils.o String.o
