#!/usr/bin/env python

from numpy import loadtxt,pi,where,diff,nan
import pylab as pl
from sys import argv

theta,b,c,d = loadtxt(argv[1],usecols=(0,1,2,3),unpack=True)

theta = theta*180/pi

ax = pl.gca()

pos1 = where(abs(diff(b))>=0.5)
pos2 = where(abs(diff(c))>=0.5)
pos3 = where(abs(diff(d))>=0.5)
b[pos1] = nan
c[pos2] = nan
d[pos3] = nan
ax.plot(theta,b,'g',linewidth=2,label='Transmitted SV')
ax.plot([theta[pos1[0][0]-1],theta[pos1[0][0]+1]],[b[pos1[0][0]-1],b[pos1[0][0]+1]],'g--',linewidth=2)
#ax.plot(theta,b,'g',linewidth=2,label='Reflected SV')
ax.plot(theta,c,'b',linewidth=2,label='Reflected P')
ax.plot([theta[pos2[0][0]-1],theta[pos2[0][0]+1]],[c[pos2[0][0]-1],c[pos2[0][0]+1]],'b--',linewidth=2)
ax.plot(theta,d,'r',linewidth=2,label='Transmitted P')
ax.plot([theta[pos3[0][0]-1],theta[pos3[0][0]+1]],[d[pos3[0][0]-1],d[pos3[0][0]+1]],'r--',linewidth=2)

ax.set_xlabel('Incident Angle[Degree]')
#ax.set_ylabel('Reflection/Transmission Coefficients')
ax.set_ylabel('Phase shift')

pl.legend(loc='upper left')
#pl.legend(loc='lower left')
ax.minorticks_on()

ax.figure.set_size_inches(3.5,8,forward=True)
pl.tight_layout()
pl.show()
