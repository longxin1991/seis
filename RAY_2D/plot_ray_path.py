#!/usr/bin/env python

import numpy as np
import pylab as pl
import mpl_toolkits.axisartist.floating_axes as floating_axes
from matplotlib.projections import PolarAxes
from mpl_toolkits.axisartist.grid_finder import (FixedLocator, MaxNLocator,
                                                 DictFormatter)

#theta,r = np.loadtxt('ray.txt',usecols=(0,1),skiprows=1,unpack=True)

#theta1,r1 = np.loadtxt('ray1.txt',usecols=(0,1),skiprows=2,unpack=True);

fig = pl.figure(figsize=(12,8))

rfp = open('rays_df.txt','r')
lines = rfp.readlines()
ln = len(lines)

def setup_axes2(fig, rect):
    """
    With custom locator and formatter.
    Note that the extreme values are swapped.
    """
    tr = PolarAxes.PolarTransform()

    pi = np.pi
    angle_ticks = [(.5*pi, r"90$\degree$"),
                   (.625*pi, r"112.5$\degree$"),
                   (.75*pi, r"135$\degree$"),
                   (.81111111*pi, r"146$\degree$"),
                   (.84444444*pi, r"152$\degree$"),
                   (pi, r"180$\degree$")]
    grid_locator1 = FixedLocator([v for v, s in angle_ticks])
    tick_formatter1 = DictFormatter(dict(angle_ticks))


    grid_locator2 = MaxNLocator(2)

    grid_helper = floating_axes.GridHelperCurveLinear(
        tr, extremes=(pi, 0.5*pi, 6371, 0),
        grid_locator1=grid_locator1,
        grid_locator2=grid_locator2,
        tick_formatter1=tick_formatter1,
        tick_formatter2=None)

    ax1 = floating_axes.FloatingSubplot(fig, rect, grid_helper=grid_helper)
    ax1.axis["bottom"].major_ticklabels.set_axis_direction("top")
    ax1.axis["left"].toggle(ticklabels=False)
    ax1.axis["left"].toggle(ticks=False)
    ax1.axis["top"].toggle(ticks=False)
    fig.add_subplot(ax1)

    # create a parasite axes whose transData in RA, cz
    aux_ax = ax1.get_aux_axes(tr)

    aux_ax.patch = ax1.patch  # for aux_ax to have a clip path as in ax
    ax1.patch.zorder = 0.9  # but this has a side effect that the patch is
    # drawn twice, and possibly over some other
    # artists. So, we decrease the zorder a bit to
    # prevent this.

    return ax1, aux_ax

ax1,ax = setup_axes2(fig,111)

#ax = fig.gca(projection='polar')

#ax.plot(theta,r,'k',linewidth=1.0)
#ax.plot(theta1,r1,'r',linewidth=1.0)

i=0
while (i<ln):
    npts = int(lines[i])+1
    deg = np.zeros(npts)
    radi = np.zeros(npts)
    for j in range(npts):
        tmp = lines[i+1+j].split()
        deg[j] = float(tmp[0])
        radi[j] = float(tmp[1])
    ax.plot(deg,radi,'r',linewidth=1.0)
    print("plotting ray at {0:.3f}".format(deg[-1]*180/np.pi))
    i = i+npts+1

atheta = np.linspace(0,2*np.pi,100)
#icb
ax.plot(atheta,np.ones(100)*1221.5,'k')
#cmb
ax.plot(atheta,np.ones(100)*3480,'k')

ax.plot(np.linspace(124*np.pi/180,156*np.pi/180,100),np.ones(100)*3780,'k')

th1 = np.linspace(124*np.pi/180,140*np.pi/180,100)
th2 = np.linspace(140*np.pi/180,156*np.pi/180,100)
radi1 = np.ones(100)*3480
radi2 = np.ones(100)*3780


ax.fill_between(th1,radi1,radi2,color='red',alpha=0.4)
ax.fill_between(th2,radi1,radi2,color='blue',alpha=0.4)

ax.set_ylim(0,6371.0)

ax.yaxis.set_ticklabels([])

fig.tight_layout()
fig.savefig('rays_df_146_152.pdf',bbox_inches='tight')
#pl.show()
