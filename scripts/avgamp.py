#!/usr/bin/env python

from plotsection import GetStream
from wfutils import get_max_amp
import argparse
import numpy as np

if __name__ == '__main__':

    parser = argparse.ArgumentParser()
    parser.add_argument('-d',nargs=1,
            required=True,metavar='DIR',help='sacfile directory.')

    args = parser.parse_args()

    path = args.d[0]
    st = GetStream(path)

    tr_num = len(st)

    amp = np.empty(tr_num)

    for i,tr in enumerate(st):

        mind = get_max_amp(tr,'t4',pre=5.0,post=5.0)
        amp[i] = tr.data[mind]

    avg = np.average(amp)

    std = np.std(amp)

    print("amp,std:{0:.3f} {1:.3f}".format(avg,std))
