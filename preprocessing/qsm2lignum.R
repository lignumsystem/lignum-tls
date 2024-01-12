##------------------------------------------------
# Name: qsm2lignum.R
# Desc: Script to convert matlab treeqsms to a
#       lignum readable csv
# Author: Hannah O'Sullivan
#
# Inputs: ../data/opt_qsms/ (831 QSMs)
# Outputs: ../results/lignum_trees/*.csv
#
##------------------------------------------------

# Clear env
rm(list=ls())

# Turn off plots
dev.off()

# Install/load packages
require(tidyverse)
require(R.matlab)
require(ITSMe)

##-----------------------
## Loop though qsm trees
##-----------------------

# Define input and output folders
input_folder <- "../data/qsm_opt/"
output_folder <- "../results/lignum_trees/"

# Create file list
file_list <- list.files(path=input_folder, pattern = ".mat")

# Loop through files 
for (i in file_list){
  # Get filename
  file_name <- sub("^(.*?)-.*", "\\1", i)
  
  # Read QSM
  qsm <- read_tree_qsm(path = paste0(input_folder, i),
                       version = "2.0",
                       global = FALSE)
  
  # Extract columns 
  rad <- qsm$cylinder$radius
  
  len <- qsm$cylinder$length
  
  start_point <- qsm$cylinder$start
  start_point_x <- start_point[,1]
  start_point_y <- start_point[,2]
  start_point_z <- start_point[,3]
  
  
  axis_dir <- qsm$cylinder$axis
  axis_dir_x <- axis_dir[,1]
  axis_dir_y <- axis_dir[,2]
  axis_dir_z <- axis_dir[,3]
  
  parent_cyl <- qsm$cylinder$parent
  
  child_cyl <- qsm$cylinder$extension
  
  branch_index <- qsm$cylinder$branch
  
  branch_ord <- qsm$cylinder$BranchOrder
  
  branch_pos <- qsm$cylinder$PositionInBranch
  
  added <- qsm$cylinder$added
  
  # Write new df
  lignum_df <- data.frame(rad,len, start_point_x, start_point_y, start_point_z,
                          axis_dir_x, axis_dir_y, axis_dir_z, parent_cyl,
                          child_cyl, branch_index, branch_ord, branch_pos,
                          added)
  # Get name
  file_name <- sub("^(.*?)-.*", "\\1", i)
  
  # Export
  write.csv(lignum_df, paste0(output_folder, "LIG_", file_name, ".csv"), row.names=FALSE)
  
}









