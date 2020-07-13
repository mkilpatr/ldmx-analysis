
#include "EventPrinter.h"

namespace ldmx {

    void EventPrinter::configure(Parameters& ps) {

        verbosity_ = ps.getParameter<int>("verbosity");

        return;
    }

    void EventPrinter::analyze(const Event& event) {

        event.Print(verbosity_);

        return;
    }

}

DECLARE_ANALYZER_NS(ldmx, EventPrinter)
