#ifndef FARQUHAR_H
#define FARQUHAR_H
#include <cmath>
#include <list>
#include <algorithm>
#include <Lignum.h>
///\file Farquhar.h
///\brief Farquhar photosynthesis model
///
///Farquhar photosynthesis model as in Lu et al 2011.

namespace LignumTLS{
  ///\brief Farquhar photosynthesis model
  ///
  ///The Farquhar photosynthesis model with its applications are reported for example in the following five  publications:
  ///
  /// -# Lu et al. 2011. Application of the Fuctional-Structural Tree Model LIGNUM to Growth Simulation Short-Rotation Eastern Cottonwood
  /// -# Long. 1991. Modification of the response of photosynthetic productivity to rising temperature by atmospheric CO2 concentrations:
  ///                Has its importance been underestimated?
  /// -# de Pury and Farquhar. 1997. Simple scaling of photosynthesis from leaves to canopies without the errors of big-leaf models.
  /// -# Yin et al. 2004. Extension of a biochemical model for the generalized stoichiometry of electron transport limited C3 photosynthesis.
  /// -# von Caemmerer. 2013. Steady-state models of photosynthesis.
  ///
  ///Albeit different perspectives on the Farquhar photosynthesis model
  ///they share similar development of equations and notations.
  ///
  ///This implementation follows the notation in variable and function names as well as default parameter values
  ///in Lu at al., 2011.
  class Farquhar{
  public:
    ///\brief Constructor with generic and application specific parameters
    ///\param Kc_p Michaelis constant for CO2,  \f$\mathrm{\mu mol\cdot mol^{-1}}\f$
    ///\param Ko_p Michaelis constant for O2,  \f$\mathrm{m mol\cdot mol^{-1}}\f$
    ///\param Ca_p Atmospheric CO2 concentration relative to 25 \f$\mathrm{^\circ C}\f$, \f$\mathrm{\mu mol\cdot mol^{-1}}\f$
    ///\param Oa_p Atmosperic O2 concentration relative to 25 \f$\mathrm{^\circ C}\f$, \f$\mathrm{\mu mol\cdot mol^{-1}}\f$
    ///\param Vcmax25_p The Vcmax value at 25 \f$\mathrm{^\circ C}\f$, \f$\mathrm{\mu mol\cdot m^{-2}\cdot s^{-1}}\f$ 
    ///\param Rd25_p Respiration rate at reference temperature 25 \f$\mathrm{^\circ C}\f$,\f$\mathrm{\mu mol\cdot m^{-1}\cdot s^{-1}}\f$
    ///\param R_p Universal gas constant, \f$\mathrm{J\cdot mol^{-2}\cdot K^{-1}}\f$
    ///\param Q10_p Respiration temperature coefficient, unitless
    ///\param Jmax25_p  Maximum electron transport at 25 \f$\mathrm{^\circ C}\f$, \f$\mathrm{\mu mol\cdot m^{-2}\cdot s^{-1}}\f$
    ///\param Sj_p Electron transport temperature response parameter (entropy term), \f$\mathrm{J\cdot K^{-1} mol^{-1}}\f$
    ///\param Dj_p Curvature parameter (energy of thermal deactivation)  \f$\mathrm{J\cdot mol^{-1}}\f$
    Farquhar(double Kc_p=460.0, double Ko_p=330.0, double Ca_p=380.0, double Oa_p=210.0,
	     double Vcmax25_p=42.12, double Rd25_p=1.09, double R_p=8.314, double Q10_p=1.78,
	     double Jmax25_p=88.01, double Sj_p=650, double Dj_p=2e5):
      Kc(Kc_p),Ko(Ko_p),Ca(Ca_p),Oa(Oa_p),Vcmax25(Vcmax25_p),Rd25(Rd25_p),R(R_p),Q10(Q10_p),Jmax25(Jmax25_p),Sj(Sj_p),Dj(Dj_p){}
    ///\brief Copy constructor
    ///\param frqhr Farquhar model
    Farquhar(const Farquhar& frqhr);
    ///\brief Asignment operator
    ///\param frqhr Farquhar model
    ///\retval *this Farquhar object after assignment
    Farquhar& operator=(const Farquhar& frqhr);
    ///\brief The net CO2 assimilation \e rate, \f$\mathrm{\mu mol\cdot m^{–2}\cdot s^{–1}}\f$
    ///
    /// The net CO2 assimilation \e rate by a leaf with respiration
    ///\important This is the main method that returns the net CO2 assimilation rate, \f$A_l\f$, i.e. photosynthesis minus respiration:
    ///\f[
    ///A_l = (1-\Gamma^\ast/C_i)V_c-Rd
    ///\f]
    ///\param T Temperature, \f$\mathrm{^\circ C}\f$
    ///\param Q  Absorbed photon flux density (both direct and diffuse), \f$\mathrm{\mu mol\cdot m^{–2}\cdot s^{–1}}\f$
    ///\return The net CO2 assimilation \e rate by a leaf, \f$\mathrm{\mu mol\cdot m^{–2}\cdot s^{–1}}\f$
    ///\note To convert the net assimilation \e rate to the net CO2 assimilation of a leaf multiply
    ///      the net assimilation rate by the leaf area and the time step length in seconds.
    double Al(double T, double Q)const;
    ///\brief The gross CO2 assmilation \e rate, \f$ (1-\Gamma^\ast/C_i)V_c \f$
    ///
    /// The gross CO2 assmilation \e rate by a leaf without respiration:  \f$ (1-\Gamma^\ast/C_i)V_c \f$
    ///\param T Temperature, \f$\mathrm{^\circ C}\f$
    ///\param Q  Absorbed photon flux density (both direct and diffuse), \f$\mathrm{\mu mol\cdot m^{–2}\cdot s^{–1}}\f$
    ///\return The Gross CO2 assimilation \e rate by a leaf, \f$\mathrm{\mu mol\cdot m^{–2}\cdot s^{–1}}\f$
    double GrossCO2assimilation(double T, double Q)const;
  protected:
    ///\brief The RuP2 \e saturated rate of carboxylation,  \f$\mathrm{\mu mol\cdot m^{-2}\cdot s^{-1}}\f$
    ///\param T Temperature, \f$\mathrm{^\circ C}\f$
    ///\return The RuP2 \e saturated rate of carboxylation,  \f$\mathrm{\mu mol\cdot m^{-2}\cdot s{^-1}}\f$
    ///\note The equation used is as  in Long 1991, Appendix 1 (Eq. 4): \f$\frac{V_\mathit{cmax}C_i}{C_i+K_c(1+O_i/K_o)}\f$.
    ///The erratum in Lu et al., 2011, Appendix 3 (Eq. A3-7) is in the denominator multiplication: \f$O_i*K_o\f$.
    ///For the first, with the erratum CO2 assimilation rate return values in Farquhar::Al() would be negative.
    ///Secondly, the units used would be incorrect.
    ///\sa Farquhar::Wj
    double Wc(double T)const;
    ///\brief The RuP2 \e limited rate of carboxylation,  \f$\mathrm{\mu mol\cdot m^{-2}\cdot s^{-1}}\f$
    ///\param T Temperature, \f$\mathrm{^\circ C}\f$
    ///\param Q  Absorbed photon flux density (both direct and diffuse), \f$\mathrm{\mu mol\cdot m^{–2}\cdot s^{–1}}\f$
    ///\return The RuP2 \e limited rate of carboxylation,  \f$\mathrm{\mu mol\cdot m^{-2}\cdot s^{-1}}\f$
    ///\sa Farquhar::Wc
    double Wj(double T, double Q)const;
    ///\brief Carboxylation rate, \f$\mathrm{\mu mol\cdot m^{-2}\cdot s^{-1}}\f$
    ///\param T Temperature, \f$\mathrm{^\circ C}\f$
    ///\param Q  Absorbed photon flux density (both direct and diffuse), \f$\mathrm{\mu mol\cdot m^{–2}\cdot s^{–1}}\f$
    ///\return  Carboxylation rate as minimum of \f$\min\{W_c,W_j\}\f$, \f$\mathrm{\mu mol\cdot m^{-2}\cdot s^{-1}}\f$
    ///\sa Farquhar::Wc Farquhar::Wj
    double Vc(double T, double Q)const;
    ///\brief The \f$\Gamma^\ast\f$ function, \f$\mathrm{\mu mol\cdot mol^{-1}}\f$
    ///
    ///The \f$\Gamma^\ast\f$ function for example in Lu et al., 1991. Photosynthesis CO2 compensation point (no daytime respiration). 
    ///\param T Temperature, \f$\mathrm{^\circ C}\f$
    ///\return CO2 compensation point of photosynthesis in the absence of daytime respiration, \f$\mathrm{\mu mol\cdot mol^{-1}}\f$
    double Gammastar(double T)const;
    ///\brief Leaf O2 concentration, \f$\mathrm{\mu mol\cdot mol^{-1}}\f$
    ///\param T Temperature, \f$\mathrm{^\circ C}\f$
    ///\return  Intercellular O2 concentration in a leaf corrected for 25 \f$\mathrm{^\circ C}\f$, \f$\mathrm{\mu mol\cdot mol^{-1}}\f$
    ///\note The equation for is also in  Long 1991
    ///\note The temperature \p T is the leaf temperature in Long 1991
    double Oi(double T)const;
    ///\brief Leaf CO2 concentration, \f$\mathrm{\mu mol\cdot mol^{-1}}\f$
    ///\param T Temperature, \f$\mathrm{^\circ C}\f$
    ///\return Intercellular CO2 concentration in a leaf corrected for 25 \f$\mathrm{^\circ C}\f$, \f$\mathrm{\mu mol\cdot mol^{-1}}\f$
    ///\note The equation is also in Long 1991, Appendix 1 (Eq. 7)
    ///\note The temperature \p T is the leaf temperature, Long 1991
    double Ci(double T)const;
    ///\brief Potential rate of electron transport, \f$\mathrm{\mu mol\cdot m^{–2}\cdot s^{–1}}\f$
    ///
    ///\param T Temperature, \f$\mathrm{^\circ C}\f$
    ///\param Q Absorbed photon flux density (both direct and diffuse), \f$\mathrm{\mu mol\cdot m^{–2}\cdot s^{–1}}\f$
    ///\return Potential rate of electron transport, \f$\mathrm{\mu mol\cdot m^{–2}\cdot s^{–1}}\f$
    ///\sa Farquhar::Jmax
    double J(double T, double Q)const;
    ///\brief Maximum electron transport, \f$\mathrm{\mu mol\cdot m^{-2}\cdot s^{-1}}\f$
    ///
    ///Maximum light saturated electron transport.
    ///\param T  Temperature, \f$\mathrm{^\circ C}\f$
    ///\return Light saturated potential rate of electron transport, \f$\mathrm{\mu mol\cdot m^{-2}\cdot s^{-1}}\f$
    ///\sa Farquhar::J
    ///\note The equation is also in Yin et al., 2004, Appendix A (Eq. A6) and de Pury and Farquhar, 1997. Table 1 (Eq. 10)
    double Jmax(double T)const;
    ///\brief Carboxylation velocity, \f$\mathrm{\mu mol\cdot m^{-2}\cdot s^{-1}}\f$ 
    ///
    ///Maximum RuP2-saturated rate of carboxylation corrected relative to the rate at 25. 
    /// 
    ///\param T Temperature, \f$\mathrm{^\circ C}\f$
    ///\return Maximum RuP2-saturated rate of carboxylation corrected for 25 \f$\mathrm{^\circ C}\f$, \f$\mathrm{\mu mol\cdot m^{-2}\cdot s^{-1}}\f$ 
    ///\sa Farquhar::Vcmax25 Farquhar::R
    ///\note The equation is also in Yin et al 2004, Appendix A (Eq. A5)
    double Vcmax(double T)const;
    ///\brief Oxygenation velocity, \f$\mathrm{\mu mol\cdot m^{-2}\cdot s^{-1}}\f$
    ///
    ///Maximum oxygenation velocity, in terms of Vcmax.
    ///\param T Temperature, \f$\mathrm{^\circ C}\f$
    ///\return 0.21*Vcmax, \f$\mathrm{\mu mol\cdot m^{-2}\cdot s^{-1}}\f$
    ///\note The equation is also in Long 1991
    ///\sa Farquhar::Vcmax
    double Vomax(double T)const;
    ///\brief Leaf respiration rate, \f$\mathrm{\mu mol\cdot m^{-2}\cdot s^{-1}}\f$
    ///
    ///Leaf respiration rate (excluding photorespiration) based on reference temperature 25 \f$\mathrm{^\circ C}\f$
    ///\param T Measured ambient temperature, \f$\mathrm{^\circ C}\f$
    ///\return Leaf respiration rate (dark respiration rate), \f$\mathrm{\mu mol\cdot m^{-2}\cdot s^{-1}}\f$
    ///\sa Farquhar::Q10 (Turnbull et al 2002)
    double Rd(double T)const;
  private:
    double Kc;///< Michaelis constant for CO2,  \f$\mathrm{\mu mol\cdot mol^{-1}}\f$
    double Ko;///< Michaelis constant for O2,  \f$\mathrm{\mu mol\cdot mol^{-1}}\f$
    double Ca;///< Atmospheric CO2 concentration relative to 25 \f$\mathrm{^\circ C}\f$,  \f$\mathrm{\mu mol\cdot mol^{-1}}\f$
    double Oa;///< Atmospheric O2 concentration relative to 25 \f$\mathrm{^\circ C}\f$,  \f$\mathrm{\mu mol\cdot mol^{-1}}\f$
    double Vcmax25;///< The Vcmax value at 25 \f$\mathrm{^\circ C}\f$, \f$\mathrm{\mu mol\cdot m^{-2}\cdot s^{-1}}\f$ 
    double Rd25;///< Respiration rate at reference temperature 25 \f$\mathrm{^\circ C}\f$,\f$\mathrm{\mu mol\cdot m^{-2}\cdot s^{-1}}\f$ 
    double R;///< Gas constant, \f$\mathrm{J\cdot mol^{-1}\cdot K^{-1}}\f$
    double Q10;///< Respiration temperature coefficient, unitless
    double Jmax25;///< Maximum electron transport at 25 \f$\mathrm{^\circ C}\f$, \f$\mathrm{\mu mol\cdot m^{-2}\cdot s^{-1}}\f$
    double Sj;///< Electron transport temperature response parameter (entropy term), \f$\mathrm{J\cdot K^{-1}\cdot mol^{-1}}\f$
    double Dj;///< Curvature parameter (energy of deactivation), \f$\mathrm{J\cdot mol^{-1}}\f$
  };

  ///\brief Apply Farquhar model in a tree
  ///
  ///Functor to traverse the tree with ForEach operator.
  ///It will apply Farquhar model \f$\mathrm{A_l}\f$ for each leaf in a tree
  ///The photosynthesis \f$\mathrm{P_{leaf}}\f$ of a single leaf depends on the leaf area \f$\mathrm{A_{leaf}}\f$
  ///and the time step \f$\mathrm{t}\f$: \f$\mathrm{P_{leaf}=A_{l}(Q_{abs},T)*A_{leaf}*t}\f$
  ///\sa The class LignumTLS::Farquhar
  ///\sa Lignum::ForEach()
  ///\tparam TS Tree segment
  ///\tparam BUD Bud
  ///\tparam LEAF Leaf shape
  template<class TS, class BUD, class LEAF>
  class FarquharPhotosynthesis{
  public:
    ///\brief Default constructor
    ///\param frqhr_p Farquhar model for photosynthesis
    ///\param T_p Temperature (leaf temperature) in Celsius
    ///\param time_step_p Time step in seconds
    FarquharPhotosynthesis(const Farquhar& frqhr_p,double T_p, double time_step_p)
      :frqhr(frqhr_p),T(T_p),time_step(time_step_p){}
    ///\brief Copy constructor
    ///\param frqhrp_p FarquharPhotosynthesis 
    FarquharPhotosynthesis(const FarquharPhotosynthesis& frqhrp_p)
      :frqhr(frqhrp_p.frqhr),T(frqhrp_p.T),time_step(frqhrp_p.time_step){}
    ///\brief Assignment operator
    ///\param frqhrp FarquharPhotosynthesis
    ///\retval *this FarquharPhotosynthesis object after assignment
    FarquharPhotosynthesis& operator=(const FarquharPhotosynthesis& frqhrp);
    ///\brief  Photosynthesis in a tree 
    ///
    ///Apply Farquhar photosynthesis model for each leaf in a tree. 
    ///The photosynthesis \f$\mathrm{P_{leaf}}\f$ of a single leaf
    ///depends on the net CO2 assmilation rate (\f$A_l\f$), true leaf area (\f$\mathrm{A_{leaf}}\f$)
    ///and the time step (\f$\mathrm{t}\f$): \f$\mathrm{P_{leaf}=A_{l}(Q_{abs},T)*A_{leaf}*t}\f$
    ///\param tc Current tree compartment 
    ///\retval tc Current tree compartment 
    ///\note The leaf area (\f$\mathrm{A_{leaf}}\f$) is based on degree of filling (\e dof).
    ///If \e dof is 1 then leaf area equals the area of the geometric shape of the leaf.
    ///\sa LignumTLS::Farquhar::Al()
    ///\sa Lignum::ForEach
    ///\sa Lignum::LGAA
    ///\sa Lignum::BroadLeaf::GetValue()
    TreeCompartment<TS,BUD>* operator()(TreeCompartment<TS,BUD>* tc)const;
  private:
    Farquhar frqhr;///<Farquhar photosynthesis model 
    double T;///< Temperature (leaf temperature) in Celsius
    double time_step;///<Time step in seconds
  };

  template<class TS, class BUD, class LEAF>
  FarquharPhotosynthesis<TS,BUD,LEAF>& FarquharPhotosynthesis<TS,BUD,LEAF>::operator=(const FarquharPhotosynthesis<TS,BUD,LEAF>& frqhrp)
  {
    frqhr = frqhrp.frqhr; T = frqhrp.T; time_step = frqhrp.time_step;
    return *this;
  }
  
  template<class TS, class BUD, class LEAF>
  TreeCompartment<TS,BUD>* FarquharPhotosynthesis<TS,BUD,LEAF>::operator()(TreeCompartment<TS,BUD>* tc)const
  {
    if (HwTreeSegment<TS,BUD,LEAF>* hw_ts = dynamic_cast<HwTreeSegment<TS,BUD,LEAF>*>(tc)){
      std::list<BroadLeaf<LEAF>* >& leaf_ls = GetLeafList(*hw_ts);
      typename std::list<BroadLeaf<LEAF>*>::iterator first;
      //More undestandable to loop leaves than to create another functor for the std::list  
      for (first=leaf_ls.begin(); first != leaf_ls.end(); first++){
	BroadLeaf<LEAF>* leaf = *first;
	//Note leaf area, LGAA, is based on degree of filling (dof).
	//If dof is 1 then leaf area equals the area of the geometric shape of the leaf
	double leaf_area = GetValue(*leaf,LGAA);
	double Qabs = GetValue(*leaf,LGAQabs);
	//Farquhar model photosynthesis
	double P = frqhr.Al(Qabs,T);
	//Leaf photosynthesis for the time step
	double Pleaf = P*leaf_area*time_step;
	SetValue(*leaf,LGAP,Pleaf);
      }
    }
    return tc;
  }
  
}//End lignumtls
#endif
