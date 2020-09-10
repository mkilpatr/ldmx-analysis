"""Configuration for ECal-related analyzers"""

from LDMX.Framework.ldmxcfg import Analyzer

class ECalVetoAnalyzer(Analyzer) :
    """Configuration for ECalVetoAnalyzer

    Attributes
    ----------
    trig_result_collection : str
        Collection name of trigger result
    tracker_veto_collection : str
        Collection name of tracker veto
    hcal_veto_collection : str
        Collection name of hcal veto
    ecal_veto_collection : str
        Collection name of ecal veto
    ecal_simhit_collection : str
        Collection name for Ecal SimHits
    ecal_rechit_collection : str
        Collection name for Ecal RecHits
    """

    def __init__(self,name = 'ecalVetoAna') :
        super().__init__(name,'ldmx::ECalVetoAnalyzer')

        from LDMX.Analysis import include
        include.library()

        self.trig_result_collection  = 'Trigger'
        self.tracker_veto_collection = 'TrackerVeto'
        self.hcal_veto_collection    = 'HcalVeto'
        self.ecal_veto_collection    = 'EcalVeto'
        self.ecal_simhit_collection  = 'EcalSimHits'
        self.ecal_rechit_collection  = 'EcalRecHits'

class ECalMultiElecAnalyzer(Analyzer) :
    """Configuration for ECalMultiElecAnalyzer

    Attributes
    ----------
    trig_result_collection : str
        Collection name of trigger result
    tracker_veto_collection : str
        Collection name of tracker veto
    hcal_veto_collection : str
        Collection name of hcal veto
    ecal_veto_collection : str
        Collection name of ecal veto
    ecal_simhit_collection : str
        Collection name for Ecal SimHits
    ecal_rechit_collection : str
        Collection name for Ecal RecHits
    """

    def __init__(self,name = 'ecalVetoAna') :
        super().__init__(name,'ldmx::ECalMultiElecAnalyzer')

        from LDMX.Analysis import include
        include.library()

        self.ecal_simhit_collection  = 'EcalSimHits'
