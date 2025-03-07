#include <Farquhar.h>

namespace LignumTLS{
  
  double Farquhar::Al(double T, double Q)const
  {
    double gamma_star = Gammastar(T);
    double ci = Ci(T);
    double vc = Vc(T,Q);
    double rd = Rd(T);
    return (1-gamma_star/ci)*vc-rd;
  }
  
  double Farquhar::Wc(double T)const
  {
    double ci = Ci(T);
    double vcmax = Vcmax(T);
    double oi = Oi(T);
    return (ci*vcmax)/(ci+Kc*(1.0+oi*Ko));
  }

  double Farquhar::Wj(double T, double Q)const
  {
    double j = J(T,Q);
    double gamma_star = Gammastar(T);
    double ci = Ci(T);
    return j/(4.5+10.5*gamma_star/ci);
  }

  double Farquhar::Vc(double T, double Q)const
  {
    double wc = Wc(T);
    double wj = Wj(T,Q);
    return std::min(wc,wj);
  }
  
  double Farquhar::Gammastar(double T)const
  {
    double vcmax = Vcmax(T);
    double vomax = Vomax(T);
    double oi = Oi(T);
    return (0.5*vomax*Kc*oi)/(vcmax*Ko);
  }
  
  double Farquhar::Oi(double T)const
  {
    return Oa*((4.700e-2-1.3087e-3*T+2.5603e-5*pow(T,2.0)-2.1441e-7*pow(T,3.0))/2.6934e-2);
  }
  
  double Farquhar::Ci(double T)const
  {
    return 0.7*Ca*((1.6740-6.1294e-2*T+1.1688e-3*pow(T,2.0)-8.8741e-6*pow(T,3.0))/0.73547);
  }
  
  double Farquhar::J(double T, double Q)const
  {
    double jmax = Jmax(T);
    return (jmax*Q)/(Q+2.1*jmax);
  }
  
  double Farquhar::Jmax(double T)const
  {
    double p1 = (T-25.0)*65330.0;
    double p2 = 298.0*R*(T+273.0);
    double p3 = 298.0*Sj-Dj;
    double p4 = 298.0*R;
    double p5 = (T+273.0)*Sj-Dj;
    double p6 = R*(T+273.0);
     
    return Jmax25*std::exp(p1/p2)*((1.0+std::exp(p3/p4))/(1.0+std::exp((p5/p6))));
  }
  
  double Farquhar::Rd(double T)const
  {
    return Rd25*std::pow(Q10,(T-25.0)/10.0);
  }

  double Farquhar::Vcmax(double T)const
  {
    return Vcmax25*std::exp( ((T-25.0)*68000.0) / (R*298.0*(273.0+T)) );
  }

  double Farquhar::Vomax(double T)const
  {
    double vcmax = Vcmax(T);
    return 0.21*vcmax;
  }
  
}
