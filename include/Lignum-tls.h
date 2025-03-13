#ifndef LIGNUMTLS_H
#define LIGNUMTLS_H

#include <QSMreader.h>
#include <Kite.h>
#include <VoxelSpace.h>
#include <VoxelBox.h>




class SetQinInLeaves {
 public:
 SetQinInLeaves(VoxelSpace* vsp) : space(vsp) {;}
  
  TreeCompartment<HwQSMSegment_k,HwQSMBud_k>* operator()
    (TreeCompartment<HwQSMSegment_k,HwQSMBud_k>* tc)const {
    if(HwTreeSegment<HwQSMSegment_k,HwQSMBud_k,Kite>* ts =
       dynamic_cast<HwTreeSegment<HwQSMSegment_k,HwQSMBud_k,Kite>*>(tc)){
      list<BroadLeaf<Kite>*> leaf_list = GetLeafList(*ts);
      std::list<BroadLeaf<Kite>*>::iterator Il;
      for(Il = leaf_list.begin(); Il != leaf_list.end(); Il++) {
	Point p = GetCenterPoint(**Il);
	VoxelBox box =  space->getVoxelBox(p);
	SetValue(**Il, LGAQin, box.getQin());
	vector<int> ii = space->getBoxIndexes(p);
      }
    }
    return tc;
  }
 private:
  VoxelSpace* space;
};

#endif
