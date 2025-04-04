project(SQLiteUtils)

# Include directories for dependencies
include_directories(${CMAKE_SOURCE_DIR}/StringUtils)
include_directories(${PROJECT_SOURCE_DIR}/SQLite3/src)

# SQLite3 library paths depending on the platform
IF(WIN32)
    set(SQLITE3_LIB_DEBUG ${PROJECT_SOURCE_DIR}/SQLite3/win32/lib/x86/sqlite3.lib)
    set(SQLITE3_LIB_RELEASE ${PROJECT_SOURCE_DIR}/SQLite3/win32/lib/x86/sqlite3.lib)

ELSEIF(APPLE)
    set(SQLITE3_LIB_DEBUG ${PROJECT_SOURCE_DIR}/SQLite3/osx/lib/x86/sqlite3.a)
    set(SQLITE3_LIB_RELEASE ${PROJECT_SOURCE_DIR}/SQLite3/osx/lib/x86/sqlite3.a)

ELSE()  # Linux case
    set(SQLITE3_LIB_DEBUG ${PROJECT_SOURCE_DIR}/SQLite3/linux/lib/libsqlite3.a)
    set(SQLITE3_LIB_RELEASE ${PROJECT_SOURCE_DIR}/SQLite3/linux/lib/libsqlite3.a)
ENDIF()

# Gather all source files for SQLiteUtils
file(GLOB SOURCES
    ${PROJECT_SOURCE_DIR}/../StringUtils/StringUtils.h
    ${PROJECT_SOURCE_DIR}/../StringUtils/StringUtils.cpp
    ${PROJECT_SOURCE_DIR}/*.h
    ${PROJECT_SOURCE_DIR}/*.cpp
)

# Add SQLiteUtils as a static library
ADD_LIBRARY(SQLiteUtils STATIC ${SOURCES})

# Link SQLite3 to SQLiteUtils for both debug and release
target_link_libraries(SQLiteUtils debug ${SQLITE3_LIB_DEBUG})
target_link_libraries(SQLiteUtils optimized ${SQLITE3_LIB_RELEASE})

# Optionally: Specify the output directory for the library
# set_target_properties(SQLiteUtils PROPERTIES
#     LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
# )

