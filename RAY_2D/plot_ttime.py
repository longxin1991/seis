#!/usr/bin/env python

import pylab as pl
import numpy as np


deg1,tbc1 = np.loadtxt('t_bc.txt',usecols=(0,1),unpack=True)
deg2,tbc2 = np.loadtxt('t_bc4.txt',usecols=(0,1),unpack=True)

deg3,tdf1 = np.loadtxt('t_df.txt',usecols=(0,1),unpack=True)
deg4,tdf2 = np.loadtxt('t_df4.txt',usecols=(0,1),unpack=True)

fig = pl.figure(figsize=(10,4))

ax1 = pl.subplot(121)
ax1.plot(deg1,tbc1,'k--',linewidth=2.0,label='PREM')
ax1.plot(deg2,tbc2,'r',linewidth=2.0,label='With Heterogeneities')

ax1.set_xlim(145,153)
ax1.set_xlabel('Distance[$\degree$]')
ax1.set_ylabel('Time[s]')
ax1.set_title('BC')
ax1.minorticks_on()
ax1.legend(loc='upper left',shadow=True,fancybox=True)


ax2 = pl.subplot(122)
ax2.plot(deg3,tdf1,'k--',linewidth=2.0,label='PREM')
ax2.plot(deg4,tdf2,'r',linewidth=2.0,label='With Heterogeneities')

ax2.set_xlim(145,153)
ax2.set_xlabel('Distance[$\degree$]')
ax2.set_ylabel('Time[s]')
ax2.set_title('DF')
ax2.minorticks_on()
ax2.legend(loc='upper left',shadow=True,fancybox=True)

fig.tight_layout()
#pl.show()
fig.savefig('ttime_04.pdf',bbox_inches='tight')
