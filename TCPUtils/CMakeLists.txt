
project(TCPUtils)


file(GLOB SOURCES
	${PROJECT_SOURCE_DIR}/*.h
	${PROJECT_SOURCE_DIR}/*.cpp)
ADD_LIBRARY(TCPUtils ${SOURCES})
if(WIN32)
	target_link_libraries(TCPUtils wsock32 ws2_32)
endif()