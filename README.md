# LIGNUM-tls

This repository contains the code for the project: *A TLS based model for assessing crown-level light microenvironments on forest stand productivity*

## Current folder contents
 
## Task list 

### Prelimiary tasks
- [x] Delete old things in the repository
- [x] Original .obj leaf data to .csv
- [x] Original branch data to .csv (works with lignumreader) 
- [x] Add branch convertion script
- [ ] Add leaf conversion script
- [x] Convert all branch data to .csv
- [ ] Convert all leaf data to .csv
- [x] Figure out a good data sharing system (Google drive chosen)
      
### Setting up the lignum tree (test tree 1302)
- [x] Implemement parallelogram
- [x] Implemented Kite leaf including XML read and write.
      The final testing can be done when TLS trees with Kite leaves are vailable.
- [x] Read in branch data to lignum tree - Risto   
      - Hannah: Risto should have the Tree 1302, branching data a data csv file.
- [ ] Read in leaf data (without petioles) to lignum structure - Risto + Jari   
      - Hannah: Risto should have the Tree 1302, leaf data csv file.   
- [ ] Visual inspection that leaf/branch data is okay
- [x] Check orientation of corner points of leaves for petioles, i.e. are they clockwise or anti-clockwise? - Hannah    
       - Kite points run clockwise, the first being the leaf base (i.e. nadir point, petiole end)         

Thus, there should be data files available to construct the Tree 1302 with leaves.

### Setting up the Firmament model
- [ ] Subdaily time step needed (e.g. 60 minute time steo)
- [x] Input data: ERA5 hourly/ MODIS PAR (https://developers.google.com/earth-engine/datasets/catalog/ECMWF_ERA5_LAND_HOURLY#bands OR https://developers.google.com/earth-engine/datasets/catalog/MODIS_061_MCD18C2)
- [ ] Convert ERA5 radiation to PAR (unless using modis)
- [ ] Download PAR data from MODIS satellite hosted by GEE (Google Earth Engine) 
- [ ] PAR data is *both* diffuse *and* direct radiation. Can this be distributed in Firmament as SOC (Standard overcast sky)?
- [ ] Define growing season for the tree species
- [ ] Extract and set up growing season data input file for Firmament

Hannah will make the files available in GitHub for the test tree and Firmament by the end of the week 18 after Wappu.

### Set up photosynthesis model (Farquhar)
- [ ] Two options for Farquhar model: Optimality and the one in poplar.
- [ ] Make the first implementations of Farquhar models in Python (check poplar farquhar function)
- [ ] Use input file for Firmament
- [ ] Other data and parameters for Farquhar models
- [ ] Check data before running function/plotting
- [ ] Visualise results with matplotlib (3d mesh plot)
- [ ] Reimplement in lignum with C++ (Jari + Hannah)
- [ ] Use individual leaves in radiation calculations (test tree 1302 has about 5000 leaves)
- [ ] If time allows compare with the use of leaf area sums and "average" leaf direction in voxels.

Before midsummer but as soon as possible.

### Set up the main loop in lignum 
- [ ] Combine different components from previous sections
- [ ] Analyse results of interest
- [ ] Compare photosynthetic production to structural metrics of each tree
- [ ] Study poplar big leaf model (and reimplement) no. of leaves could be approx. 8000

