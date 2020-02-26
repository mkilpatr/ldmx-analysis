
file(GLOB ANALYSES
    "*.cxx"
)

add_library(Analysis SHARED ${ANALYSES})
target_link_libraries(Analysis ${LDMX_LIBS})

install(TARGETS Analysis DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
