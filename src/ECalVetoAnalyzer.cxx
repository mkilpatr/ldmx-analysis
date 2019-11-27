
#include <iostream> 

#include "ECalVetoAnalyzer.h" 

#include "Framework/Process.h" 
#include "Framework/NtupleManager.h"
#include "Event/EcalVetoResult.h" 

namespace ldmx { 

    ECalVetoAnalyzer::ECalVetoAnalyzer(const std::string &name, Process &process) : 
        Analyzer(name, process) {
    }

    ECalVetoAnalyzer::~ECalVetoAnalyzer() { 
    }

    void ECalVetoAnalyzer::onProcessStart() {
        process_.openHistoFile();

        ntuple_ = NtupleManager::getInstance(); 
        ntuple_->create("ECalVeto"); 

        ntuple_->addVar<float>("ECalVeto", "bdt_pred"); 
    }

    void ECalVetoAnalyzer::configure(const ParameterSet &pSet) {
        
        // Set the name of the ECal veto collection to use
        ecalVetoCollectionName_ = pSet.getString("ecal_veto_collection"); 
    }


    void ECalVetoAnalyzer::analyze(const Event& event) {
   
        // Check for the ECal veto collection in the event.  If it doesn't 
        // exists, skip creating an ntuple.
        if (!event.exists(ecalVetoCollectionName_)) return; 
        
        // Get the ECal veto result for this event
        auto veto{static_cast<const EcalVetoResult*>(
                event.getCollection(ecalVetoCollectionName_)->At(0))};

        // Set the BDT score
        ntuple_->setVar<float>("bdt_pred", veto->getDisc());  

    }

} //

DECLARE_ANALYZER_NS(ldmx, ECalVetoAnalyzer)
