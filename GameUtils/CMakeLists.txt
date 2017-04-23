
project(GameUtils)

file(GLOB SOURCES
	${PROJECT_SOURCE_DIR}/*.h
	${PROJECT_SOURCE_DIR}/*.cpp)
ADD_LIBRARY(GameUtils ${SOURCES})
