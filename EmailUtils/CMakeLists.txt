
project(EmailUtils)
include_directories(${PROJECT_SOURCE_DIR}/../StringUtils)
include_directories(./curl/include)


IF(WIN32)
set(CURL_LIB_DEBUG ${PROJECT_SOURCE_DIR}/curl/win32/lib/x64/libcurl-d_imp.lib)
set(CURL_LIB_RELEASE ${PROJECT_SOURCE_DIR}/curl/win32/lib/x64/libcurl_imp.lib)

ELSEIF(APPLE)
set(CURL_LIB_DEBUG ${PROJECT_SOURCE_DIR}/curl/osx/lib/x64/libcurl-d_imp.a)
set(CURL_LIB_RELEASE ${PROJECT_SOURCE_DIR}/curl/osx/lib/x64/libcurl_imp.a)
ELSE()
set(CURL_LIB_DEBUG ${PROJECT_SOURCE_DIR}/curl/lin/lib/x64/libcurl-d_imp.a)
set(CURL_LIB_RELEASE ${PROJECT_SOURCE_DIR}/curl/lin/lib/x64/libcurl_imp.a)
ENDIF()

file(GLOB SOURCES
	${PROJECT_SOURCE_DIR}/../StringUtils/StringUtils.h
	${PROJECT_SOURCE_DIR}/../StringUtils/StringUtils.cpp
	${PROJECT_SOURCE_DIR}/*.h
	${PROJECT_SOURCE_DIR}/*.cpp)
ADD_LIBRARY(EmailUtils ${SOURCES})

#link_directories(StringUtils)
#target_link_libraries (EmailUtils debug EmailUtils ${SQLITE3_LIB_DEBUG} release ${SQLITE3_LIB_RELEASE})
#looks like i will have to add the sqlite lib manually in vis studio->Librarian\General\AdditionalDependencies

target_link_libraries (EmailUtils debug ${CURL_LIB_DEBUG} release ${CURL_LIB_RELEASE})
