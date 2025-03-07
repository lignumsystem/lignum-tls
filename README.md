# LIGNUM-tls
LIGNUM model to create and analyse radiation climate for tree models created 
from Lidar/QSM trees.

## Compilation
The CMake system is used with the LIGNUM system. The command line examples are for the macOS Terminal
with the default `zsh` shell.

### Prerequisites
The LIGNUM-tls project depends on the *lignum-core* and *QSMReader* projects. To set-up these three projects 
correctly from GitHub:

+ Download *lignum-core* project
+ Download *QSMreader* and *lignum-tls* under the *lignum-core* project

Using Terminal and starting from the very beginning:

	git clone https://github.com/lignumsystem/lignum-core.git
	cd lignum-core
	git clone https://github.com/lignumsystem/QSMreader.git
	git clone https://github.com/lignumsystem/lignum-tls.git

### Compile lignum-tls
To compile the `lignum-tls` binary type in the command line:

    cd lignum-tls
    mkdir release
    cd release
    cmake .. -DCMAKE_BUILD_TYPE=Release
    make install

You should see the `lignum-tls` binary in the lignum-tls directory. See the *CMakeLists.txt* file 
for details, for example to generate XCode project for debugging. 

## Documentation
The software documentation is done with the Doxygen document generator that can parse 
specially formatted comments in software files as well as GitHub markdown files. 
To install Doxygen with MacPorts type:
	
	sudo port install doxygen +qt5
	sudo port install doxygen-launcher +qt5

The latter `doxygen-launcher` is the GUI (a.k.a `doxywizard`) for editing `doxygen` configuration files 
(so called Doxyfiles). 

Doxygen uses LaTeX to typeset mathematical symbols and equations found. 
To make full LaTeX installation type:

	sudo port install texlive +full
	
To generate the HTML documentation type:

	doxygen Doxyfile 2>error.txt
	open DoxygenDoc/html/index.html
	
The file *error.txt* contains errors and alerts found in comments and markdown (*.md*) files.

## Cython
The class Farquhar has a simple Cython interface that allows to build Python test program 
in order to verify the correctness and study the behaviour of the Farquhar model. 
Create a Python virtual environment, install required python packages and build the `farquhar`
Python package:
	
	sudo port install python312
	/opt/local/bin/python3.12 -m venv cythonenv
	source cythonenv/bin/activate
	pip install -r requirements.txt
	python3 setup.py build_ext --inplace
	
The test program is to appear.
