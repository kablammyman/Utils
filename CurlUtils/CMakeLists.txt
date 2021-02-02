project(CurlUtils)
include_directories(./libcurl/include)
include_directories(${PROJECT_SOURCE_DIR}/../StringUtils)

IF(WIN32)
set(LIBCURL_LIB_DEBUG ${PROJECT_SOURCE_DIR}/libcurl/lib/win32/libcurl_a.lib)
set(LIBCURL_LIB_RELEASE ${PROJECT_SOURCE_DIR}/libcurl/lib/win32/libcurl_a.lib)
add_definitions( -DCURL_STATICLIB )

ELSE()
set(LIBCURL_LIB_DEBUG ${PROJECT_SOURCE_DIR}/libcurl/lib/linux/libcurl.a)
set(LIBCURL_LIB_RELEASE ${PROJECT_SOURCE_DIR}/libcurl/lib/linux/libcurl.a)
ENDIF()

file(GLOB SOURCES
	${PROJECT_SOURCE_DIR}/../StringUtils/StringUtils.h
	${PROJECT_SOURCE_DIR}/../StringUtils/StringUtils.cpp
	${PROJECT_SOURCE_DIR}/*.h
	${PROJECT_SOURCE_DIR}/*.cpp)

ADD_LIBRARY(CurlUtils STATIC ${SOURCES})

if(WIN32)
	target_link_libraries(CurlUtils wsock32 ws2_32 Wldap32 Crypt32 Normaliz)	
endif()


target_link_libraries (CurlUtils debug ${LIBCURL_LIB_DEBUG} release ${LIBCURL_LIB_RELEASE})
