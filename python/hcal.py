"""Configuration for HCal-related analyzers"""

from LDMX.Framework.ldmxcfg import Analyzer

class HCalVetoAnalyzer(Analyzer) :
    """Configuration for HCalVetoAnalyzer
    
    This analyzer has no configuration parameters at this time.
    """

    def __init__(self,name = 'hcalVetoAna') :
        super().__init__(name,'ldmx::HCalVetoAnalyzer')

        from LDMXANA import libAnaPath
        from LDMX.Framework.ldmxcfg import Process
        Process.addLibrary( libAnaPath.libAnaPath() )
