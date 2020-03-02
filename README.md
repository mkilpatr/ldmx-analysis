# ldmx-analysis

This repo is a helpful "kickstart" repo that you can fork on github.
While you may wish to make direct changes to `ldmx-sw`, this will allow you to run many processors without have to re-compile `ldmx-sw` every time you wish to re-compile your own processors. This will speed up compilation time and therefore help you be more efficient!

### Building and Installing

This analysis package can only be used with a built and installed version of `ldmx-sw`.
If you haven't built and installed `ldmx-sw` yet, please [do so](https://github.com/LDMX-Software/ldmx-sw).

Here are the steps to configuring and building this stand-alone library:

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
