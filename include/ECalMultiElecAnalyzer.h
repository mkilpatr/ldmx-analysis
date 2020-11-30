
#ifndef ECAL_MULTI_ANALYZER_H
#define ECAL_MULTI_ANALYZER_H

/*~~~~~~~~~~~~~~*/
/*   Framework  */
/*~~~~~~~~~~~~~~*/
#include "Framework/EventProcessor.h"
#include "Framework/Parameters.h"
#include "DetDescr/EcalID.h"
#include "DetDescr/EcalHexReadout.h"

namespace ldmx { 

    /// Forward Declarations
    class NtupleManager; 

    /**
     * @class ECalMultiElecAnalyzer 
     * @brief Analyzer used to create an ntuple out of ECal BDT features and other variables needed for ECal performance studies.
     */
    class ECalMultiElecAnalyzer : public Analyzer { 
   
        public: 
            
            /**
             * Class constructor.
             *
             * @param name Name for this instance of the class.
             * @param process The Process used to run this analyzer, provided 
             *                by the framework. 
             */
            ECalMultiElecAnalyzer(const std::string &name, Process &process); 

            /// Destructor
            ~ECalMultiElecAnalyzer();

            /**
             * Callback for the analyzer to take any action before the 
             * processing of events begins. 
             */
            void onProcessStart() final override;

            /** 
             * Configure the processor using the given user specified parameters.
             * 
             * @param parameters Set of parameters used to configure this processor.
             */
            void configure(Parameters& parameters) final override;

            /**
             * Process the event and create an ntuple for analysis. 
             *
             * @param event The Event to analyze
             */ 
            virtual void analyze(const Event& event) final override; 

        private: 

            /// Default names of the collections
            std::string ecalSimHitCollectionName_{"EcalSimHits"}; 

            /// Extra tree holding info for the individual hits
            TTree* hitTree_{nullptr};
            std::vector<float> hitXv, hitYv, hitZv, hitLayerv, recHitEnergyv, recHitAmplitudev, simHitEnergyv, recParTime, simParTime, simNContib, elecTime, elecDiffTime, elecAbsDiffTime;
            int trigPass;
            bool debug = false;

            // Helper Instance of EcalDetectorID
            EcalID detID_;

            // Helper Instance of EcalHexReadout:
            std::unique_ptr<EcalHexReadout> ecalHexReadout_;

    }; // ECalMultiElecAnalyzer

} // ldmx

#endif // ECAL_MULTI_ANALYZER_H
