
project(NetUtils)


file(GLOB SOURCES
	${PROJECT_SOURCE_DIR}/*.h
	${PROJECT_SOURCE_DIR}/*.cpp)
ADD_LIBRARY(NetUtils ${SOURCES})
if(WIN32)
	target_link_libraries(NetUtils wsock32 ws2_32)
endif()