project(CurlUtils)
include_directories(${PROJECT_SOURCE_DIR}/libcurl/include)
include_directories(${CMAKE_SOURCE_DIR}/StringUtils)

IF(WIN32)
set(LIBCURL_LIB_DEBUG ${PROJECT_SOURCE_DIR}/libcurl/lib/win32/libcurl_a.lib)
set(LIBCURL_LIB_RELEASE ${PROJECT_SOURCE_DIR}/libcurl/lib/win32/libcurl_a.lib)
#add_definitions( -DCURL_STATICLIB )

ELSE()
set(LIBCURL_LIB_DEBUG ${PROJECT_SOURCE_DIR}/libcurl/lib/linux/libcurl.a)
set(LIBCURL_LIB_RELEASE ${PROJECT_SOURCE_DIR}/libcurl/lib/linux/libcurl.a)
ENDIF()
add_definitions( -DCURL_STATICLIB )

file(GLOB SOURCES
	${PROJECT_SOURCE_DIR}/../StringUtils/StringUtils.h
	${PROJECT_SOURCE_DIR}/../StringUtils/StringUtils.cpp
	${PROJECT_SOURCE_DIR}/*.h
	${PROJECT_SOURCE_DIR}/*.cpp)
ADD_LIBRARY(CurlUtils ${SOURCES})

if(WIN32)
	target_link_libraries(CurlUtils wsock32 ws2_32 Wldap32 Crypt32 Normaliz  ${LIBCURL_LIB_DEBUG} )	
else()
	target_link_libraries (CurlUtils debug crypto ssl ${LIBCURL_LIB_DEBUG} )
endif()
