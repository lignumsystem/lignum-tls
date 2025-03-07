# LIGNUM-tls
LIGNUM model to create and analyse radiation climate for tree models created 
from Lidar/QSM trees.

## Compilation
The CMake system is used with the LIGNUM system.

### Prerequisites
Download and compile the *lignum-core* project. Download *QSMreader* and *lignum-tls* under
the *lignum-core* project (i.e. under the lignum-core directory). This is assumed in the CMakeLists.txt files.

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
The software documentation is done with the Doxygen document generator.
To install Doxygen with MacPorts type:
	
	sudo port install doxygen +qt5
	sudo port install doxygen-launcher +qt5

The latter `doxygen-launcher` is the GUI (a.k.a `doxywizard`) for editing `doxygen` configuration files 
(so called Doxyfiles). 

LaTeX is used to typeset mathematical symbols and equations found in Doxygen comments. 
To make full LaTeX installation type:

	sudo port install texlive +full
	
Doxygen uses LaTeX to typeset mathematical symbols and equations found in speacially formatted comments. 
To make full LaTeX installation type:

	sudo port install texlive +full
	
To extract information from the software and generate the HTML documentation type:

	doxygen Doxyfile 2>error.txt
	open DoxygenDoc/html/index.html
	
The example is for macOS Terminal with `zsh` shell. The file *error.txt* contains errors found
in parsing specially formatted software comments and markdown (*.md*) files.

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
	
The example is for macOS. The test program is to appear.
