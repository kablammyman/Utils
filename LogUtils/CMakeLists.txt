
project(LogUtils)
include_directories(../StringUtils)
include_directories(../TCPUtils)

file(GLOB SOURCES
	${PROJECT_SOURCE_DIR}/*.h
	${PROJECT_SOURCE_DIR}/*.cpp)
set(windows_sources ${SOURCES})
if (NOT WIN32)
list(REMOVE_ITEM sources ${PROJECT_SOURCE_DIR}/WindowsEventLogOutput.cpp)
list(REMOVE_ITEM sources ${PROJECT_SOURCE_DIR}/WindowsEventLogOutput.h)
endif()

ADD_LIBRARY(LogUtils ${SOURCES})



