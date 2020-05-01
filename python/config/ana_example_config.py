
from LDMX.Framework import ldmxcfg

# Create a process with pass name "recon"
p=ldmxcfg.Process('vetoana')

# Append the library that contains the analyzer below to the list of libraries 
# that the framework will load.
# You will need to add the path to your install location to PYTHONPATH, e.g.
#   export PYTHONPATH=/path/to/this/install/lib/python:$PYTHONPATH
from LDMXANA import libAnaPath
p.libraries.append( libAnaPath.libAnaPath() )

# Create an instance of the ECal veto analyzer.  This analyzer is used to create
# an ntuple out of ECal BDT variables. The analyzer requires that the
# veto collection name be set.   
ecal_veto_ana = ldmxcfg.Producer("ecal", "ldmx::ECalVetoAnalyzer")
ecal_veto_ana.parameters['ecal_veto_collection'] = "EcalVetoGabriel"

# Create an instance of the HCal veto analyzer. This analyzer is used to create 
# an ntuple with results from the HCal veto processor. 
hcal_veto_ana = ldmxcfg.Producer("hcal", "ldmx::HCalVetoAnalyzer") 

# Define the order in which the analyzers will be executed.
p.sequence=[ecal_veto_ana, hcal_veto_ana]

# input the file as an argument on the command line
import sys
p.inputFiles=[sys.argv[1]]

# Specify the output file.  When creating ntuples or saving histograms, the 
# output file name is specified by setting the attribute histogramFile.  
p.histogramFile='ntuple_test.root'

# Print out the details of the configured analyzers. 
print p
