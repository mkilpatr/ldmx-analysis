
file(GLOB ANALYSES
    "*.cxx"
)

add_library(Analysis SHARED ${ANALYSES})
target_link_libraries(Analysis ${FRAMEWORK_LIBRARY} ${EVENT_LIBRARY})
    
install(TARGETS Analysis DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
