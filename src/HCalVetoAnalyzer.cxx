
#include "HCalVetoAnalyzer.h" 

#include "Framework/Process.h" 
#include "Framework/NtupleManager.h"
#include "Event/HcalHit.h" 
#include "Event/HcalVetoResult.h" 

namespace ldmx { 

    HCalVetoAnalyzer::HCalVetoAnalyzer(const std::string &name, Process &process) : 
        Analyzer(name, process) {
    }

    HCalVetoAnalyzer::~HCalVetoAnalyzer() { 
    }

    void HCalVetoAnalyzer::onProcessStart() {
        process_.openHistoFile();

        ntuple_ = NtupleManager::getInstance(); 
        ntuple_->create("HCalVeto"); 

        ntuple_->addVar<float> ("HCalVeto", "max_pe"); 
        ntuple_->addVar<float>  ("HCalVeto", "max_pe_time"); 
    }

    void HCalVetoAnalyzer::configure(Parameters& parameters) {
    }


    void HCalVetoAnalyzer::analyze(const Event& event) {
 
        // Check for the existence of the HCal digis collection. If it doesn't
        // exists, skip the creation of the ntuple.  This collection needs to 
        // be present in order to get a reference to the max PE hit. 
        event.exists("hcalDigis");

        // Check for the HCal veto collection in the event.  If it doesn't 
        // exists, skip creating an ntuple.
        if (!event.exists("HcalVeto")) return;

        // Get the HCal veto result for this event
        auto veto{event.getObject<HcalVetoResult>("HcalVeto")};

        // Get the hit with the maximum number of PE's 
        auto maxPEHit{veto.getMaxPEHit()};
        
        // Fill the HCal veto variables
        ntuple_->setVar<float>("max_pe",      maxPEHit.getPE());  
        ntuple_->setVar<float>("max_pe_time", maxPEHit.getTime());  

    }

} // ldmx 

DECLARE_ANALYZER_NS(ldmx, HCalVetoAnalyzer)
