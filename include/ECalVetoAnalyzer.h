
#ifndef ECAL_VETO_ANALYZER_H
#define ECAL_VETO_ANALYZER_H

/*~~~~~~~~~~~~~~*/
/*   Framework  */
/*~~~~~~~~~~~~~~*/
#include "Framework/EventProcessor.h"
#include "Framework/Parameters.h"

namespace ldmx { 

    /// Forward Declarations
    class NtupleManager; 

    /**
     * @class ECalVetoAnalyzer 
     * @brief Analyzer used to create an ntuple out of ECal BDT features.
     */
    class ECalVetoAnalyzer : public Analyzer { 
   
        public: 
            
            /**
             * Class constructor.
             *
             * @param name Name for this instance of the class.
             * @param process The Process used to run this analyzer, provided 
             *                by the framework. 
             */
            ECalVetoAnalyzer(const std::string &name, Process &process); 

            /// Destructor
            ~ECalVetoAnalyzer();

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

            /// Default name of the ECal veto collection
            std::string ecalVetoCollectionName_{"EcalVeto"}; 
        
            /// Instance of NtupleManager
            NtupleManager* ntuple_{nullptr}; 

    }; // ECalVetoAnalyzer

} // ldmx

#endif // ECAL_VETO_ANALYZER_H
