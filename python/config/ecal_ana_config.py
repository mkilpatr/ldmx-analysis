from LDMX.Framework import ldmxcfg

# Create a process with pass name "recon"
p=ldmxcfg.Process('vetoana')

# Create an instance of the ECal veto analyzer.  This analyzer is used to create
# an ntuple out of ECal BDT variables. The analyzer requires that the
# veto collection name be set.   
from LDMXANA import ecal
ecal_veto_ana = ecal.ECalVetoAnalyzer()

# Define the order in which the analyzers will be executed.
p.sequence=[ecal_veto_ana]

p.keep = [ "keep .*" ]

# input the file as an argument on the command line
import sys
p.inputFiles=[sys.argv[1]]

# Specify the output file.  When creating ntuples or saving histograms, the 
# output file name is specified by setting the attribute histogramFile.  
p.histogramFile='ntuple_test.root'

# Print out the details of the configured analyzers. 
print(p)
