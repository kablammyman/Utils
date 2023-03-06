project(TaggingUtils)
include_directories($ENV{MY_STRING_UTILS} )
include_directories($ENV{MY_SQLITE3_UTILS}/SQLiteUtils/SQLite3/src)
include_directories($ENV{MY_SQLITE3_UTILS} )


IF(WIN32)

set(MY_STRING_LIB_64_D $ENV{MY_X64_DEBUG_LIBS}/StringUtils.lib)
set(MY_STRING_LIB_64_R $ENV{MY_X64_RELEASE_LIBS}/StringUtils.lib)
set(MY_SQLITE_LIB_64_D $ENV{MY_X64_DEBUG_LIBS}/SQLiteUtils.lib)
set(MY_SQLITE_LIB_64_R $ENV{MY_X64_RELEASE_LIBS}/SQLiteUtils.lib)

ELSE()
#notice the "lib" in front of my stuff
set(MY_STRING_LIB_64_D $ENV{MY_X64_DEBUG_LIBS}/libStringUtils.a)
set(MY_STRING_LIB_64_R $ENV{MY_X64_RELEASE_LIBS}/libStringUtils.a)
set(MY_SQLITE_LIB_64_D $ENV{MY_X64_DEBUG_LIBS}/SQLiteUtils.a)
set(MY_SQLITE_LIB_64_R $ENV{MY_X64_RELEASE_LIBS}/SQLiteUtils.a)

ENDIF()

file(GLOB SOURCES
	${PROJECT_SOURCE_DIR}/*.h
	${PROJECT_SOURCE_DIR}/*.cpp)
ADD_LIBRARY(TaggingUtils ${SOURCES})


target_link_libraries (TaggingUtils debug   ${MY_STRING_LIB_64_D} ${MY_SQLITE_LIB_64_D}  )
target_link_libraries (TaggingUtils optimized   ${MY_STRING_LIB_64_D} ${MY_SQLITE_LIB_64_R}  )
