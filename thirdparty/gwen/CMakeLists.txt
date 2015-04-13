# GWEN
#
# This file is part of GWEN.
#
# MIT License
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#

CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)
PROJECT(GWEN)

INCLUDE_DIRECTORIES(${GWEN_SOURCE_DIR}/include/)
INCLUDE_DIRECTORIES(${GWEN_SOURCE_DIR}/src/)

SET(RENDERER_FILES ${GWEN_SOURCE_DIR}/Renderers/SDL2/SDL2.cpp)
SET(LIB_DIR ${GWEN_SOURCE_DIR}/lib)

FILE(GLOB SOURCE_FILES src/*.cpp)
FILE(GLOB SOURCE_CONTROL_FILES src/Controls/*.cpp)

IF (WIN32)
	SET(SOURCE_PLATFORM_FILES src/Platforms/Windows.cpp)
ELSE ()
	SET(SOURCE_PLATFORM_FILES src/Platforms/Null.cpp)
ENDIF ()

SET(ALL_FILES
    ${RENDERER_FILES}
	${SOURCE_FILES}
	${SOURCE_CONTROL_FILES}
	${SOURCE_PLATFORM_FILES}
)

IF(UNIX)
    ADD_DEFINITIONS(-fPIC)
ENDIF(UNIX)

ADD_LIBRARY(${PROJECT_NAME} STATIC ${ALL_FILES})

# Add some preprocessor definitions.
ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
