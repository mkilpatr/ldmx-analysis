
#ifndef HCAL_VETO_ANALYZER_H
#define HCAL_VETO_ANALYZER_H

/*~~~~~~~~~~~~~~*/
/*   Framework  */
/*~~~~~~~~~~~~~~*/
#include "Framework/EventProcessor.h" 
#include "Framework/Parameters.h"

namespace ldmx { 

    /// Forward Declarations
    class NtupleManager; 

    /**
     * @class HCalVetoAnalyzer 
     * @brief Analyzer used to create an ntuple from HCal veto variables.
     */
    class HCalVetoAnalyzer : public Analyzer { 
   
        public: 
            
            /**
             * Class constructor.
             *
             * @param name Name for this instance of the class.
             * @param process The Process used to run this analyzer.
             */
            HCalVetoAnalyzer(const std::string &name, Process &process); 

            /// Destructor
            ~HCalVetoAnalyzer();

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

    }; // HCalVetoAnalyzer

} // ldmx

#endif // HCAL_VETO_ANALYZER_H
