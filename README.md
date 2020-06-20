# MakeIt

inte klart för fem öre

CMake was too hard so i made my own thingy heheh.
I know the code is not good! I just wanted the thing.
I'm gonna clean it up when i have a keyboard under my fingers

example:
-
`
MakeIt.txt
`
```
project: // project name
  example

var: // variables
  SOURCES
  HEADERS
  LIBS

append:
  SOURCES // variable name
  ${CURRENT_DIR}/main.cpp
  ${CURRENT_DIR}/src/random.cpp

append:
  HEADERS
  ${CURRENT_DIR}/src/headers/**.hpp // `**` = search sub directories

find: // finds all files with a pattern. needed if you look above
  HEADERS // append to
  ${HEADERS} // files to search

append: // appends glfw and GLEW to LIBS
  LIBS
  glfw
  GLEW

directory: // add sub directories (the dir must contain a MakeIt.txt)
  ${CURRENT_DIR}/more_sources

library: // library paths
  /usr/lib/
  ${CURRENT_DIR}/libs/

include: // include paths
  /usr/include/
  ${CURRENT_DIR}/include/

cout: // print text
  compiling sources: ${SOURCES}

dependencies: // not done !!!
  git@something something // param[0]: clone/download link, param[1] target directory

define: // the same thing as `#define` in c
  USE_FLOAT
  DEBUG_MODE

verify: // check if files exists
  SOURCES
  HEADERS

makefile:
  -std=c++17
  ${SOURCES}
  ${HEADERS}
  ${LIBS}
 

```
