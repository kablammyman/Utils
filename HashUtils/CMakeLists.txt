
project(HashUtils)

file(GLOB SOURCES
	${PROJECT_SOURCE_DIR}/*.h
	${PROJECT_SOURCE_DIR}/*.cpp)
ADD_LIBRARY(HashUtils ${SOURCES})
