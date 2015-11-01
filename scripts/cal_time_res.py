#!/usr/bin/env python

from plotsection import GetStream
import argparse
import numpy as np
from scipy.stats import sem
#import scikits.bootstrap as boot

def get_max_time(trace,tmark,pre=0.5,post=0.5) :

    ttime = trace.stats.sac[tmark]
    delta = trace.stats.delta

    tloc = int(ttime/delta)

    b = int(tloc - pre/delta)
    e = int(tloc + post/delta)

    seg = trace.data[b:e]
    rind = seg.argmax()
    
    mtime = (rind + b)*delta

    
    return mtime

if __name__== '__main__' :

    parser = argparse.ArgumentParser()
    parser.add_argument('-d',nargs=1,
            required=True,metavar='DIR',help='sacfile directory.')

    args = parser.parse_args()

    st = GetStream(args.d[0])

    tr_num = len(st)
    pcp_t = np.empty(tr_num)
    pkikp_t = np.empty(tr_num)
    #pcp_tt = np.empty(tr_num)
    #pkikp_tt = np.empty(tr_num)
    res = np.empty(tr_num)

    for i,tr in enumerate(st) :

        pcp_t[i]=get_max_time(tr,'t3')
        pkikp_t[i]=get_max_time(tr,'t4')
        #pcp_tt[i] = tr.stats.sac[t3]
        t1 = tr.stats.sac['t1']
        #pkikp_tt[i] = tr.stats.sac[t4]
        t2 = tr.stats.sac['t2']
        res[i] = pkikp_t[i] - pcp_t[i] - t2 + t1
        #print("{0:.2f} {1:.2f} {2:.2f} {3:.2f} {4:.2f}".format(pcp_t[i],pkikp_t[i],t1,t2,res[i]))

    
    #e = boot.ci(ratios,np.average,alpha=0.05,method='abc',output='errorbar')
    #ratio = pkikp_mean/pcp_mean
    res_mean = np.average(res)
    res_sem = sem(res)
    #down = e[0][0]
    #up = e[1][0]
    dis = tr.stats.sac.gcarc
    #sta = tr.stats.station
    print("{0:.2f} {1:.2f} {2:.4f}".format(res_mean,res_sem,dis)) 
