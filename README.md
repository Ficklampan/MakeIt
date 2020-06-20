# MakeIt

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

verify: // check if files exists
  SOURCES
  HEADERS

makefile:
  -std=c++17
  ${SOURCES}
  ${HEADERS}
  ${LIBS}
 

```
