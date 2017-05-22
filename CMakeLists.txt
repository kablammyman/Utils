# Copyright 2015 Etc2Comp Authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 2.8.9)
project(Utils)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/x64/Debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/x64/Debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/x64/Release)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/poop)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/poop)


#MESSAGE("--------------------------")
#MESSAGE("${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}")
#MESSAGE("--------------------------")

set (CMAKE_CXX_STANDARD 11)
IF (APPLE)
	set (CMAKE_CXX_FLAGS "-I/usr/include/i386-linux-gnu/c++/4.8 -I/usr/include/c++/4.8 -std=c++11 -g3 -Wall -O3")
ELSE ()
	IF (WIN32)
		set (CMAKE_CXX_FLAGS "-I/usr/include/i386-linux-gnu/c++/4.8 -I/usr/include/c++/4.8 -W4 /EHsc")
		ADD_SUBDIRECTORY(WindowsUtils)
		foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
			string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
			set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_SOURCE_DIR}/x64/${OUTPUTCONFIG} )
			set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_SOURCE_DIR}/x64/${OUTPUTCONFIG} )
			set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_SOURCE_DIR}/x64/${OUTPUTCONFIG} )
		endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

	ELSE()
		set (CMAKE_CXX_FLAGS "-I/usr/include/i386-linux-gnu/c++/4.8 -I/usr/include/c++/4.8 -std=c++11 -pthread -g3 -Wall -O2")
		
		
	ENDIF()
ENDIF ()

ADD_SUBDIRECTORY(StringUtils)
ADD_SUBDIRECTORY(CFGUtils)
ADD_SUBDIRECTORY(CommandLineUtils)
ADD_SUBDIRECTORY(FileUtils)
ADD_SUBDIRECTORY(GameUtils)
ADD_SUBDIRECTORY(HashUtils)
ADD_SUBDIRECTORY(SQLiteUtils)
#ADD_SUBDIRECTORY(TCPUtils)
#ADD_SUBDIRECTORY(LogUtils)
