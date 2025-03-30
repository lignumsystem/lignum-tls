#ifndef LIGNUMTLS_H
#define LIGNUMTLS_H

#include <QSMreader.h>
#include <Kite.h>
#include <VoxelSpace.h>
#include <VoxelBox.h>


namespace LignumTLS{
  template <class TS, class BUD> 
  class SetQinInLeaves {
  public:
    SetQinInLeaves(VoxelSpace* vsp) : space(vsp) {;}
  
    TreeCompartment<TS,BUD>* operator()
      (TreeCompartment<TS,BUD>* tc)const {
      if(HwTreeSegment<TS,BUD,Kite>* ts =
	 dynamic_cast<HwTreeSegment<TS,BUD,Kite>*>(tc)){
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
}
#endif
