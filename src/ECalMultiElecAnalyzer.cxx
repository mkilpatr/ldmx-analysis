
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
    ntuple_.addVar<double>("EcalSimHits", "Electron_energyDep");
    
    hitTree_ = new TTree("EcalHits","EcalHits");
    //hitTree_->Branch("hitX",&hitXv);
    //hitTree_->Branch("hitY",&hitYv);
    //hitTree_->Branch("hitZ",&hitZv);
    //hitTree_->Branch("hitLayer",&hitLayerv);
    //hitTree_->Branch("recHitEnergy",&recHitEnergyv);
    //hitTree_->Branch("recHitAmplitude",&recHitAmplitudev);
    hitTree_->Branch("simHitEnergy",&simHitEnergyv);
    //hitTree_->Branch("recParTime",&recParTime);
    hitTree_->Branch("simParTime",&simParTime);
    hitTree_->Branch("simNContib",&simNContib);
    hitTree_->Branch("elecTime",&elecTime);
    hitTree_->Branch("elecDiffTime",&elecDiffTime);
    hitTree_->Branch("elecAbsDiffTime",&elecAbsDiffTime);
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
    simNContib.clear();
    elecTime.clear();   
    elecDiffTime.clear();   
    elecAbsDiffTime.clear();   
 
    // Check for the ECal veto collection in the event.  If it doesn't 
    // exist, skip creating an ntuple.
    
    // Get the results/hit collections for this event
    auto ecalSimHits{event.getCollection<SimCalorimeterHit>(ecalSimHitCollectionName_)};
    auto particleMap{event.getMap< int, SimParticle >("SimParticles")};

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
    double elecEnergyDep = 0;
    float noiseEnergy = 0.0;
    
    int numSimHits = 0;
    double totalSimEDep = 0.;

    //for (const auto &keyVal : particleMap) {
    //    const SimParticle *simParticle = &(keyVal.second);
    //    //simParticle->Print();
    //}

    for ( const auto& simHit : ecalSimHits ) {
        //simHit.Print();
        totalSimEDep += simHit.getEdep();
        //if(simHit.getNumberOfContribs() < 15) continue;
        simParTime.push_back(simHit.getTime());
        simNContib.push_back(simHit.getNumberOfContribs());
        for( int i = 0 ; i < simHit.getNumberOfContribs() ; i++){
            if(simHit.getNumberOfContribs() < 1) continue;
            auto contrib = simHit.getContrib(i);
            if(debug) std::cout << "contrib " << i << " trackID: " << contrib.trackID << " pdgID: " << contrib.pdgCode << " edep: " << contrib.edep << std::endl;
            if(debug) std::cout << "\t particle id: " << particleMap[contrib.trackID].getPdgID() << " particle status: " << particleMap[contrib.trackID].getGenStatus() << std::endl;
            if( TMath::Abs(particleMap[contrib.trackID].getPdgID()) == 11 && particleMap[contrib.trackID].getGenStatus() == 1 ){
                nElectrons++;
                elecEnergyDep += contrib.edep;
                elecTime.push_back(contrib.time);
            }
        }
        simHitEnergyv.push_back(totalSimEDep);
    }

    for(int iE1 = 0; iE1 != elecTime.size(); iE1++){
        for(int iE2 = 0; iE2 != elecTime.size(); iE2++){
            if(iE1 >= iE2) continue;
            double elecDiff = elecTime[iE2] - elecTime[iE1];
            elecDiffTime.push_back(elecDiff);
            elecAbsDiffTime.push_back(TMath::Abs(elecDiff));
        }
    }

    //for ( const auto& simHit : ecalSimHits ) {
    //    simHit.Print();
    //    totalSimEDep += simHit.getEdep();
    //    for( int i = 0 ; i < simHit.getNumberOfContribs() ; i++){
    //        SimCalorimeterHit::Contrib contrib = simHit.getContrib(i);
    //        if (contrib.pdgCode == kIDs) {
    //            nElectrons++;
    //        }
    //    }
    //} //close loop over simHits

    ntuple_.setVar<int>("nElectrons", nElectrons);
    ntuple_.setVar<double>("Electron_energyDep", elecEnergyDep);
 
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
    
    hitTree_->Fill();
  
  }

} //

DECLARE_ANALYZER_NS(ldmx, ECalMultiElecAnalyzer)
