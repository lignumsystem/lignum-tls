# LIGNUM-tls
LIGNUM model to create and analyse radiation climate for tree models created 
from Lidar/QSM trees.

## Compilation
The CMake system is used with the LIGNUM system.

## Prerequisites

Download and compile the *lignum-core* project. Download *QSMreader* and *lignum-tls* under
the *lignum-core* project (i.e. under the lignum-core directory). This is assumed in the CMakeLists.txt files.

## Compile lignum-tls
To compile the `lignum-tls` binary type in the command line:

    cd lignum-tls
    mkdir release
    cd release
    cmake .. -DCMAKE_BUILD_TYPE=Release
    make install

You should see the `lignum-tls` binary in the lignum-tls directory. See the *CMakeLists.txt* file 
for details, for example to generate XCode project for debugging. 
