# Minimum CMake version
cmake_minimum_required(VERSION 3.2.3 FATAL_ERROR)

# Project name
project(ldmx-analysis LANGUAGES CXX)

# Set the CXX standard 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Setup the LDMX INSTALL location
if(LDMX_INSTALL_PREFIX)
    message( STATUS "ldmx-sw passed as parameter: ${LDMX_INSTALL_PREFIX}" )
else()
    message( FATAL_ERROR "LDMXSW install not found. Give the path as '-DLDMX_INSTALL_PREFIX=<path-to-ldmx-sw-install>'" )
endif()

# Include LDMXSW stuff
set(LDMXSW_INCLUDE_DIR "${LDMX_INSTALL_PREFIX}/include")
include_directories(${LDMXSW_INCLUDE_DIR})
file(GLOB LDMX_LIBS ${LDMX_INSTALL_PREFIX}/lib/*.so)
message( STATUS "ldmx-sw found at ${LDMX_INSTALL_PREFIX}" )

# Get root and add it as an include
find_package(ROOT REQUIRED COMPONENTS Core RIO PyROOT)
include(${ROOT_USE_FILE})
message( STATUS "ROOT found at: ${ROOT_DIR}" )

## Install configuration scripts - switching out cmake parameters
#   NOT IMPLEMENTED
#file(GLOB config_scripts ${CMAKE_CURRENT_SOURCE_DIR}/config/*)
#foreach(config_script ${config_scripts})
#    get_filename_component(config_script_output ${config_script} NAME)
#    configure_file(${config_script} ${CMAKE_CURRENT_BINARY_DIR}/${config_script_output})
#    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${config_script_output} DESTINATION bin PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
#endforeach()

# Define the headers
include_directories(include)

# Define the sources 
add_subdirectory(src)

### Install a Python init file with a global variable with full path to analysis library
#   NOT IMPLEMENTED
#file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/python/__init__.py "# full path to analysis library\nANALYSIS_LIB = \"${CMAKE_INSTALL_PREFIX}/lib/libAnalysis.so\"" )
#install(FILES ${CMAKE_CURRENT_BINARY_DIR}/python/__init__.py DESTINATION lib/python/LDMX)

message( STATUS "ldmx-analysis will be installed to: ${CMAKE_INSTALL_PREFIX}" )
