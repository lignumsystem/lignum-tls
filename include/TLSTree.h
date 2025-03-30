#ifndef TLSTREE_H
#define TLSTREE_H
#include <Lignum.h>
#include <Kite.h>
///\file TLSTree.h
///\brief Tree segment and bud for lignum-tls project.
///
///Enhance implementations as required during  model development.
using namespace Lignum;
namespace LignumTLS{
  
  class TLSBud;
  ///\brief TLSSegment tree segment for lignum-tls project.
  ///
  ///Currently HwTreeSegment with Kite leaf.
  class TLSSegment: public HwTreeSegment<TLSSegment,TLSBud,cxxadt::Kite>{
  public:
    TLSSegment(const Point& p,const PositionVector& d, const LGMdouble go,const METER l,
	       const METER r,const METER rh,Tree<TLSSegment,TLSBud>* tree)
      :HwTreeSegment(p,d,go,l,r,rh,tree){}

  };

  ///\brief TLSBud bud for lignum-tls project.
  ///
  ///Currently Bud with TLSSegment and TLSBud.
  class TLSBud: public  Bud<TLSSegment,TLSBud>{
  public:
    TLSBud(const Point& p, const PositionVector& d, const LGMdouble omega, Tree<TLSSegment,TLSBud>* tree)
      :Bud(p,d,omega,tree){}
  };
}
#endif
