set(net_SRC
	src/xsocket.c
	src/XSPSyncSocket.cpp
	src/XSPSocket.cpp
	src/XSPLocalIpList.cpp
	src/XSPInetTable.cpp
	src/XSPInetAddr.cpp
	src/XSPAsyncSocket.cpp)


INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/build/lib)

ADD_LIBRARY(net SHARED ${net_SRC})

set(HEADERS
	src/xsocket.h
	src/xnet.h
	src/xcurl.h
	src/XSPSyncSocket.h
	src/XSPSocket.h
	src/XSPLocalIpList.h
	src/XSPInetTable.h
	src/XSPInetAddr.h
	src/XSPAsyncSocket.h)