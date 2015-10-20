#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import numpy as np
from scipy.stats import sem
import pylab as pl


def bin_size(low, high, size):
    n = (high-low)/size
    if high-low > int(n)*size:
        n = int(n) + 1

    return n


def make_bin(xdata, ydata, size):

    x_min = xdata.min()
    #x_max = xdata.max()
    low = int(x_min - 1)
    #high = int(x_max + 1)
    high = 55

    n = bin_size(low, high, size)

    bin_data = np.empty(n)
    bin_x = np.empty(n)
    sems = np.empty(n)

    for i in range(n):

        left = low + size*i
        right = left + size
        print("bin {0:.2f} {1:.2f}".format(left, right))
        bin_member = []
        for j, x in enumerate(xdata):
            if x > left and x < right:
                bin_member.append(ydata[j])
        
        bin_num = len(bin_member)
        if bin_num is not 0 and bin_num is not 1:
            bin_data[i] = np.average(bin_member)
            sems[i] = sem(bin_member)
        elif bin_num is 0:
            bin_data[i] = 0
            sems[i] = None
        elif bin_num is 1:
            bin_data[i] = bin_member[0]
            sems[i] = 0
        bin_x[i] = size/2.0 + left
        print("{0:d} data point. avg:{1:.2f}".format(bin_num, bin_data[i]))

    return bin_x, bin_data, sems

if __name__ == '__main__':

    parser = argparse.ArgumentParser()
    parser.add_argument('-f', nargs=1,
            required=True, metavar='FILE', help='amp data file.')

    args = parser.parse_args()
    amp, dis = np.loadtxt(args.f[0], usecols=(2, 3), unpack=True)

    size = 2
    bin_x, bin_y, sems = make_bin(dis, amp, size)
    fig = pl.figure(1,figsize=(6,6.13))
	
	
    for i, y in enumerate(bin_y):
        if bin_y[i] > 0.00001 and sems[i] is not None:
            #pl.plot(bin_x[i],bin_y[i],'ro')
            pl.errorbar(bin_x[i], bin_y[i], xerr=0, yerr=sems[i], marker='o', mfc='red',
                    ecolor='blue')

    pl.text(40, 0.45, 'Bin size:{0:.2f}'.format(size))
    pl.ylabel('PKiKP/PcP Amplitude Ratio')
    pl.xlabel(u'Distance[°]')
    pl.xlim(5, 55)
    pl.ylim(0, 0.5)
    pl.grid(True)
    pl.savefig("bin.eps", bbox_inches='tight')
    #pl.show()
