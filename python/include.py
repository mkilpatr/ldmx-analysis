"""Include the Analysis module"""

def library() :
    """Attach the name of Analysis library to the process"""
    from LDMX.Framework.ldmxcfg import Process
    Process.addLibrary('@CMAKE_INSTALL_PREFIX@/lib/libAnalysis.so')
