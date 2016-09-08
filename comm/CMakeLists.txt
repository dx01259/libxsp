set(comm_SRC
    src/daemon.c
    )

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/build/lib/)

ADD_LIBRARY(comm SHARED ${comm_SRC})



set(HEADERS
    src/daemon.h
	src/xdatetype.h
	src/xfun.h)
