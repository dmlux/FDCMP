#   Created by Denis-Michael Lux on 05. November 2015.
#
#   This file is part of FDCMP.
#
#   FDCMP is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   FDCMP is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with PSOFFT.  If not, see <http://www.gnu.org/licenses/>.

CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)

SET(FDCMP_MAJOR 1)
SET(FDCMP_MINOR 0)
SET(FDCMP_PATCH 0)

SET(PSOFFT_VERSION ${FDCMP_MAJOR}.${FDCMP_MINOR}.${FDCMP_PATCH})
MESSAGE(STATUS "Configuring FDCMP ${FDCMP_VERSION}")

PROJECT(FDCMP C)

MESSAGE(STATUS "")
MESSAGE(STATUS "CMAKE_SYSTEM_NAME        = ${CMAKE_SYSTEM_NAME}"         )
MESSAGE(STATUS "CMAKE_C_COMPILER_ID      = ${CMAKE_C_COMPILER_ID}"     )
MESSAGE(STATUS "CMAKE_C_COMPILER_VERSION = ${CMAKE_C_COMPILER_VERSION}")
MESSAGE(STATUS "")

MESSAGE(STATUS "")
MESSAGE(STATUS "*** FDCMP wrapper library will use the following libraries:")
MESSAGE(STATUS "*** FDCMP_LIBS           = ${FDCMP_LIBS}"                  )
MESSAGE(STATUS "*** FDCMP_INCLUDE_DIRS   = ${FDCMP_INCLUDE_DIRS}"          )
MESSAGE(STATUS "")

IF(CMAKE_COMPILER_IS_GNUC)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
    MESSAGE(STATUS "Detected gcc. Added '-O3' to compiler flags")
ENDIF()

IF(DEFINED CMAKE_C_COMPILER_ID AND DEFINED CMAKE_C_COMPILER_VERSION)
    IF(CMAKE_C_COMPILER_ID STREQUAL "GNU" AND NOT ${CMAKE_C_COMPILER_VERSION} VERSION_LESS 4.8.1)
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3"     )

        MESSAGE(STATUS "Detected gcc 4.8.1 or later. Added '-std=c99 -fopenmp' to compiler flags")
        MESSAGE(STATUS "OpenMP support for FDCMP enabled.")
    ENDIF()
ENDIF()

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/include )
INCLUDE_DIRECTORIES( ${FDCMP_INCLUDE_DIRS}               )

IF(APPLE)
    IF("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
        MESSAGE(STATUS "Clang compiler on MacOS X detected. Added '-std=c99 -O3' to compiler flags.")
    ENDIF()
ENDIF()

MESSAGE(STATUS "")
MESSAGE(STATUS "CMAKE_C_FLAGS              = ${CMAKE_C_FLAGS}"          )
MESSAGE(STATUS "CMAKE_SHARED_LINKER_FLAGS  = ${CMAKE_SHARED_LINKER_FLAGS}")
MESSAGE(STATUS "CMAKE_REQUIRED_INCLUDES    = ${CMAKE_REQUIRED_INCLUDES}"  )
MESSAGE(STATUS "")

FILE(GLOB SRCS ${SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c     )
FILE(GLOB SRCS ${SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h )

ADD_EXECUTABLE( fdcmp ${SRCS} )