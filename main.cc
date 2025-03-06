#include <Lignum.h>
#include <QSMreader.h>
#include <VoxelSpace.h>

#include <XMLTree.h>

void Usage() {
  cout << endl;
  cout << "Usage: ./lignum-tls <input_tree>" << endl;
  cout << endl;
}




int main(int argc, char** argv) {

  if(argc < 2) {
    Usage();
    exit(0);
  }

  // 1) Read the tree from a file

  Tree<HwQSMSegment_k,HwQSMBud_k> lignum_tree_hw_k(Point(0,0,0), PositionVector(0,0,1.0));
  string tree_file = argv[1];
  XMLDomTreeReader<HwQSMSegment_k,HwQSMBud_k,Kite> tree_reader;
  tree_reader.readXMLToTree(lignum_tree_hw_k, tree_file);


  double LA = 0.0;
  LA = Accumulate(lignum_tree_hw_k, LA, CollectLeafArea<HwQSMSegment_k,HwQSMBud_k,Kite>());
  
  cout << "Tree " << tree_file << " read in, its characteristics are: " << endl;
  cout <<  "Dbh: " << 100*GetValue(lignum_tree_hw_k,LGADbh) << " cm,  Dbase: "
       << 100*GetValue(lignum_tree_hw_k,LGADbase) << " cm,  H: " << GetValue(lignum_tree_hw_k,LGAH)
       << " m,  Leaf area: " << LA << " m2" << endl;


  // 2) VoxelSpace around the tree(s)

  //Note that the luminosity of the upper hemisphere (=incoming radiation)
  //is included in VoxelSpace
  Firmament sky(9, 12, 1200.0);   //(no_incl, no_azim, rad_plane)
  //Radiation value is only for example calculation
  double box_xyz = 0.2;
  //Only box side lengths box_xyz and sky are here important
  VoxelSpace vs(Point(0,0,0),Point(1.0,1.0,1.0), box_xyz, box_xyz, box_xyz,
		5, 5, 5, sky);
  
  BoundingBox bb;
  FindHwBoundingBox<HwQSMSegment_k,HwQSMBud_k,Kite> fb;
  bb = Accumulate(lignum_tree_hw_k, bb, fb);

  Point ll = bb.getMin();
  Point ur = bb.getMax();
  vs.resize(ll, ur);
  vs.reset();

  cout << endl;
  cout << "Lower left corner of VoxelSpace " << ll << endl;
  cout << "Upper right corner of VoxelSpace" << ur << endl;

  DumpHwTree(vs, lignum_tree_hw_k);

  
  // 3) Calculate light

  cout << endl << "CALCULATING LIGHT" << endl << endl;

  bool use_border_forest = false;
  bool own_box_shading = false;
  vs.calculateTurbidLight(use_border_forest, own_box_shading);  

  vs.writeVoxBoxesToFile("box_data.txt", false);  //Write only boxes with foliage

  return 0;
}
