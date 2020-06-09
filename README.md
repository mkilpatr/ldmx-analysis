# ldmx-analysis

This repo is a helpful "kickstart" repo that you can fork on github.
While you may wish to make direct changes to `ldmx-sw`, this will allow you to run many processors without have to re-compile `ldmx-sw` every time you wish to re-compile your own processors. This will speed up compilation time and therefore help you be more efficient!

### Building and Installing

This analysis package can only be used with a built and installed version of `ldmx-sw`.
If you haven't built and installed `ldmx-sw` yet, please [do so](https://github.com/LDMX-Software/ldmx-sw).

Here are the steps to configuring and building this stand-alone library.

> If you are using the docker container (i.e. you ran `source ldmx-sw/scripts/ldmx-env.sh`), then you should prefix all of the below commands with `ldmx` (e.g. `make install` would instead be `ldmx make install`). Since you built ldmx-sw inside of the container, you need to build ldmx-analysis inside the container.

1. Make and Enter a build directory: `mkdir build; cd build`
2. Run the following cmake command. This command tells this build where ldmx-sw is using the environment variables defined by the setup script. You need to type in the path to your ldmx-sw installation.
```bash
cmake -DCMAKE_INSTALL_PREFIX=../install/ -DLDMX_INSTALL_PREFIX=<path-to-ldmx-sw-install> ../
```
This repository assumes that you require ROOT as well, so you may need to tell cmake where ROOT is using `-DROOT_DIR=<path-to-root-install>`.

3. Build and install: `make install`

### Running Processors

Any processors in this repository can be run with `ldmx-app` as long as you tell `ldmx-app` where to find these processors.
This can be achieved by adding the following line to your python configuration script:
```python
p.libraries.append( "<path-to-this-repo-install>/lib/libAnalysis.so" )
```
where `p` is a previously defined `Process` object. With this line, you can use any processors written and compiled here as if they are processors inside of `ldmx-sw`.

Alternatively, if you add `<path-to-this-repo-install>/lib/python` to your PYTHONPATH environment variable, you will be able to use the predefined function `libAnaPath()` to return the full path to the analysis library.

In your environment setup script or `.bashrc`:
```bash
export PYTHONPATH=<path-to-this-repo-install>/lib/python:$PYTHONPATH
```
And in your python configuration file:
```python
import LDMXANA.libAnaPath as LA
p.libraries.append( LA.libAnaPath() )
```
