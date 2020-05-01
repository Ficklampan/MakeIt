# MakeIt

example:
-
`
MakeIt.txt
`
```
project:
  example

var:
  SOURCES
  LIBS
  
append:
  SOURCES
  ${CURRENT_DIR}/main.cpp
  ${CURRENT_DIR}/src/random.cpp

append:
  LIBS
  glfw
  GLEW

directory:
  ${CURRENT_DIR}/more_sources

library:
  /usr/lib/
  ${CURRENT_DIR}/libs/

include:
  /usr/include/
  ${CURRENT_DIR}/include/

cout:
  compiling sources: ${SOURCES}

exec:
  -std=c++17
  ${SOURCES}
  ${LIBS}
 

```
