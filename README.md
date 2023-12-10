# LIGNUM-tls

This repository contains the code for the project: *A TLS based model for assessing crown-level light microenvironments on forest stand productivity*

## Current folder contents
* process-qsms
  * Folder containing qsm processing tools
 
## Task list 

### Prelimiary tasks
- [ ] Delete old things in the repository
- [ ] Original .obj leaf data to .csv
- [x] Original bracnh data to .csv (works with lignumreader) 
- [ ] Add branch and leaf convertion scripts
- [ ] Convert all leaf/branch data to .csv (loop)
- [ ] Figure out radiation data sharing
      
### Setting up the lignum tree (test tree 1302)
- [x] Implemement parallelogram 
- [ ] Implement parallelogram leaf - Jari
      - It seems Parallelogram needs to be changed to Kite shape.
      - Should not be difficult implement basic geometry computations (use existing Triangle if needed).
- [x] Read in branch data to lignum tree - Risto
- [ ] Read in leaf data (without petioles) to lignum structure - Risto + Jari
- [ ] Visual inspection that leaf/branch data is okay
- [ ] If needed, check orientation of corner points of leaves for petioles, i.e. are they clockwise or anti-clockwise? - Hannah 

By the the second week of January

### Setting up the firmament model (i.e. 30 minute time step as in poplar) 
- [ ] Input data: ERA5 hourly/ MODIS PAR (https://developers.google.com/earth-engine/datasets/catalog/ECMWF_ERA5_LAND_HOURLY#bands OR https://developers.google.com/earth-engine/datasets/catalog/MODIS_061_MCD18C2)
- [ ] Convert ER5 radiation to PAR (unless using modis)
- [ ] Download PAR data from MODIS from GEE 
- [ ] Total diffuse radiation
- [ ] Set up growing season data input .txt file for firmament

By the end of January

### Set up photosynthesis model (farquhar)
- [ ] Make the first implementation of farquhar model in python (check poplar farquhar function)
- [ ] Use input .txt file from firmament
- [ ] Check data before running function/plotting
- [ ] Visualise results with matplotlib (3d mesh plot)
- [ ] Reimplement in lignum with C++ (Jari + Hannah)

By the second week of February

### Set up the main loop in lignum 
- [ ] Combine different components from previous sections
- [ ] Analyse results of interest
- [ ] Compare photosynthetic production to structural metrics of each tree
- [ ] Study poplar big leaf model (and reimplement) no. of leaves could be approx. 8000

By the end of February
