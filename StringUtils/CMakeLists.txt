
project(StringUtils)

file(GLOB SOURCES
	${PROJECT_SOURCE_DIR}/*.h
	${PROJECT_SOURCE_DIR}/*.cpp)
ADD_LIBRARY(StringUtils STATIC ${SOURCES})
