project(CurlUtils)
include_directories(./libcurl/include)

IF(WIN32)
set(LIBCURL_LIB_DEBUG ${PROJECT_SOURCE_DIR}/libcurl/lib/win32/libcurl_a.lib)
set(LIBCURL_LIB_RELEASE ${PROJECT_SOURCE_DIR}/libcurl/lib/win32/libcurl_a.lib)

ELSE()
set(LIBCURL_LIB_DEBUG ${PROJECT_SOURCE_DIR}/libcurl/lib/linux/libcurl.a)
set(LIBCURL_LIB_RELEASE ${PROJECT_SOURCE_DIR}/libcurl/lib/linux/libcurl.a)
ENDIF()

file(GLOB SOURCES
	${PROJECT_SOURCE_DIR}/*.h
	${PROJECT_SOURCE_DIR}/*.cpp)
add_definitions(CURL_STATICLIB)
ADD_LIBRARY(StringUtils STATIC ${SOURCES})
