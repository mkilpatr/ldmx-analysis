
#include <iostream> 

#include "ECalMultiElecAnalyzer.h" 

#include "Framework/Process.h" 
#include "Framework/NtupleManager.h"
#include "Event/EcalVetoResult.h" 
#include "Event/SimCalorimeterHit.h"

using namespace std;

namespace ldmx { 
  
  ECalMultiElecAnalyzer::ECalMultiElecAnalyzer(const std::string &name, Process &process) : 
    Analyzer(name, process) {
  }
  
  ECalMultiElecAnalyzer::~ECalMultiElecAnalyzer() { 
  }
  
  void ECalMultiElecAnalyzer::onProcessStart() {
    getHistoDirectory();
    
    ntuple_.create("EcalSimHits"); 
    
    //ntuple_.addVar<int>("EcalSimHits", "nReadoutHits");  
    //ntuple_.addVar<float>("EcalSimHits", "summedDet");
    //ntuple_.addVar<float>("EcalSimHits", "summedTightIso");
    //ntuple_.addVar<float>("EcalSimHits", "maxCellDep");
    //ntuple_.addVar<float>("EcalSimHits", "showerRMS");
    //ntuple_.addVar<float>("EcalSimHits", "xStd");
    //ntuple_.addVar<float>("EcalSimHits", "yStd");
    //ntuple_.addVar<float>("EcalSimHits", "avgLayerHit");
    //ntuple_.addVar<float>("EcalSimHits", "stdLayerHit");
    //ntuple_.addVar<int>("EcalSimHits", "deepestLayerHit");
    ntuple_.addVar<int>("EcalSimHits", "nElectrons");
    
    //hitTree_ = new TTree("EcalHits","EcalHits");
    //hitTree_->Branch("hitX",&hitXv);
    //hitTree_->Branch("hitY",&hitYv);
    //hitTree_->Branch("hitZ",&hitZv);
    //hitTree_->Branch("hitLayer",&hitLayerv);
    //hitTree_->Branch("recHitEnergy",&recHitEnergyv);
    //hitTree_->Branch("recHitAmplitude",&recHitAmplitudev);
    //hitTree_->Branch("simHitEnergy",&simHitEnergyv);
    //hitTree_->Branch("recParTime",&recParTime);
    //hitTree_->Branch("simParTime",&simParTime);
  }
  
  void ECalMultiElecAnalyzer::configure(Parameters& parameters) {
    
    // Set the name of the ECal veto collection to use
    ecalSimHitCollectionName_ = parameters.getParameter< std::string>("ecal_simhit_collection");
    
    auto hexReadout{parameters.getParameter<Parameters>("hexReadout")};
    ecalHexReadout_ = std::make_unique<EcalHexReadout>(hexReadout);  
  
  }
  
  void ECalMultiElecAnalyzer::analyze(const Event& event) {
  
    hitXv.clear();
    hitYv.clear();
    hitZv.clear();
    hitLayerv.clear();
    recHitEnergyv.clear();
    recHitAmplitudev.clear();
    simHitEnergyv.clear();
    recParTime.clear();
    simParTime.clear();
    
    // Check for the ECal veto collection in the event.  If it doesn't 
    // exist, skip creating an ntuple.
    
    // Get the results/hit collections for this event
    auto ecalSimHits{event.getCollection<SimCalorimeterHit>(ecalSimHitCollectionName_)};
    
    // Set variables
    //ntuple_.setVar<int>("nReadoutHits", ecalRecHits.getNReadoutHits());  
    //ntuple_.setVar<float>("summedDet", ecalRecHits.getSummedDet());
    //ntuple_.setVar<float>("summedTightIso", ecalRecHits.getSummedTightIso());
    //ntuple_.setVar<float>("maxCellDep", ecalRecHits.getMaxCellDep());
    //ntuple_.setVar<float>("showerRMS", ecalRecHits.getShowerRMS());
    //ntuple_.setVar<float>("xStd", ecalRecHits.getXStd());
    //ntuple_.setVar<float>("yStd", ecalRecHits.getYStd());
    //ntuple_.setVar<float>("avgLayerHit", ecalRecHits.getAvgLayerHit());
    //ntuple_.setVar<float>("stdLayerHit", ecalRecHits.getStdLayerHit());
    //ntuple_.setVar<int>("deepestLayerHit", ecalRecHits.getDeepestLayerHit());
    //
   
    std::sort( ecalSimHits.begin() , ecalSimHits.end() , 
            []( const SimCalorimeterHit &lhs , const SimCalorimeterHit &rhs ) {
                return lhs.getID() < rhs.getID();
            }
            );
   
    int nNoiseHits = 0;
    int nElectrons = 0;
    float noiseEnergy = 0.0;
    
    bool k_flag = false;
    std::list<int> kIDs = {11}; 
    int numSimHits = 0;
    double totalSimEDep = 0.;
    for ( const SimCalorimeterHit &simHit : ecalSimHits ) {
        simHit.Print();
        totalSimEDep += simHit.getEdep();
        for(int iContrib=0; iContrib <simHit.getNumberOfContribs() - 1; ++iContrib) { //loop over contribs
          
          auto contrib = simHit.getContrib(iContrib);
          int absPDG = std::abs(contrib.pdgCode);
          if(std::find(kIDs.begin(), kIDs.end(), absPDG) != kIDs.end()) {
            k_flag = true;
            nElectrons++;
            //break; //stop loop over contribs
          }
        } //close loop over contribs
        //if (k_flag) break; //stop loop over simHits
    } //close loop over simHits

    ntuple_.setVar<int>("nElectrons", nElectrons);
 
    //for(auto recHit : ecalRecHits) {
    //   if(recHit.isNoise()) {
    //      nNoiseHits++;
    //      noiseEnergy += recHit.getEnergy();
    //   } else {
    //      int rawID = recHit.getID();
    //      double x, y, z;
    //      ecalHexReadout_->getCellAbsolutePosition(rawID, x, y, z);
    //      detID_.setRawValue(rawID);
    //      detID_.unpack();
    //      int layer = detID_.getLayerID();
    //      hitXv.push_back(x);
    //      hitYv.push_back(y);
    //      hitZv.push_back(z);
    //      hitLayerv.push_back(layer);
    //      recHitEnergyv.push_back(recHit.getEnergy());
    //      recHitAmplitudev.push_back(recHit.getAmplitude());
    //      float totalSimEDep = 0.;
    //      for (auto simHit : ecalSimHits ) {
    //         if (simHit.getID() == rawID) {
    //            totalSimEDep += simHit.getEdep();
    //            recParTime.push_back(recHit.getTime());
    //            simParTime.push_back(simHit.getTime());
    //         } else if (simHit.getID() > rawID) {
    //            break;
    //         }
    //      }
    //      simHitEnergyv.push_back(totalSimEDep);
    //   }
    //}
    //
    //ntuple_.setVar<int>("nNoiseHits", nNoiseHits);  
    //ntuple_.setVar<float>("noiseEnergy", noiseEnergy);  
    
    //hitTree_->Fill();
  
  }

} //

DECLARE_ANALYZER_NS(ldmx, ECalMultiElecAnalyzer)
