
project(FileUtils)
include_directories(../StringUtils)
link_directories(StringUtils)
file(GLOB SOURCES
	${PROJECT_SOURCE_DIR}/*.h
	${PROJECT_SOURCE_DIR}/*.cpp)
ADD_LIBRARY(FileUtils ${SOURCES})

