import argparse
import math
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d
import farquhar

plt.rcParams['text.usetex'] = True

def farquharplot(num:int,cmap:str):
    #Create vectorized version of the Farquhar model
    frqhr = farquhar.PyFarquhar()
    vfrqhr = np.vectorize(frqhr.Al)
    #Create meshgrid, temperature and photon flux density
    xmin = -35
    xmax = 50
    ymin = 0
    ymax = 1200
    tsamp = np.linspace(xmin,xmax,num)
    qsamp = np.linspace(ymin,ymax,num)
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

    #Optimal temperature (near optimal) as dashded line
    near_optimal = 31.5
    Txpoints=np.linspace(near_optimal,near_optimal,100)
    Typoints=np.linspace(ymin,ymax,100)
    Tzpoints=np.linspace(resmin,resmin,100)
    #3D figure
    #(fig,axis)=plt.subplots(subplot_kw=dict(projection='3d'))
    fig = plt.figure()
    axis=fig.add_subplot(projection='3d')
    fig.tight_layout()
    vticks = np.linspace(resmin,resmax,10, endpoint=True)
    axis.axes.set_ylim(ymin=ymin,ymax=ymax)
    axis.axes.set_zlim(zmin=resmin,zmax=math.ceil(resmax+1))
    #Projection on z axis 
    axis.contourf(TSAMP,QSAMP,res,zdir='z',offset=resmin,alpha=0.5,cmap=cmap)
    #axis.contourf(TSAMP,QSAMP,res,zdir='y',offset=ymax,alpha=0.5,cmap=cmap)
    axis.plot(Txpoints,Typoints,Tzpoints,color='black',linestyle='--')
    axis.text(near_optimal-1,ymin-150,resmin,str(near_optimal),zdir="x")
    surface = axis.plot_surface(TSAMP,QSAMP,res,rstride=8,cstride=8,alpha=0.8,cmap=cmap)
    #MatplotLib plot_surface calculates results or Z axis values with the help of quadrilaterals
    #that define the surface. Each Z value is a mean of four vertex values. The set_clim forces
    #explicitly minimum and maximum values that will show at the end points of the colorbar.
    surface.set_clim(resmin,resmax)
    fig.colorbar(surface,ax=axis,format='%.2f',shrink=0.5,pad=0.1,ticks=vticks)
    fig.suptitle(r'$\mathrm{A_l}$: net $\mathrm{CO}_2$ assimilation rate')
    axis.set(xlabel='T (\N{DEGREE SIGN}C)', ylabel=r'Q ($\mathrm{\mu mol\cdot m^{-2}\cdot s^{-1}}$)',
             zlabel=r'$\mathrm{A_l}$ ($\mathrm{\mu mol\cdot m^{-2}\cdot s^{-1}}$)')
    
    plt.show()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter,
        description="Visualize the Farquhar model implemented the project")
    parser.add_argument("-n","--num",dest="n",type=int,default=600,
                        help="Plot smoothness, number of samples in numpy.linspace")
    parser.add_argument("-c","--colormap",dest="c",type=str,default="Spectral",choices=["Spectral","Greys","YlGn"],
                        help="Colormap choices") 
    args = parser.parse_args()
    farquharplot(args.n,args.c)
