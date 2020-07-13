"""Configuration classes for debugging """

class EventPrinter(Analyzer):
    """Configuration class for EventPrinter

    Prints each event with the input verbosity.


    Attributes
    ----------
    verbosity : int, optional
        Verbosity to be passed to Event::Print
    
    Warnings
    --------
    Very Verbose!! Only helpful when looking at < 10 events.

    Examples
    --------
    A quick python configuration to print all the events in the input root file:
        from LDMX.Framework import ldmxcfg
        p = ldmxcfg.Process('print')
        import sys
        p.inputFiles = [ sys.argv[1] ]
        from LDMX.Analysis import debug
        p.sequence = [ debug.EventPrinter() ]
    """

    def __init__(self,verbosity=3) :
        super().__init__('event_print_%d'%verbosity,'ldmx::EventPrinter')

        from LDMX.Analysis import include
        include.library()

        self.verbosity = verbosity
