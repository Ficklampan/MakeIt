g++ -c makeit.cpp makeit_parser.cpp file_utils.cpp string_utils.cpp -std=c++17 -lboost_filesystem
g++ -o makeit makeit.o makeit_parser.o file_utils.o string_utils.o -lboost_filesystem
