#!/usr/bin/env python
# -*- coding: utf-8 -*-


import numpy as np
import argparse
import pylab as pl


if __name__ == '__main__' :

    parser = argparse.ArgumentParser()
    parser.add_argument('-f',nargs=1,
            required=True,metavar='FILE',help='amp data file.')

    args = parser.parse_args()

    res,err,dis,bla,blo = np.loadtxt(args.f[0],usecols=(1,2,3,4,5),unpack=True)
    
    #pl.plot(dis,amp,'ro')
    
    fig=pl.figure()
    ax=fig.gca()

    np = len(res);
    for i in range(np):
        if bla[i] < 0 :
            mmfc = 'green'
        else:
            mmfc = 'black'
        ax.errorbar(blo[i],res[i],xerr=0,yerr=err[i],marker='o',mfc=mmfc,fmt='o',color='blue')
    #ax.errorbar(blo,res,xerr=0,yerr=err,marker='o',mfc='black',fmt='o')

    ax.set_ylabel('PKiKP/PcP Travel Time Residual[s](PREM)')
    ax.set_xlabel(u'Longitude[°]')
    ax.set_xlim(-181,181)
    ax.set_ylim(-3.5, 3.5)
    ax.grid(True)
    pl.savefig('time_res.eps',bbox_inches='tight')
    #pl.show()
