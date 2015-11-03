#!/usr/bin/env python

from numpy import loadtxt,pi
import pylab as pl
from sys import argv

theta,b,c,d = loadtxt(argv[1],usecols=(0,1,2,3),unpack=True)

theta = theta*180/pi

ax = pl.gca()

#ax.plot(theta,b,'g',linewidth=2,label='Transmitted SV')
ax.plot(theta,b,'g',linewidth=2,label='Reflected SV')
ax.plot(theta,c,'b',linewidth=2,label='Reflected P')
ax.plot(theta,d,'r',linewidth=2,label='Transmitted P')

ax.set_xlabel('Incident Angle[Degree]')
ax.set_ylabel('Reflection/Transmission Coefficients')

#pl.legend(loc='upper left')
pl.legend(loc='lower left')
ax.minorticks_on()

ax.figure.set_size_inches(3.5,8,forward=True)
pl.tight_layout()
pl.show()
