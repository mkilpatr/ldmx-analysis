# Minimum CMake version
cmake_minimum_required(VERSION 3.2.3 FATAL_ERROR)

# Project name
project(ldmx-analysis LANGUAGES CXX)

# Set the CXX standard 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#this piece of cmake nonsense allows us to change the default
#   installation location
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX $ENV{LDMX_BASE}/ldmx-analysis/install CACHE PATH "..." FORCE)
endif()

# Setup the LDMX INSTALL location
if(LDMX_INSTALL_PREFIX)
    message( STATUS "ldmx-sw passed as parameter: ${LDMX_INSTALL_PREFIX}" )
else()
    set(LDMX_INSTALL_PREFIX $ENV{LDMX_BASE}/ldmx-sw/install )
endif()

# Include LDMXSW stuff
set(LDMXSW_INCLUDE_DIR "${LDMX_INSTALL_PREFIX}/include")
if(EXISTS ${LDMXSW_INCLUDE_DIR})
    include_directories(${LDMXSW_INCLUDE_DIR})
else()
    message( FATAL_ERROR "LDMXSW install not found. Give the path as '-DLDMX_INSTALL_PREFIX=<path-to-ldmx-sw-install>'" )
endif()
file(GLOB LDMX_LIBS ${LDMX_INSTALL_PREFIX}/lib/*.so)
message( STATUS "ldmx-sw found at ${LDMX_INSTALL_PREFIX}" )

# Get root and add it as an include
find_package(ROOT REQUIRED COMPONENTS Core RIO PyROOT)
include(${ROOT_USE_FILE})
message( STATUS "ROOT found at: ${ROOT_DIR}" )

# find boost headers
# specify location with -DBOOST_ROOT=<location-of-boost-prefix>
find_package(Boost REQUIRED)
message(STATUS "Boost lib found at: ${Boost_LIBRARY_DIRS}")
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

# Define the headers
include_directories(include)

# Define the sources 
add_subdirectory(src)

### Install a Python init file with a function returning full path to analysis library
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/python/__init__.py "\"\"\"LDMXANA python module\"\"\"" )
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/python/__init__.py DESTINATION lib/python/LDMXANA)

### Install the rest of the python after configuring them with cmake parameters
file(GLOB py_scripts ${CMAKE_CURRENT_SOURCE_DIR}/python/*)
foreach(py_script ${py_scripts})
    get_filename_component(py_script_output ${py_script} NAME)
    configure_file(${py_script} ${CMAKE_CURRENT_BINARY_DIR}/${py_script_output})
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${py_script_output} 
        DESTINATION lib/python/LDMXANA )
endforeach()

message( STATUS "ldmx-analysis will be installed to: ${CMAKE_INSTALL_PREFIX}" )
