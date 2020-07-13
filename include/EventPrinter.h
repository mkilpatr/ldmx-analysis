#ifndef ANALYSIS_EVENTPRINTER_H
#define ANALYSIS_EVENTPRINTER_H

//LDMX Framework
#include "Event/EventDef.h" // Includes all possible event bus objects
#include "Framework/EventProcessor.h" // Needed to declare processor
#include "Framework/Parameters.h" // Needed to import parameters from configuration file

namespace ldmx {
    
    /**
     * @class EventPrinter
     * @brief Prints every event through Event::Print using the input verbosity.
     */
    class EventPrinter : public Analyzer {
        public:

            EventPrinter(const std::string& name, Process& process) : Analyzer(name, process) {}

            /// set the verbosity of event printing
            virtual void configure(Parameters& ps);

            /// print each event
            virtual void analyze(const Event& event);

        private:

            /// verbosity to be passed to Event::Print
            int verbosity_;

    };
}

#endif /* ANALYSIS_EVENTPRINTER_H */
