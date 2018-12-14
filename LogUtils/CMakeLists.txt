
project(LogUtils)
include_directories(../StringUtils)
if (WIN32)
include_directories(../TCPUtils)
endif()

file(GLOB SOURCES
	${PROJECT_SOURCE_DIR}/*.h
	${PROJECT_SOURCE_DIR}/*.cpp)

if (NOT WIN32)
list(REMOVE_ITEM SOURCES ${PROJECT_SOURCE_DIR}/WindowsEventLogOutput.cpp)
list(REMOVE_ITEM SOURCES ${PROJECT_SOURCE_DIR}/WindowsEventLogOutput.h)
list(REMOVE_ITEM SOURCES ${PROJECT_SOURCE_DIR}/TCPOutput.cpp)
list(REMOVE_ITEM SOURCES ${PROJECT_SOURCE_DIR}/TCPOutput.h)
endif()

ADD_LIBRARY(LogUtils ${SOURCES})



