
project(FileUtils)
include_directories(${PROJECT_SOURCE_DIR}/../StringUtils)


file(GLOB SOURCES
	${PROJECT_SOURCE_DIR}/../StringUtils/StringUtils.h
	${PROJECT_SOURCE_DIR}/../StringUtils/StringUtils.cpp
	${PROJECT_SOURCE_DIR}/*.h
	${PROJECT_SOURCE_DIR}/*.cpp)


#link_directories(StringUtils)

ADD_LIBRARY(FileUtils STATIC ${SOURCES})

