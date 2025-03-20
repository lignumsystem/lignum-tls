///\file HDF5VoxelData.h
///\brief Data structure for HDF5 file
///
///Data structure containing important fields
///from VoxelBox that can be saved to HDF5 file
///for further analysis
#ifndef VOXELDATA_H
#define VOXELDATA_H
#include <H5Cpp.h>

namespace LignumTLS{
  ///Data set name for VoxelSpace data 
  const string VOXELSPACEDATA("VoxelSpaceData");
  ///Data set name for VoxelSpaceData attribute
  const string VSDATA_ATTRIBUTE("VoxelSpaceAxes");
  ///Data set attribute names
  const vector<string> VSDATA_ATTRIBUTE_NAMES={"X","Y","Z"}; 
  ///\brief VoxelSpaceData for HDF5 file
  ///
  ///VoxelSpaceData  data members contain data from VoxelSpace. Append all necessary data items.
  ///This data structure will instantiate TMatrix3D<VoxelSpaceData>
  ///\important Keep VoxelSpaceData compatible with CreateVoxelSpaceCompData()
  ///\sa CreateVoxelSpaceCompData()
  struct VoxelSpaceData{
    int x;///< Voxel X-index
    int y;///< Voxel Y-index
    int z;///< Voxel Z-index
    double qabs;///< Absorbed radiation
  };

  ///\brief HDF5 CompType for VoxelSpaceData
  ///
  ///HDF5 compatible data items in VoxelSpaceData that can be used to create HDF5 data set.
  ///\retval comp_data HDF5 CompType 
  ///\important Keep CreateVoxelSpaceCompData() compatible with VoxelSpaceData
  ///\sa VoxelSpaceData
  inline H5::CompType CreateVoxelSpaceCompData(){
    H5::CompType comp_data(sizeof(VoxelSpaceData));
    comp_data.insertMember("X-INDEX",HOFFSET(VoxelSpaceData,x),H5::PredType::NATIVE_INT);
    comp_data.insertMember("Y-INDEX",HOFFSET(VoxelSpaceData,y),H5::PredType::NATIVE_INT);
    comp_data.insertMember("Z-INDEX",HOFFSET(VoxelSpaceData,z),H5::PredType::NATIVE_INT);
    comp_data.insertMember("Absorbed radiation",HOFFSET(VoxelSpaceData,qabs),H5::PredType::NATIVE_DOUBLE);
    return comp_data;
  }
}
#endif
