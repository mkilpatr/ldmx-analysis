
#include <iostream> 

#include "ECalMultiElecAnalyzer.h" 

#include "Framework/Process.h" 
#include "Framework/NtupleManager.h"
#include "Event/EcalVetoResult.h" 
#include "Event/HcalVetoResult.h" 
#include "Event/TrackerVetoResult.h" 
#include "Event/TriggerResult.h" 
#include "Event/SimCalorimeterHit.h"

using namespace std;

namespace ldmx { 
  
  ECalMultiElecAnalyzer::ECalMultiElecAnalyzer(const std::string &name, Process &process) : 
    Analyzer(name, process) {
  }
  
  ECalMultiElecAnalyzer::~ECalMultiElecAnalyzer() { 
  }
  
  void ECalMultiElecAnalyzer::onProcessStart() {
    process_.openHistoFile();
    
    ntuple_.create("EcalVeto"); 
    
    ntuple_.addVar<int>("EcalVeto", "trigPass");  
    ntuple_.addVar<float>("EcalVeto", "hcalMaxPE");
    ntuple_.addVar<int>("EcalVeto", "passHcalVeto");
    ntuple_.addVar<int>("EcalVeto", "passTrackerVeto");
    ntuple_.addVar<int>("EcalVeto", "nReadoutHits");  
    ntuple_.addVar<float>("EcalVeto", "summedDet");
    ntuple_.addVar<float>("EcalVeto", "summedTightIso");
    ntuple_.addVar<float>("EcalVeto", "maxCellDep");
    ntuple_.addVar<float>("EcalVeto", "showerRMS");
    ntuple_.addVar<float>("EcalVeto", "xStd");
    ntuple_.addVar<float>("EcalVeto", "yStd");
    ntuple_.addVar<float>("EcalVeto", "avgLayerHit");
    ntuple_.addVar<float>("EcalVeto", "stdLayerHit");
    ntuple_.addVar<int>("EcalVeto", "deepestLayerHit");
    ntuple_.addVar<int>("EcalVeto", "nElectrons");
    
    hitTree_ = new TTree("EcalHits","EcalHits");
    hitTree_->Branch("hitX",&hitXv);
    hitTree_->Branch("hitY",&hitYv);
    hitTree_->Branch("hitZ",&hitZv);
    hitTree_->Branch("hitLayer",&hitLayerv);
    hitTree_->Branch("recHitEnergy",&recHitEnergyv);
    hitTree_->Branch("recHitAmplitude",&recHitAmplitudev);
    hitTree_->Branch("simHitEnergy",&simHitEnergyv);
    hitTree_->Branch("recParTime",&recParTime);
    hitTree_->Branch("simParTime",&simParTime);
    hitTree_->Branch("trigPass", &trigPass, "trigPass/I");  
  }
  
  void ECalMultiElecAnalyzer::configure(Parameters& parameters) {
    
    // Set the name of the ECal veto collection to use
    trigResultCollectionName_ = parameters.getParameter< std::string>("trig_result_collection"); 
    trackerVetoCollectionName_ = parameters.getParameter< std::string>("tracker_veto_collection"); 
    hcalVetoCollectionName_ = parameters.getParameter< std::string>("hcal_veto_collection"); 
    ecalVetoCollectionName_ = parameters.getParameter< std::string>("ecal_veto_collection"); 
    ecalSimHitCollectionName_ = parameters.getParameter< std::string>("ecal_simhit_collection"); 
    ecalRecHitCollectionName_ = parameters.getParameter< std::string>("ecal_rechit_collection"); 
    ecalScorePlaneHitCollectionName_ = parameters.getParameter< std::string>("ecal_scoreplane_collection"); 
    
    double moduleRadius = 85.0; //same as default
    int    numCellsWide = 23; //same as default
    double moduleGap = 1.0;
    double ecalFrontZ = 220;
    std::vector<double> ecalSensLayersZ = {
         7.850,
        13.300,
        26.400,
        33.500,
        47.950,
        56.550,
        72.250,
        81.350,
        97.050,
        106.150,
        121.850,
        130.950,
        146.650,
        155.750,
        171.450,
        180.550,
        196.250,
        205.350,
        221.050,
        230.150,
        245.850,
        254.950,
        270.650,
        279.750,
        298.950,
        311.550,
        330.750,
        343.350,
        362.550,
        375.150,
        394.350,
        406.950,
        426.150,
        438.750
    };
    
    ecalHexReadout_ = std::make_unique<EcalHexReadout>(
            moduleRadius,
            moduleGap,
            numCellsWide,
            ecalSensLayersZ,
            ecalFrontZ
            );
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
    if (not event.exists(ecalVetoCollectionName_)) return; 
    
    // Get the results/hit collections for this event
    auto ecalVeto{event.getObject<EcalVetoResult>(ecalVetoCollectionName_)};
    auto trigResult{event.getObject<TriggerResult>(trigResultCollectionName_)};
    auto trackerVeto{event.getObject<TrackerVetoResult>(trackerVetoCollectionName_)};
    auto hcalVeto{event.getObject<HcalVetoResult>(hcalVetoCollectionName_)};
    auto ecalSimHits{event.getCollection<SimCalorimeterHit>(ecalSimHitCollectionName_)};
    auto ecalRecHits{event.getCollection<EcalHit>(ecalRecHitCollectionName_)};
    auto ecalScoreHits{event.getCollection<EcalHit>(ecalScorePlaneHitCollectionName_)};
    
    trigPass = trigResult.passed();
    
    // Set variables
    ntuple_.setVar<int>("trigPass", trigPass);  
    ntuple_.setVar<float>("hcalMaxPE", hcalVeto.getMaxPEHit().getPE());
    ntuple_.setVar<int>("passHcalVeto", hcalVeto.passesVeto());
    ntuple_.setVar<int>("passTrackerVeto", trackerVeto.passesVeto());
    ntuple_.setVar<int>("nReadoutHits", ecalVeto.getNReadoutHits());  
    ntuple_.setVar<float>("summedDet", ecalVeto.getSummedDet());
    ntuple_.setVar<float>("summedTightIso", ecalVeto.getSummedTightIso());
    ntuple_.setVar<float>("maxCellDep", ecalVeto.getMaxCellDep());
    ntuple_.setVar<float>("showerRMS", ecalVeto.getShowerRMS());
    ntuple_.setVar<float>("xStd", ecalVeto.getXStd());
    ntuple_.setVar<float>("yStd", ecalVeto.getYStd());
    ntuple_.setVar<float>("avgLayerHit", ecalVeto.getAvgLayerHit());
    ntuple_.setVar<float>("stdLayerHit", ecalVeto.getStdLayerHit());
    ntuple_.setVar<int>("deepestLayerHit", ecalVeto.getDeepestLayerHit());
    //
    
    std::sort( ecalSimHits.begin() , ecalSimHits.end() , 
            []( const SimCalorimeterHit &lhs , const SimCalorimeterHit &rhs ) {
                return lhs.getID() < rhs.getID();
            }
            );
    
    std::sort( ecalRecHits.begin() , ecalRecHits.end() , 
            []( const EcalHit &lhs , const EcalHit &rhs ) {
                return lhs.getID() < rhs.getID();
            }
            );
    
    std::sort( ecalScoreHits.begin() , ecalScoreHits.end() , 
            []( const EcalHit &lhs , const EcalHit &rhs ) {
                return lhs.getID() < rhs.getID();
            }
            );
    
    int nNoiseHits = 0;
    int nElectrons = 0;
    float noiseEnergy = 0.0;
    
    bool k_flag = false;
    std::list<int> kIDs = {11}; //PDG IDs for K_L^0, K_S^0, K^0, K^+
    
    for(auto simHit: ecalSimHits) { //loop over simHits
      //printf(simHit);
      for(int iContrib=0; iContrib <simHit.getNumberOfContribs(); ++iContrib) { //loop over contribs
        auto contrib = simHit.getContrib(iContrib);
        if(std::find(kIDs.begin(), kIDs.end(), std::abs(contrib.pdgCode)) != kIDs.end()) {
          nElectrons++;
        }
      } 
    } 
    ntuple_.setVar<int>("nElectrons", nElectrons);
 
    for(auto recHit : ecalRecHits) {
       if(recHit.isNoise()) {
          nNoiseHits++;
          noiseEnergy += recHit.getEnergy();
       } else {
          int rawID = recHit.getID();
          double x, y, z;
          ecalHexReadout_->getCellAbsolutePosition(rawID, x, y, z);
          detID_.setRawValue(rawID);
          detID_.unpack();
          int layer = detID_.getLayerID();
          hitXv.push_back(x);
          hitYv.push_back(y);
          hitZv.push_back(z);
          hitLayerv.push_back(layer);
          recHitEnergyv.push_back(recHit.getEnergy());
          recHitAmplitudev.push_back(recHit.getAmplitude());
          float totalSimEDep = 0.;
          for (auto simHit : ecalSimHits ) {
             if (simHit.getID() == rawID) {
                totalSimEDep += simHit.getEdep();
                recParTime.push_back(recHit.getTime());
                simParTime.push_back(simHit.getTime());
             } else if (simHit.getID() > rawID) {
                break;
             }
          }
          simHitEnergyv.push_back(totalSimEDep);
       }
    }
    
    ntuple_.setVar<int>("nNoiseHits", nNoiseHits);  
    ntuple_.setVar<float>("noiseEnergy", noiseEnergy);  
    
    hitTree_->Fill();
  
  }

} //

DECLARE_ANALYZER_NS(ldmx, ECalMultiElecAnalyzer)
