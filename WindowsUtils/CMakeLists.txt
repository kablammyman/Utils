
project(WindowsUtils)
if(WIN32)
file(GLOB SOURCES
	${PROJECT_SOURCE_DIR}/*.h
	${PROJECT_SOURCE_DIR}/*.cpp)
ADD_LIBRARY(WindowsUtils ${SOURCES})
endif()