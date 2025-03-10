import argparse
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d
import farquhar

plt.rcParams['text.usetex'] = True

def farquharplot(num:int):
    #Create vectorized version of the Farquhar model
    frqhr = farquhar.PyFarquhar()
    vfrqhr = np.vectorize(frqhr.Al)
    #Create meshgrid, temperature and photon flux density
    tsamp = np.linspace(-35,50,num)
    qsamp = np.linspace(0,1200,num)
    (TSAMP,QSAMP) = np.meshgrid(tsamp,qsamp)
    #Call vectorized Farquhar model
    res = vfrqhr(TSAMP,QSAMP)
    #Testing ordinary vs vectorized Farquhar
    #print(frqhr.Al(40,200),vfrqhr(40,200))
    resmin = res.min()
    #print(resmin)
    resmax = res.max()
    #print(resmax)
    #Check for NaN results
    #nan_count = np.count_nonzero(np.isnan(res))
    #print(nan_count)

    #3D figure
    #(fig,axis)=plt.subplots(subplot_kw=dict(projection='3d'))
    fig = plt.figure()
    axis=fig.add_subplot(projection='3d')
    fig.tight_layout()
    vticks = np.linspace(resmin,resmax,10, endpoint=True)
    axis.contourf(TSAMP,QSAMP,res,zdir='z',offset=resmin,cmap='Spectral')
    surface = axis.plot_surface(TSAMP,QSAMP,res,rstride=8,cstride=8,alpha=0.8,cmap='Spectral')
    surface.set_clim(resmin,resmax)
    fig.colorbar(surface,ax=axis,format='%.2f',shrink=0.5,pad=0.1,ticks=vticks)
    fig.suptitle(r'$\mathrm{A_l}$: net $\mathrm{CO}_2$ assimilation rate')
    axis.set(xlabel='T (\N{DEGREE SIGN}C)', ylabel=r'Q ($\mathrm{\mu mol\cdot m^{-2}\cdot s^{-1}}$)',
             zlabel=r'$\mathrm{A_l}$ ($\mathrm{\mu mol\cdot m^{-2}\cdot s^{-1}}$)')
    plt.show()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter,
        description="Visualize the Farquhar model implemented the project")
    parser.add_argument("-n","--num",dest="n",type=int,default=1000,help="Plot smoothness, number of samples in numpy.linspace")
    args = parser.parse_args()
    farquharplot(args.n)
