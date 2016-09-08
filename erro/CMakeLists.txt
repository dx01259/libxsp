SET(erro_SRC
    src/xdef.cpp
    src/xerro.c)


INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})
ADD_LIBRARY(erro SHARED ${erro_SRC})

SET(HEADERS
    src/xdef.h
    src/xerro.h)


