
project(SQLiteUtils)

include_directories(./SQLite3/src)


IF(WIN32)
set(SQLITE3_LIB_DEBUG ${PROJECT_SOURCE_DIR}/SQLite3/win32/lib/x86/sqlite3.lib)
set(SQLITE3_LIB_RELEASE ${PROJECT_SOURCE_DIR}/SQLite3/win32/lib/x86/sqlite3.lib)

ELSEIF(APPLE)
set(SQLITE3_LIB_DEBUG ${PROJECT_SOURCE_DIR}/SQLite3/osx/lib/x86/sqlite3.a)
set(SQLITE3_LIB_RELEASE ${PROJECT_SOURCE_DIR}/SQLite3/osx/lib/x86/sqlite3.a)
ELSE()
set(SQLITE3_LIB_DEBUG ${PROJECT_SOURCE_DIR}/SQLite3/lin/lib/x86/sqlite3.a)
set(SQLITE3_LIB_RELEASE ${PROJECT_SOURCE_DIR}/SQLite3/lin/lib/x86/sqlite3.a)
ENDIF()

file(GLOB SOURCES
	${PROJECT_SOURCE_DIR}/*.h
	${PROJECT_SOURCE_DIR}/*.cpp)
ADD_LIBRARY(SQLiteUtils ${SOURCES})

#target_link_libraries (SQLiteUtils debug sqlite3 ${SQLITE3_LIB_DEBUG} release ${SQLITE3_LIB_RELEASE})
target_link_libraries (SQLiteUtils debug ${SQLITE3_LIB_DEBUG} release ${SQLITE3_LIB_RELEASE})
#looks like i will have to add the sqlite lib manually in vis studio->Librarian\General\AdditionalDependencies