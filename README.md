# LIGNUM-tls

This repository contains the code to model GPP based on TLS data

## Current folder contents
 
## Task list 

### Prelimiary tasks
- [x] Delete old things in the repository
- [x] Original .obj leaf data to .csv
- [x] Original branch data to .csv (works with lignumreader) 
- [] Add branch convertion script
- [ ] Add leaf conversion script
- [x] Convert all branch data to .csv
- [ ] Convert all leaf data to .csv
- [x] Figure out a good data sharing system (Google drive chosen)
      
### Setting up the lignum tree (test tree 1302)
- [x] Implemement parallelogram
- [x] Implemented Kite leaf including XML read and write.
      The final testing can be done when TLS trees with Kite leaves are vailable.
- [x] Read in branch data to lignum tree - Risto
- [ ] Read in leaf data (without petioles) to lignum structure - Risto + Jari     
- [ ] Visual inspection that leaf/branch data is okay
- [x] Check orientation of corner points of leaves for petioles, i.e. are they clockwise or anti-clockwise?
      - Hannah: anti-clockwise        

Thus: there should be data files available to construct the Tree 1302 with Kite leaves in Lignum.

### Setting up the Firmament model
- [ ] Subdaily time step needed (e.g. 30 minute time step)
  - [ ] Convert daily PAR to 30 minute timestep - Hannah
- [x] Input data: ERA5 hourly/ MODIS PAR (https://developers.google.com/earth-engine/datasets/catalog/ECMWF_ERA5_LAND_HOURLY#bands OR https://developers.google.com/earth-engine/datasets/catalog/MODIS_061_MCD18C2)
~~- [ ] Convert ERA5 radiation to PAR (unless using MODIS)~~
- [x] Download PAR data from MODIS satellite hosted by GEE (Google Earth Engine) 
- [ ] PAR data is *both* diffuse *and* direct radiation. Can this be distributed in Firmament as SOC (Standard overcast sky)?
- [ ] Define growing season for the tree species (or AOI) - Hannah 
- [ ] Extract and set up growing season data input file for Firmament - Hannah

### Set up photosynthesis model (Poplar Farquhar)
~~- [ ] Two options for Farquhar model: Optimality and the one in poplar.~~
- [ ] Make the first implementations of Farquhar models in Python (check poplar farquhar function)
- [ ] Use input file for Firmament
- [ ] Other data and parameters for Farquhar models - check poplar SI 
- [ ] Check data before model testing 
- [ ] Visualise model results with matplotlib (3d mesh plot)
- [ ] Reimplement in lignum with C++ (Jari + Hannah)
- [ ] Use individual leaves in radiation calculations (test tree 1302 has about 5000 leaves)
- [ ] If time allows compare with the use of leaf area sums and "average" leaf direction in voxels.

### Set up the main loop in lignum 
- [ ] Combine different components from previous sections
- [ ] Analyse results of interest
- [ ] Compare photosynthetic production to structural metrics of each tree
- [ ] Study poplar big leaf model (and reimplement) no. of leaves could be approx. 8000

## Writing
- [ ] Abstract
- [ ] Methods v1
- [ ] Figs v1
- [ ] Introduction
- [ ] Methods v2
- [ ] Figs v2
- [ ] Discussion
- [ ] First draft

## Organisation 
- [ ] Prioritise tasks
- [ ] Set deadlines based on priority

