# ldmx-analysis

This repo is a helpful "kickstart" repo.
While you may wish to make direct changes to `ldmx-sw`, 
this will allow you to run many processors without having to re-compile `ldmx-sw` 
every time you wish to re-compile your own processors.
This will speed up compilation time and therefore help you be more efficient!

## Building and Installing

This analysis package can only be used with a built and installed version of `ldmx-sw`.
If you haven't built and installed `ldmx-sw` yet, please [do so](https://github.com/LDMX-Software/ldmx-sw).

Here are the steps to configuring and building this stand-alone library.

> If you are using the docker container (i.e. you ran `source ldmx-sw/scripts/ldmx-env.sh`), then you should prefix all of the below commands with `ldmx` (e.g. `make install` would instead be `ldmx make install`). Since you built ldmx-sw inside of the container, you need to build ldmx-analysis inside the container.

1. Make and Enter a build directory: `mkdir build; cd build`
2. Run the following cmake command. This command tells this build where ldmx-sw is using the environment variables defined by the setup script. You need to type in the path to your ldmx-sw installation.
```bash
cmake -DLDMX_INSTALL_PREFIX=<path-to-ldmx-sw-install> ../
```
This repository assumes that you require ROOT as well, so you may need to tell cmake where ROOT is using `-DROOT_DIR=<path-to-root-install>`.
If you are using the docker container, the defaults for all of these variables are already coded into the container and `CMakeLists.txt`, so you can just run `ldmx cmake ..` in your build directory.

3. Build and install: `make install`

## Running Processors in ldmx-analysis

#### Environment Setup
This repository is setup to act like another module of ldmx-sw and is installed alongside ldmx-sw.
If you aren't installing alongside ldmx-sw, you will need to add the separate installation to your `PYTHONPATH`.
```bash
# put these lines in your environment setup script to make this easier (maybe ~/.bash_profile? or ~/.bashrc?)
export PYTHONPATH=<path-to-this-repo-install>/lib/python:$PYTHONPATH
```

#### Configuration Script
A few example scripts are given in the `config` directory.
The basic idea behind running these is the same as other processors, for example:
```python
#import templates
from LDMX.Analysis import ecal
#create a copy
myEcalVetoAna = ecal.ECalVetoAnalyzer('myEcalVetoAna')
#change parameters (if you want to)
myEcalVetoAna.tracker_veto_collection_name = 'MySpecialTrackerVeto'
#give to process
p.sequence.append( myEcalVetoAna )
```
