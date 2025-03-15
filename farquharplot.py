import argparse
import math
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d
import farquhar

plt.rcParams['text.usetex'] = True

def farquharplot(num:int,cmap:str):
    #Titles
    main_title = "Eastern cottonwood, Lu et al. 2011 \n Photosynthesis submodel"
    netco2title = r"Net $\mathrm{CO}_2$ assimilation rate"
    co2title = r"Gross $\mathrm{CO}_2$ assimilation rate (no respiration)"
    
    #Create vectorized version of the Farquhar model
    frqhr = farquhar.PyFarquhar()
    vfrqhr = np.vectorize(frqhr.Al)
    vco2 = np.vectorize(frqhr.GrossCO2assimilation)
    #Create meshgrid for temperature and photon flux density
    xmin = -35
    xmax = 50
    ymin = 0
    ymax = 1200
    tsamp = np.linspace(xmin,xmax,num)
    qsamp = np.linspace(ymin,ymax,num)
    (TSAMP,QSAMP) = np.meshgrid(tsamp,qsamp)

    #Call vectorized Farquhar model
    res1 = vfrqhr(TSAMP,QSAMP)
    figure_title1 = netco2title 
    res2 = vco2(TSAMP,QSAMP)
    figure_title2 = co2title
    resmin1 = res1.min()
    resmax1 = res1.max()
    resmin2 = res2.min()
    resmax2 = res2.max()
    #Testing ordinary vs vectorized Farquhar
    #print(frqhr.Al(40,200),vfrqhr(40,200))
    #print(resmin1)
    #print(resmax1)
    #Check for NaN results
    #nan_count = np.count_nonzero(np.isnan(res))
    #print(nan_count)

    #Mark pptimal (near optimal) temperature as dashded line
    near_optimal = 31.5
    Txpoints=np.linspace(near_optimal,near_optimal,100)
    Typoints=np.linspace(ymin,ymax,100)
    Tzpoints=np.linspace(resmin1,resmin1,100)
    #Required for the second subfigure
    Tzpoints2=np.linspace(resmin2,resmin2,100)

    #3D figure
    #(fig,axis1)=plt.subplots(subplot_kw=dict(projection='3d'))
    #Width and height in inches
    fig = plt.figure(figsize=(8,5))
    axis1=fig.add_subplot(1,2,1,projection='3d')
    fig.tight_layout()
    fig.suptitle(main_title,multialignment='center')
    #First subplot
    vticks = np.linspace(resmin1,resmax2,10, endpoint=True)
    #vticks = np.linspace(resmin1,resmax1,10)
    axis1.axes.set_ylim(ymin=ymin,ymax=ymax)
    axis1.axes.set_zlim(zmin=resmin1,zmax=math.ceil(resmax1+1))
    #Projection on z axis1 
    axis1.contourf(TSAMP,QSAMP,res1,zdir='z',offset=resmin1,alpha=0.5,cmap=cmap)
    #axis1.contourf(TSAMP,QSAMP,res1,zdir='y',offset=ymax,alpha=0.5,cmap=cmap)
    axis1.plot(Txpoints,Typoints,Tzpoints,color='black',linestyle='--')
    axis1.text(near_optimal-1,ymin-250,resmin1,str(near_optimal),zdir="x")
    surface = axis1.plot_surface(TSAMP,QSAMP,res1,rstride=8,cstride=8,alpha=0.8,cmap=cmap)
    #MatplotLib plot_surface calculates results or Z axis values with the help of quadrilaterals
    #each of which define a subsurface of the plot. Each Z value is a mean of four vertex values.
    #The set_clim forces explicitly minimum and maximum values that will show at the end points of the colorbar.
    surface.set_clim(resmin1,resmax2)
    #fig.colorbar(surface,ax=axis1,format='%.2f',shrink=0.5,pad=0.3,ticks=vticks)
    fig.colorbar(surface,ax=axis1,format='%.2f',shrink=0.3,pad=0.15,location='right',ticks=vticks)
    axis1.set_title(figure_title1,multialignment='center')
    axis1.set(xlabel='T (\N{DEGREE SIGN}C)', ylabel=r'Q ($\mathrm{\mu mol\cdot m^{-2}\cdot s^{-1}}$)',
             zlabel=r'$\mathrm{A_l}$ ($\mathrm{\mu mol\cdot m^{-2}\cdot s^{-1}}$)')

    #Second subplot
    axis2 = fig.add_subplot(1,2,2,projection='3d')
    #vticks = np.linspace(resmin2,resmax2,10, endpoint=True)
    #vticks=np.linspace(resmin2,resmax2,11)
    axis2.axes.set_ylim(ymin=ymin,ymax=ymax)
    axis2.axes.set_zlim(zmin=resmin2,zmax=math.ceil(resmax2+1))
    #Projection on z axis2 
    axis2.contourf(TSAMP,QSAMP,res2,zdir='z',offset=resmin2,alpha=0.5,cmap=cmap)
    #axis2.contourf(TSAMP,QSAMP,res2,zdir='y',offset=ymax,alpha=0.5,cmap=cmap)
    axis2.plot(Txpoints,Typoints,Tzpoints2,color='black',linestyle='--')
    axis2.text(near_optimal-1,ymin-250,resmin2,str(near_optimal),zdir="x")
    surface = axis2.plot_surface(TSAMP,QSAMP,res2,rstride=8,cstride=8,alpha=0.8,cmap=cmap)
    #MatplotLib plot_surface calculates results or Z axis values with the help of quadrilaterals
    #each of which define a subsurface of the plot. Each Z value is a mean of four vertex values.
    #The set_clim forces explicitly minimum and maximum values that will show at the end points of the colorbar.
    surface.set_clim(resmin1,resmax2)
    #fig.colorbar(surface,ax=axis2,format='%.2f',shrink=0.5,pad=0.3,ticks=vticks)
    fig.colorbar(surface,ax=axis2,format='%.2f',shrink=0.3,pad=0.15,location='right',ticks=vticks)
    #fig.suptitle(figure_title,multialignment='center')
    axis2.set_title(figure_title2,multialignment='center')
    axis2.set(xlabel='T (\N{DEGREE SIGN}C)', ylabel=r'Q ($\mathrm{\mu mol\cdot m^{-2}\cdot s^{-1}}$)',
             zlabel=r'$\mathrm{A_{l}^{tot}}$ ($\mathrm{\mu mol\cdot m^{-2}\cdot s^{-1}}$)')
    
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
