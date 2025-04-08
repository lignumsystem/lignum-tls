///\file TLSTree.h
///Tree segment and bud for lignum-tls project.
///
///Enhance implementations as required during  model development.
#ifndef TLSTREE_H
#define TLSTREE_H
#include <Lignum.h>
#include <Kite.h>

using namespace Lignum;
namespace LignumTLS{
  
  class TLSBud;
  ///\brief TLSSegment tree segment for lignum-tls project.
  ///
  ///Currently HwTreeSegment with Kite leaf.
  class TLSSegment: public HwTreeSegment<TLSSegment,TLSBud,cxxadt::Kite>{
  public:
    ///\brief Constructor, identical in HwTreeSegment
    ///\param p Base point of the segment
    ///\param d Direction of the segment
    ///\param go Gravelius order
    ///\param l Segment length
    ///\param r Segment radius
    ///\param rh Segment heartwood radius
    ///\param tree Tree the segment belongs to
    TLSSegment(const Point& p,const PositionVector& d, const LGMdouble go,const METER l,
	       const METER r,const METER rh,Tree<TLSSegment,TLSBud>* tree)
      :HwTreeSegment(p,d,go,l,r,rh,tree){}

  };

  ///\brief TLSBud bud for lignum-tls project.
  ///
  ///Currently Bud with TLSSegment and TLSBud.
  class TLSBud: public  Bud<TLSSegment,TLSBud>{
  public:
    ///\brief Constructor, identical in Bud
    ///\param p  Position of the bud
    ///\param d Direction of the bud
    ///\param go Gravelius order
    ///\param tree Tree the bud belongs to
    TLSBud(const Point& p, const PositionVector& d, const LGMdouble go, Tree<TLSSegment,TLSBud>* tree)
      :Bud(p,d,go,tree){}
  };
}
#endif
