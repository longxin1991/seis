#!/usr/bin/env python

from plotsection import GetStream
import argparse
import numpy as np
import scikits.bootstrap as boot

def get_max_amp(trace,tmark,pre=0.5,post=0.5) :

    ttime = trace.stats.sac[tmark]
    delta = trace.stats.delta

    tloc = int(ttime/delta) + 1

    b = int(tloc - pre/delta)
    e = int(tloc + post/delta)

    seg = trace.data[b:e]
    mag = seg.max()

    
    return mag

if __name__== '__main__' :

    parser = argparse.ArgumentParser()
    parser.add_argument('-d',nargs=1,
            required=True,metavar='DIR',help='sacfile directory.')

    args = parser.parse_args()

    st = GetStream(args.d[0])

    tr_num = len(st)
    pcp_mags = np.empty(tr_num)
    pkikp_mags = np.empty(tr_num)
    ratios = np.empty(tr_num)

    for i,tr in enumerate(st) :

        pcp_mags[i]=get_max_amp(tr,'t3')
        pkikp_mags[i]=get_max_amp(tr,'t4')
        ratios[i] = pkikp_mags[i]/pcp_mags[i] 
        #print("{0:.6f} {1:.6f}".format(pcp_mags[i],pkikp_mags[i]))

    pcp_mean = np.average(pcp_mags)
    pkikp_mean = np.average(pkikp_mags)
    r_mean = np.average(ratios)
    
    e = boot.ci(ratios,np.average,alpha=0.05,method='abc',output='errorbar')
    #ratio = pkikp_mean/pcp_mean
    down = e[0][0]
    up = e[1][0]
    dis = tr.stats.sac.gcarc
    sta = tr.stats.station
    print("{0:.3f} {1:.3f} {2:.3f} {3:.3f} {4}".format(down,up,r_mean,dis,sta))
