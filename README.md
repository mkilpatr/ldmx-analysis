# ldmx-analysis

This repo is a helpful "kickstart" repo that you can fork on github.
While you may wish to make direct changes to `ldmx-sw`, this will allow you to run many processors without have to re-compile `ldmx-sw` every time you wish to re-compile your own processors. This will speed up compilation time and therefore help you be more efficient!

## Building and Installing

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
If you are using the docker container, the defaults for both of these variables are already coded into the container and `CMakeLists.txt`, so you can just run `ldmx cmake ..` in your build directory.

3. Build and install: `make install`

## Running Processors

There are three different methods for running the processors inside of this repo with the ldmx-sw application. All of them basically are used to tell ldmx-sw where to find the details it needs to know about these processors. 
They are listed in order of preference.

Below `p` refers to a previously defined `Process` object in your python configuration.

#### 1. Like any other processor
In the docker container the `LD_LIBRARY_PATH` environment variable is set to include the path to the ldmx-analysis library installation location, so all you need in your python configuration is
```python
p.libraries.append( 'libAnalysis.so' )
```
which is exactly how you specify other processors in other modules of ldmx-sw. You can replicate this behavior outside of the docker container by adding the path to the ldmx-analysis library to your own `LD_LIBRARY_PATH`:
```bash
export LD_LIBRARY_PATH=<path-to-this-repo-install>/lib:$LD_LIBRARY_PATH
```

#### 2. With some python + cmake magic
In the docker container, the `PYTHONPATH` environment variable is set to include the path to the ldmx-analysis python module installation, so you can access the full path to this library with a python function in your python config file.
```python
import LDMXANA.libAnaPath as LA
p.libraries.append( LA.libAnaPath() )
```
You can replicate this behavior without the docker container if you add the path to the install to your `PYTHONPATH`.
```bash
export PYTHONPATH=<path-to-this-repo-install>/lib/python:$PYTHONPATH
```

#### 3. Hardcoding the full path
This is the most brute force way of making ldmx-sw find your library. It is discouraged because then it is difficult to share your python configuration scripts with people on different computers.
```python
p.libraries.append( "<path-to-this-repo-install>/lib/libAnalysis.so" )
```
