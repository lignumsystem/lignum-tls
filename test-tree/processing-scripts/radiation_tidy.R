## -------------------------------------------
## Desc: Script to tidy solar radiation data
## Date: Mar 2025
## Auth: Hannah O'Sullivan
## -------------------------------------------

## TO DO
# What about the quality flags? 
# Are the data continuous? 
# Does anything need gap-filling?

# Clear env
rm(list=ls())
# Set wd
setwd("~/lignum-shared/ET-pres/code")

# Load packages
library(lubridate)
library(tidyverse)
library(knitr)
library(readr)
library(janitor)
library(dplyr)
library(corrplot)
library(suncalc) # for solar geometry 

# First get the file paths
file1 <- "../data/NREL_ww_2017/300402_51.77_-1.34_2017.csv"
file2 <- "../data/NREL_ww_2017/301615_51.77_-1.30_2017.csv"

# Read the first two rows (metadata) from the file
metadata1 <- readLines(file1, n = 2)
metadata2 <- readLines(file2, n = 2)

# Write rows to a new metadata.txt file
# writeLines(metadata1, "metadata1.txt")

# Add a metadata df
# Split the metadata lines by commas
meta_names <- strsplit(metadata1, ",")[[1]]
meta_values <- strsplit(metadata1, ",")[[2]]

# Create a new metadata dataframe
meta_df <- data.frame(Column = meta_names, Units = meta_values, stringsAsFactors = FALSE)

# Read in the NREL data, skipping the first two rows (since they are metadata)
radiation1 <- read_csv("../data/NREL_ww_2017/300402_51.77_-1.34_2017.csv", skip = 2)
radiation2 <- read_csv("../data/NREL_ww_2017/301615_51.77_-1.30_2017.csv", skip = 2)

# Combine dataframes
radiation <- bind_rows(radiation1, radiation2)

# Check dimensions of new df
dim(radiation)

# Make sure we have the columns we expect
print(colnames(radiation))

## -------------------------------------------
## Preliminary tidy up
## -------------------------------------------
#
# * Convert columns to lower case and remove
#   spaces
# * Add datetime, doy and date columns

# Clean up column names and get relevant columns
radiation_clean <- radiation %>%
  clean_names() %>%  # Convert to lowercase and replace spaces
  select(year, month, day, hour, minute, 
         dni, dhi, ghi, 
         clearsky_dni, clearsky_dhi, clearsky_ghi, 
         solar_zenith_angle, cloud_type, temperature, pressure,
         dew_point, relative_humidity, precipitable_water) %>%
  mutate(
    datetime = ymd_hm(paste(year, month, day, hour, minute)),  # Add datetime
    date = as.Date(datetime), # Add date 
    doy = yday(datetime)  # Add day of year
  )

# Convert necessary columns to factors
radiation_clean <- radiation_clean %>%
  mutate(across(c(year, month, day, cloud_type), as.factor))

# Check new df types
glimpse(radiation_clean)

## -------------------------------------------
## Add additional solar geometry vars
## -------------------------------------------
#
# * Solar zenith angle
# * Solar declination
# * Day length
# * Solar hour angle
# * Solar elevation

# First extract lat, long and elevation for study site
lat <- as.numeric(meta_df$Units[meta_df$Column == "Latitude"])
lon <- as.numeric(meta_df$Units[meta_df$Column == "Longitude"])
elev <- as.numeric(meta_df$Units[meta_df$Column == "Elevation"])

# Next we can get the solar elevation
radiation_clean <- radiation_clean %>%
  mutate(
    # Solar elevation angle
    solar_elevation_angle = 90 - solar_zenith_angle
  )

# And solar azimuth...
solar_pos <- getSunlightPosition(date = radiation_clean$datetime, lat = lat, lon = lon)
radiation_clean$solar_azimuth <- (solar_pos$azimuth * 180 / pi + 180) %% 360  # convert from radians to degrees

# Solar declination whilst we're at it (seasonal sun position)
# Formula: δ = 23.45° * sin(360°*(284 + day_of_year)/365)
radiation_clean <- radiation_clean %>%
  mutate(
    solar_declination = 23.45 * sin((360/365) * (284 + doy) * pi/180)
  )

# And day length in hours from latitude and declination
# (For daily light integral later on)
radiation_clean <- radiation_clean %>%
  mutate(
    lat_rad = lat * pi / 180,
    dec_rad = solar_declination * pi / 180,
    day_length = (2 / 15) * acos(-tan(lat_rad) * tan(dec_rad)) * 180 / pi
  )

# And finally the solar hour angle just in case
radiation_clean <- radiation_clean %>%
  mutate(
    hour_decimal = hour + minute / 60,
    solar_time = hour_decimal + (4 * (lon - 0)) / 60, # Assuming GMT as standard meridian (0°)
    solar_hour_angle = 15 * (solar_time - 12)  # degrees, 0 at solar noon
  )

# Check new columns
glimpse(radiation_clean)

## -------------------------------------------
## Unit conversion
## -------------------------------------------
#
# * Watts -> Joules
# * Bars -> Pascals/standard atmosphere

# Convert to MJ/m2
radiation_clean <- radiation_clean %>%
  mutate(
    ghi_MJ = ghi * 0.0018,  #  W/m² to MJ/m² per 30 min timestep 
    dni_MJ = dni * 0.0018,  
    dhi_MJ = dhi * 0.0018,  
    # Convert pressure from mbar to Pascals (Pa)
    pressure_Pa = pressure * 100,
    # Alternatively, get standard atmosphere (1 atm = 101325 Pa)
    pressure_atm = pressure_Pa / 101325
  )

# Check new columns
head(radiation_clean %>% select(ghi, ghi_MJ, dni, dni_MJ, dhi, dhi_MJ, pressure_Pa, pressure_atm))
glimpse(radiation_clean)

## -------------------------------------------
## PAR and PPFD
## -------------------------------------------
#
# * Add PAR (45% of total flux)
# * Add instantaneous PPFD
# * Add integrated PPFD

# Add PAR, instantaneous and integrated PPFD.
radiation_clean <- radiation_clean %>%
  mutate(
    # Instantaneous PAR calculations
    dni_PAR = dni * 0.45,
    dhi_PAR = dhi * 0.45,
    ghi_PAR = ghi * 0.45,
    
    # Instantaneous PPFD (µmol m⁻² s⁻¹)
    dni_PPFD = dni_PAR * 4.6,
    dhi_PPFD = dhi_PAR * 4.6,
    ghi_PPFD = ghi_PAR * 4.6,
    
    # Integrated PPFD (µmol m⁻² per 30 minutes)
    dni_PPFD_int = dni_PPFD * 1800,
    dhi_PPFD_int = dhi_PPFD * 1800,
    ghi_PPFD_int = ghi_PPFD * 1800
  )

## -------------------------------------------
## Daily light integral (DLI)
## -------------------------------------------
#
# * Add PAR (45% of total flux)
# * Add instantaneous PPFD
# * Add integrated PPFD

# Calculate DLI for ghi, dni and dhi
radiation_clean <- radiation_clean %>%
  group_by(date) %>%
  mutate(
    # DLI calculated from GHI integrated PPFD, in mol m⁻² d⁻¹
    ghi_DLI = sum(ghi_PPFD_int, na.rm = TRUE) / 1e6,
    # Optionally, you can also compute DLI for DNI and DHI if needed:
    dni_DLI = sum(dni_PPFD_int, na.rm = TRUE) / 1e6,
    dhi_DLI = sum(dhi_PPFD_int, na.rm = TRUE) / 1e6
  ) %>%
  ungroup()

# Final check through dataset
glimpse(radiation_clean)

# Write new csv
write.csv(radiation_clean,"../data/radiation_clean.csv", row.names = FALSE)
