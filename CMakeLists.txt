# Minimum CMake version
cmake_minimum_required(VERSION 3.2.3 FATAL_ERROR)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/install)

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
    find_package(Git QUIET)
    if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
        # Update submodules as needed
        option(GIT_SUBMODULE "Check submodules during build" ON)
        if(GIT_SUBMODULE)
            message(STATUS "Submodule update")
            execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMOD_RESULT)
            if(NOT GIT_SUBMOD_RESULT EQUAL "0")
                message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
            endif()
        endif()
    endif()

    if(NOT EXISTS "${PROJECT_SOURCE_DIR}/ldmx-sw/CMakeLists.txt")
        message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
    endif()

    #add external project into this directory
    set(LDMX_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/ldmx-sw/install")
    include(ExternalProject)
    externalproject_add( LDMXSW
        SOURCE_DIR ${PROJECT_SOURCE_DIR}/ldmx-sw
        CMAKE_ARGS "-DXercesC_DIR=$ENV{XERCESDIR};-DPYTHON_EXECUTABLE=`which python`;-DPYTHON_INCLUDE_DIR=$ENV{PYTHONHOME}/include/python2.7;-DPYTHON_LIBRARY=$ENV{PYTHONHOME}/lib/libpython2.7.so;-DCMAKE_INSTALL_PREFIX=${LDMX_INSTALL_PREFIX}"
        )

endif()

# Include LDMXSW stuff
set(LDMXSW_INCLUDE_DIR "${LDMX_INSTALL_PREFIX}/include")
include_directories(${LDMXSW_INCLUDE_DIR})
file(GLOB LDMX_LIBS ${LDMX_INSTALL_PREFIX}/lib/*.so)
#foreach( lib ${LDMX_LIBS} )
#    add_library(${lib})
#endforeach()
message( STATUS "ldmx-sw found at ${LDMX_INSTALL_PREFIX}" )

# Get root and add it as an include
find_package(ROOT REQUIRED COMPONENTS Core RIO PyROOT)
include(${ROOT_USE_FILE})
message( STATUS "ROOT found at: ${ROOT_DIR}" )

# Install configuration scripts - switching out cmake parameters
file(GLOB config_scripts ${CMAKE_CURRENT_SOURCE_DIR}/config/*)
foreach(config_script ${config_scripts})
    get_filename_component(config_script_output ${config_script} NAME)
    configure_file(${config_script} ${CMAKE_CURRENT_BINARY_DIR}/${config_script_output})
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${config_script_output} DESTINATION bin PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
endforeach()

# Define the headers
include_directories(include)

# Define the sources 
add_subdirectory(src)
