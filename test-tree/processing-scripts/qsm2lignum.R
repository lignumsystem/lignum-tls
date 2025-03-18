#!/usr/bin/env Rscript
# Rscript qsm2lignum.R <input_file_or_folder> [/path/to/output_folder] [limit]

##------------------------------------------------
# Name: qsm2lignum.R
# Desc: Extracts cylinder information from
#       QSM .mat files and converts to .csv
#       files for QSMReader.
# Author: H. O'Sullivan
# Date: Mar 2025
#
# Inputs:
#   input_folder : path/to/qsm_folder (or file.mat)
#   output_folder: (Optional) path/to/output_folder
#   limit        : (Optional) int limit the no.
#                  files to be processed (-5)
# Outputs:
#   LIG_*.csv     : .csv file(s) with cylinder 
#                  information 
##------------------------------------------------

# Clear env and turn off plots
# rm(list=ls())
# dev.off()

# Uncomment below to install packages
#install.packages("optparse")
#install.packages("tidyverse")
#install.packages("devtools")
#library(devtools)
#devtools::install_github("lmterryn/ITSMe", build_vignettes = TRUE)

# First check if the required packages are installed
required_pkgs <- c("tidyverse", "ITSMe", "optparse")
for (pkg in required_pkgs) {
  if (!requireNamespace(pkg, quietly = TRUE)) {
    stop(
      paste("The package", pkg, "is not installed.\n",
            "Please install it first and try again."),
      call. = FALSE
    )
  }
}

# Load packages
library(optparse)
library(tidyverse)
library(ITSMe)

# Set up cmd line arguments
option_list <- list(
  make_option(c("-i", "--input"), type="character", default=NULL,
              help="Path to input file or folder (required)", metavar="character"),
  make_option(c("-o", "--output"), type="character", default="branch_results",
              help="Path to output folder [default: %default]", metavar="character"),
  make_option(c("-l", "--limit"), type="integer", default=NA,
              help="Limit number of files to process", metavar="integer")
)

opt_parser <- OptionParser(option_list=option_list)
opts <- parse_args(opt_parser)

# Make sure input is provided
if (is.null(opts$input)) {
  print_help(opt_parser)
  stop("Error: --input must be provided.", call.=FALSE)
}

input_path <- opts$input
output_folder <- opts$output
file_limit <- opts$limit

if (!is.na(file_limit)) {
  cat("Limit: ", file_limit, "\n")
}

# Check if input is a folder or file
if (dir.exists(input_path)) {
  # For folders list all .mat files
  file_list <- list.files(path = input_path, pattern = "\\.mat$", full.names = TRUE)
} else if (file.exists(input_path)) {
  # For files check .mat extension
  if (!grepl("\\.mat$", input_path)) {
    stop("Input file does not have a '.mat' extension: ", input_path)
  }
  file_list <- input_path
} else {
  stop("The input path does not exist: ", input_path)
}

# Apply file limit if needed
if (!is.na(file_limit) && length(file_list) > abs(file_limit)) {
  file_list <- head(file_list, abs(file_limit))
}

# Create the output folder if it doesn't exist
if (!dir.exists(output_folder)) {
  cat("Output folder does not exist, creating new folder.\n")
  dir.create(output_folder, recursive = TRUE)
}

# Get the no. of .mat files to process
num_files <- length(file_list)
cat("Converting", num_files, "QSM file(s).\n")

if (num_files == 0) {
  stop("No .mat files found in the input path. Exiting.")
}

# Set up progress bar
pb <- txtProgressBar(min = 0, max = num_files, style = 3)
count <- 0

# Loop through qsms in file list 
for (i in file_list) {
  # Progress increment
  count <- count + 1
  setTxtProgressBar(pb, count)

  # Get id from filename 
  file_name_full <- basename(i)
  file_name <- sub("^(.*?)-.*", "\\1", file_name_full)

  # Read qsm
  qsm <- read_tree_qsm(
    path = i,
    version = "2.0",
    global = FALSE
  )

  # Extract columns
  rad <- qsm$cylinder$radius
  len <- qsm$cylinder$length
  
  # Cylinder starting point
  start_point <- qsm$cylinder$start
  start_point_x <- start_point[, 1]
  start_point_y <- start_point[, 2]
  start_point_z <- start_point[, 3]
  
  # Cylinder axis direction
  axis_dir <- qsm$cylinder$axis
  axis_dir_x <- axis_dir[, 1]
  axis_dir_y <- axis_dir[, 2]
  axis_dir_z <- axis_dir[, 3]

  # Cylinder order
  parent_cyl <- qsm$cylinder$parent
  child_cyl <- qsm$cylinder$extension
  branch_index <- qsm$cylinder$branch
  branch_ord <- qsm$cylinder$BranchOrder
  branch_pos <- qsm$cylinder$PositionInBranch
  added <- qsm$cylinder$added

  # Create new df for cylinder information
  lignum_df <- data.frame(
    rad, len,
    start_point_x, start_point_y, start_point_z,
    axis_dir_x, axis_dir_y, axis_dir_z,
    parent_cyl, child_cyl, branch_index,
    branch_ord, branch_pos, added
  )

  # Construct output filename
  output_file <- file.path(output_folder, paste0("LIG_B", file_name, ".csv"))

  # Write csv
  write.csv(lignum_df, output_file, row.names = FALSE)
}

close(pb)

cat("\nDone!\n")
