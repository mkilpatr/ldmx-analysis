# Minimum CMake version
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

# Project name
project(ldmx-analysis LANGUAGES CXX)

# Set the CXX standard 
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(LDMXSW_DIR)
    set(LDMXSW_INCLUDE_DIR "${LDMXSW_DIR}/include")
    set(FRAMEWORK_LIBRARY "${LDMXSW_DIR}/lib/libFramework.so")
    set(EVENT_LIBRARY "${LDMXSW_DIR}/lib/libEvent.so")
    include_directories(${LDMXSW_INCLUDE_DIR})
endif()

# Define the headers
include_directories(include)

# Define the sources 
add_subdirectory(src)
