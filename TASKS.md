# LIGNUM-tls

A lignum project using TLS data to simulate crown-level GPP.

## Table of Contents
- [LIGNUM-tls](#lignum-tls)
  - [Table of Contents](#table-of-contents)
  - [Simulation framework](#simulation-framework)
    - [Repeat for all TLS trees:](#repeat-for-all-tls-trees)
    - [For each time step for the chosen time period:](#for-each-time-step-for-the-chosen-time-period)
    - [After simulation:](#after-simulation)
    - [Finally:](#finally)
  - [Task list](#task-list)
    - [Open questions](#open-questions)
    - [Preliminary tasks](#preliminary-tasks)
    - [Setting up the lignum tree (test tree 1302)](#setting-up-the-lignum-tree-test-tree-1302)
    - [Setting up the firmament model](#setting-up-the-firmament-model)
    - [Setting up the photosynthesis model (Poplar Farquhar)](#setting-up-the-photosynthesis-model-poplar-farquhar)
    - [Setting up the main loop in lignum](#setting-up-the-main-loop-in-lignum)
    - [Further analysis](#further-analysis)
    - [Writing](#writing)
  - [Workflow overview](#workflow-overview)

## Simulation framework
### Repeat for all TLS trees:

1. Read TLS tree
   - Can be done
2. Read the meteorological data
   - Hannah could obtain the data
   - Use poplar weather data file for the time being?<br>
     (Until the meterological data becomes available)
3. Create voxel space around the tree
   - Can be done

### For each time step for the chosen time period:

4. Configure Firmament using the meteorological data
   - Use poplar weather data temporarily?
5. Calculate radiation environment / absorbed radiation for each leaf
   - What is the status for Kite leaf?
6. Calculate photosynthesis
   - Can be done
	 - Farquhar model implemented
	 - FarquharPhotosynthesis calculates photosynthesis for each leaf
   - Hannah could establish species specific parameters for Farquhar model
7. Collect necessary data from the tree
   - Hannah could design data tables necessary
   - 2D and 3D tables/matrices can be saved to HDF5 file (HDF5 accepts N-dimensional tables).
   
### After simulation:

8. Save the collected tree data to an HDF5 file
   - 2D and 3D tables/matrices can be saved to HDF5 file
9. Save VoxelSpace status to an HDF5 file (Voxels with leaves)
   - RESOLVED: Framework to collect VoxelSpace data implemented.<br>
     Extend the implementation as necessary (most likely just by adding data fields to the struct VoxelSpaceData).
   - Is there a need to save VoxeSpace status during simulation?
10. Save other simulation related data required (configuration files etc.) to an HDF5 file (to be able to reproduce results)

### Finally:

11. Analysis of the simulations to answer questions in the  Workflow overview (see the figure in the end) 

Risto's outline for the main program sounds reasonable. The main loop should be rather straightforward
because we are not simulation growth, but the trees are static (?). We might be able to keep the main program 
clean by using generic functions (i.e. ForEach, Accumulate, AccumulateDown, PropagetUp) and hiding details 
behind functions (for example finding the bounding box for a tree and resizing the voxel space).

## Task list

### Open questions

:pushpin: Should the leaf implementation use kites or trianlges? Both shapes can be extrated from .obj files.
- RESOLVED: Kite leaves
  
:pushpin: Modify Lignum Workbench to show kite leaves? (If kites are the way forward).
- DONE: Kite leaves shown
  
:pushpin: Available PAR data is *both* diffuse *and* direct radiation. Can this be distributed in Firmament as SOC (Standard overcast sky)?
- NO need to distribute as SOC: Firmament has methods for direct (sun) radiation and its direction. Sun position can be obtained e.g.from https://soltrack.sourceforge.net if not in the data.

:pushpin: Should lignum-tls have own TreeSegment and Bud instead of HwQSMSegment_k and HwQSMBud_k?

:monocle_face:

### Preliminary tasks

- [x] Delete old things in the repository
- [x] Original .obj leaf data to .csv
- [x] Original branch data to .csv (works with lignumreader)
- [x] Add branch convertion script
- [x] Add leaf conversion script
- [x] Convert all branch data to .csv
- [ ] Convert all leaf data to .csv
- [x] Figure out a good data sharing system (Google drive chosen)
- [ ] Add GEE data extraction scripts
- [x] Add skeleton project to [lignum-system](https://github.com/lignumsystem) to be used by lignum-tls @jari
- [ ] Add all data for test tree to github project
- [ ] Add workflow figure to README

:dizzy: *Outcomes: Set up project, process input data, organise workflow.*

### Setting up the lignum tree (test tree 1302)

- [x] Implemement parallelogram
- [x] Implement kite leaf including XML read and write.
- [x] Read in branch data to lignum tree - Risto
- [x] Read in leaf data (without petioles) to lignum structure @Risto @Jari
- [x] Visual inspection that leaf/branch data is okay
  - [x] Kites and Triangle leaves visible in WB.
- [x] Check orientation of corner points of leaves for petioles, i.e. are they clockwise or anti-clockwise?
      - Hannah: anti-clockwise

:dizzy: *Outcomes: Read tree and leaf geometry, visualise structure*

### Setting up the firmament model

- [x] Input data: Hourly data with [ERA5 Land](https://developers.google.com/earth-engine/datasets/catalog/ECMWF_ERA5_LAND_HOURLY#bands) or daily data with [Breathing Earth Simulator - Radiation](https://developers.google.com/earth-engine/datasets/catalog/SNU_ESL_BESS_Rad_v1#bands)
- [x] Download PAR data from GEE (Google Earth Engine)
- [ ] Convert ERA5 radiation to PAR
- [ ] Decide necessary timestep. 30 minutes, 1 hour or daily?
- [ ] Define growing season for the tree species (or AOI)
  - [ ] Decide growing season length based on research question
- [ ] Extract and set up growing season data input file for Firmament
- [ ] Add radiation regime into skeleton project

:dizzy: *Outcomes: Radiation input data compiled, radiation regime established, add to skeleton project.*

### Setting up the photosynthesis model (Poplar Farquhar)

- [ ] Study poplar big leaf model
- [ ] List input parameters for Farquhar - check poplar SI
- [ ] Compile csv of input parameters
- [x] Draft implementation of Farquhar in Python (check poplar farquhar function)
  - Farquahr implemented on C++
- [ ] Import input file for Firmament
- [ ] Check data before model testing
- [ ] Visualise model results with matplotlib (3d mesh plot)
- [ ] Reimplement in lignum with C++ (Jari + Hannah)
- [ ] Use individual leaves in radiation calculations (test tree 1302 has about 5000 leaves)
- [ ] If time allows compare with the use of leaf area sums and "average" leaf direction in voxels.

:dizzy: *Outcomes: Physiological input data compiled, draft model completeted in python, model added to skeleton project.*

### Setting up the main loop in lignum

- [ ] Combine all components from previous sections (structure, radiation, photosynthesis)
  - [x] Risto: Write a ForEach functor that assigns radiation values from VoxelSpace for all leaves
- [ ] Run simulation
- [ ] Export results using `EvaluateTree`
- [ ] Repeat for all trees

:dizzy: *Outcomes: Simulated GPP per tree, additional functional properties.*

### Further analysis

- [ ] Decide on reasonable architectural analysis metrics. Options include STAR, path fraction etc.
- [ ] Quantify relationship between production and structural metrics

:dizzy: *Outcomes: Project results, statistical analysis, figures.*

### Writing

- [ ] Add latex document
- [ ] Abstract
- [ ] Methods v1
- [ ] Results v1 
- [ ] Figs v1
- [ ] Introduction
- [ ] Methods v2
- [ ] Results v2
- [ ] Figs v2
- [ ] Discussion
- [ ] First draft

:star: ***Outcomes: Finished project :100:*** :star:

## Workflow overview

```mermaid <!-- omit from toc -->

---
config:
  look: neo
  theme: default <!-- omit from toc -->
---
graph TB
sq[Tree geometry] --> ci((Test Tree 1302))
sq2[Radiation regime] --> ci((Test Tree 1302))
sq3[Physiology] --> ci((Test Tree 1302))

subgraph Research questions
    M[**Does architectural efficiency vary with tree size and canopy position?**]
    M --> A[*H1.* Optimal crown architecture is driven by local light conditions]
    M --> B[*H2.* Light interception efficiency decreases with increasing tree size]
end
