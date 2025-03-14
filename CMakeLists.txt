# -----------------------------------------
# CMAKE file to compile 'lignum-tls'.
# It can be found in lignum-tls directory.
# -----------------------------------------
# For Unix Makefile build system for Debug:
# mkdir debug
# cd  debug
# cmake .. -DCMAKE_BUILD_TYPE=Debug
# Optionally explicitely set c++ compiler:
# cmake ..  -DCMAKE_BUILD_TYPE=Debug  -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_CXX_FLAGS=-stdlib=libc++
# make install
# ------------------------------------------
# For Unix Makefile build system for Release:
# mkdir release
# cd release
# cmake .. -DCMAKE_BUILD_TYPE=Release
# make install
# ---------------------------------------------
# 1. For Xcode IDE first create Xcode project file:
# mkdir xcode
# cd xcode
# cmake .. -G Xcode
# Open Xcode IDE:
# open lignum-tls.xcodeproj
# 2. Build the 'lignum-tls' Product in Xcode for debugging.
# See also Xcode -> Product (in menu bar) -> Scheme is set to 'lignum-tls' to allow Run to debug.
# Set command line parameters for 'lignum-tls' in Product (menu bar) -> Scheme -> Edit Schmeme -> Arguments.
# Divide the command line into practical parts for debugging from Arguments -> '+'.
# -------------------------------------------------------------------------------
# Graphviz
# Generate dependency graph between the targets and libraries in the lignum-tls project:
# mkdir release
# cd release
# mkdir graphviz
# cmake .. -DCMAKE_BUILD_TYPE=Release --graphviz=graphviz/lignum-tls.dot
# Create pdf file
# dot -Kneato -Goverlap=scale -Tpdf -o lignum-tls.pdf graphviz/lignum-tls.dot
# --------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.30)
# cmake 3.23 for Xcode 13 recommends the policy CMP0114 (warning output)
#cmake_policy(SET CMP0114 NEW)
project(lignum-tls DESCRIPTION "GPP assessment for deciduous TLS/QSM trees")
message("Project  ${CMAKE_PROJECT_NAME}: ${lignum-tls_DESCRIPTION}")
message("-----------------------------------------------------")

# CMake can generate Graphviz files showing the dependencies between the targets in a project
include(CMakeGraphVizOptions)
set(GRAPHVIZ_CUSTOM_TARGETS TRUE)
# Lignum core projects as an ExternalProject. 'lignum-core' target is compiled first
# Note: lignum-core  has to be configured with cmake first. See lignum-core/CMakeLists.txt
include(ExternalProject)

ExternalProject_add(lignum-core
SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..
DOWNLOAD_COMMAND "")
# The '*.cmake' files are produced with 'cmake' configuration for 'lignum-core' (see lignum-core/CMakeLists.txt).  
include(../c++adt/lib/cxxadt.cmake)
include(../Firmament/lib/sky.cmake)
include(../stl-lignum/lib/LGM.cmake)
include(../stl-voxelspace/lib/voxel.cmake)
if(CMAKE_HOST_SYSTEM_NAME MATCHES "Darwin")
  message("Darwin (macOS): Compiling Visual")
  include(../Graphics/lib/Visual.cmake)
endif()

if (CMAKE_HOST_SYSTEM_NAME MATCHES "Darwin")
  message("SYSTEM: Apple Darwin platform")
  # The following line finds Qt4 headers and libraries, qmake must be in PATH
  # find_package(Qt4 COMPONENTS QtOpenGL QtXml QtGui QtCore REQUIRED)
  # The following line finds Qt5 headers and libraries for Xml and Core packages
  # with C/C++ language bindings. The HL component means High Level bindings. 
  find_package(Qt5 COMPONENTS Xml Core  REQUIRED)
  # HSF5 may be uselful when collecting simulation results
  find_package(HDF5 COMPONENTS C CXX HL REQUIRED)
  # Simple visualization tool in macOS 
  find_package(OpenGL REQUIRED) 
  # cmake 3.23 output erroneusly prints 'glut not found'. It is found. 
  find_package(GLUT REQUIRED)
elseif(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
  message("SYSTEM: Linux platform")
  find_package(HDF5 COMPONENTS C CXX HL REQUIRED)
  find_package(Qt5 COMPONENTS Xml Core  REQUIRED)
else()
  message(${CMAKE_HOST_SYSTEM_NAME} " not supported")
  message(FATAL_ERROR "Exiting cmake")
endif()

#Add more source files here as they are needed
set(SOURCE_FILES main.cc
  src/Farquhar.cc
)

# Main target, the executable 'lignum-tls'.
add_executable(lignum-tls ${SOURCE_FILES})
target_include_directories(lignum-tls PUBLIC . include
  ${CMAKE_CURRENT_LIST_DIR}/../Pine
  ${CMAKE_CURRENT_LIST_DIR}/../stl-voxelspace/include
  ${CMAKE_CURRENT_LIST_DIR}/../XMLTree
  ${CMAKE_CURRENT_LIST_DIR}/../LEngine/include
  ${CMAKE_CURRENT_LIST_DIR}/../QSMreader/include
)

# This add_dependencies for lignum-tls seem to generate necessary time stamps necessary for automatic rebuilds
# of 'lignum-tls' and 'lignum-core' projects for Unix Makefile system, i.e. if a file is changed then
# that file and files depending on it will be recompiled with 'make'.
# Note: 'make clean' is required  before 'make'.
#
# Note: Xcode project created for 'lignum-tls' can compile and rebuild 'lignum-tls' (Product in Xcode) when needed.
# However 'lignum-core' dependencies (Targets in Xcode) is compiled just once ignoring after
# that time stamp based changes in Lignum core project files when rebuilding lignum-tls.
add_dependencies(lignum-tls lignum-core)

if(CMAKE_HOST_SYSTEM_NAME MATCHES "Darwin")
  message("lignum-tls")
  message("------------")
  message("Apple macOS (Darwin) libraries")
  target_link_libraries(lignum-tls
    # Uncomment if HDF5 is put to use
    HDF5::HDF5
    Qt5::Xml Qt5::Core
    GLUT::GLUT OpenGL::GLU OpenGL::GL
    Visual  sky LGM cxxadt voxel
  )
elseif(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
  message("lignum-tls")
  message("------------")
  message("Linux libraries")
  target_link_libraries(lignum-tls
    # Uncomment if HDF5 is put to use
    HDF5::HDF5 
    Qt5::Xml Qt5::Core
    sky LGM cxxadt voxel
  )
  message("REMEMBER to type \"make install\" to install lignum-tls") 
else()
  message(${CMAKE_HOST_SYSTEM_NAME} " not supported")
  message(FATAL_ERROR "Exiting cmake")
endif()
add_custom_command(TARGET lignum-tls
  COMMAND echo "Compilation lignum-tls done"
  POST_BUILD
  COMMENT "REMEMBER to type \"make install\" to install \"lignum-tls\" into the working directory above."
  )
install(TARGETS lignum-tls DESTINATION "${CMAKE_CURRENT_LIST_DIR}")
