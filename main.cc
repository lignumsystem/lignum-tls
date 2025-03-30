#include <Lignum.h>
#include <LGMHDF5File.h> 
#include <HDF5VoxelData.h> 
#include <VoxelSpace.h>
#include <XMLTree.h>
#include <QSMreader.h>
#include <TLSTree.h>
#include <Lignum-tls.h>
#include <Farquhar.h>

using namespace LignumTLS;

void Usage() {
  cout << endl;
  cout << "Usage: ./lignum-tls <input_tree>" << endl;
  cout << endl;
}




int main(int argc, char** argv) {
  //Photosynthesis model
  Farquhar P;
  //HDF5 dataset for VoxelSpace
  LGMHDF5File h5file("TestFile.h5");
  //Farquhar photosynthesis demonstration: 20 Celsius, 5 second time step
  FarquharPhotosynthesis<TLSSegment,TLSBud,Kite> frqhrP(P,20,5);
  if(argc < 2) {
    Usage();
    exit(0);
  }

  // 1) Read the tree from a file

  Tree<TLSSegment,TLSBud> lignum_tree(Point(0,0,0), PositionVector(0,0,1.0));
  string tree_file = argv[1];
  XMLDomTreeReader<TLSSegment,TLSBud,Kite> tree_reader;
  tree_reader.readXMLToTree(lignum_tree, tree_file);


  double LA = 0.0;
  LA = Accumulate(lignum_tree, LA, CollectLeafArea<TLSSegment,TLSBud,Kite>());
  
  cout << "Tree " << tree_file << " read in, its characteristics are: " << endl;
  cout <<  "Dbh: " << 100*GetValue(lignum_tree,LGADbh) << " cm,  Dbase: "
       << 100*GetValue(lignum_tree,LGADbase) << " cm,  H: " << GetValue(lignum_tree,LGAH)
       << " m,  Leaf area: " << LA << " m2" << endl;


  // 2) VoxelSpace around the tree(s)

  //Firmament is part of VoxelSpace. Its radiation values will be se later
  Firmament sky(6, 8, 1200.0);   //(no_incl, no_azim, rad_plane)
  
  double box_xyz = 0.4;
  //Only box side lengths box_xyz and sky are here important
  VoxelSpace vs(Point(0,0,0),Point(1.0,1.0,1.0), box_xyz, box_xyz, box_xyz,
		5, 5, 5, sky);
  
  BoundingBox bb;
  FindHwBoundingBox<TLSSegment,TLSBud,Kite> fb;
  bb = Accumulate(lignum_tree, bb, fb);

  Point ll = bb.getMin();
  Point ur = bb.getMax();
  vs.resize(ll, ur);
  vs.reset();

  cout << endl;
  cout << "Lower left corner of VoxelSpace  " << ll << endl;
  cout << "Upper right corner of VoxelSpace " << ur << endl;

  DumpHwTree(vs, lignum_tree);


  // 3) Radiation values of the VoxelSpace

  double diffuse_radiation = 1200.0;
  double direct_radiation = 0.0;
  vector<double> direct_dir = {0.0,0.0,1.0};
  
  vs.setDiffuseSkyRadiation(diffuse_radiation);
  vs.setDirectSkyRadiation(direct_radiation);
  vs.setDirectRadiationDirection(direct_dir);

  
  // 4) Calculate light

  cout << endl << "CALCULATING LIGHT" << endl << endl;

  bool use_border_forest = false;
  bool own_box_shading = false;
  vs.calculateTurbidLight(use_border_forest, own_box_shading);


  
  //Record Qin values from voxelboxes to leaves

  ForEach(lignum_tree, SetQinInLeaves<TLSSegment,TLSBud>(&vs) );

  //Calculate photosyhtnesis
  ForEach(lignum_tree,frqhrP);
  vs.writeVoxBoxesToFile("box_data.txt", false);  //Write only boxes with foliage

  return 0;
}
