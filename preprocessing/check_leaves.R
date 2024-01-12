##--------------------------------------------------------
# Name: check_leaves
# Desc: Script to check the leaf orientation from fanni
#       leaf coordinates (xyz) for lignum reader i.e.
#       which point is the base of the leaf? etc. 
# Author: Hannah O'Sullivan
#
# Inputs: leaf shape csv 
#         (leaf_coords_1302.csv)
# Outputs: Labelled 3D leaf plots
##--------------------------------------------------------

# Clear env
rm(list=ls())

# Turn off plots
dev.off()

# Install and require packages
#install.packages("scatterplot3d")
#install.packages("rgl")

library(scatterplot3d)
library(rgl)

# Define kite ploygon plotting function
plotKite <- function(x, y, z) {
  # Get vertices for the polygon
  vertices <- rbind(cbind(x[1], y[1], z[1]),
                    cbind(x[2], y[2], z[2]),
                    cbind(x[3], y[3], z[3]),
                    cbind(x[4], y[4], z[4]),
                    cbind(x[1], y[1], z[1]))
  
  # Draw kite and make it look vaguely leaf like
  polygon3d(vertices, col = "green")
  
  # Plot and label points
  for (i in 1:4) {
    points3d(x[i], y[i], z[i], col = "red", size = 4)
    text3d(x[i], y[i], z[i] + 0.02, texts = paste("point", i), cex = 0.9)
  }
}

# Import test leaf file
leaves <- read.csv("../leaf_coords_1302.csv", header = TRUE, sep = ",")

# Subset a few leaves
test <- head(leaves)

## Plotting

# Open 3d plot
open3d()

# To plot all leaves.. 
for (i in 1:nrow(test)) {
  # Extract coordinates for each kite
  xCoords <- as.numeric(test[i, c("x1", "x2", "x3", "x4")])
  yCoords <- as.numeric(test[i, c("y1", "y2", "y3", "y4")])
  zCoords <- as.numeric(test[i, c("z1", "z2", "z3", "z4")])
  
  # Clear plot
  clear3d
  
  # Plot kite
  plotKite(xCoords, yCoords, zCoords)
  
  # Pause to view the kite before moving to the next one
  Sys.sleep(2)  # Adjust the pause duration as needed
}

# To plot a random leaf...
open3d()

# Select the leaf row number (in this case 4)
rowNumber <- 4

# Extract coordinates for the selected leaf
xCoords <- as.numeric(test[rowNumber, c("x1", "x2", "x3", "x4")])
yCoords <- as.numeric(test[rowNumber, c("y1", "y2", "y3", "y4")])
zCoords <- as.numeric(test[rowNumber, c("z1", "z2", "z3", "z4")])

# Plot kite
plotKite(xCoords, yCoords, zCoords)

#dev.off()
