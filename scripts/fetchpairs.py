#!/usr/bin/python

import os
import sys
import time
import datetime

argc=len(sys.argv)
pdata='/home/longxin/data/pairs_data'

last=datetime.timedelta(seconds=3600)

if os.path.exists(pdata)==False :
    os.mkdir(pdata)

if argc != 2 :
    print "usage: fetchpairs.py PAIRSFILE"
    exit(0)

pairsfile=sys.argv[1]
fd=open(pairsfile,"r")

pairs=fd.readlines()
fd.close()

os.chdir(pdata)

for p in pairs :
    p=p.strip()
    p=p.split('|')
    p1=p[0]
    p2=p[1]

    p1info=p1.split(' ')
    p2info=p2.split(' ')

    p1t=p1info[1]
    p2t=p2info[1]
    
    p1s=p1info[0]
    p2s=p2info[0]
    
    elat1=p1info[2]
    elon1=p1info[3]
    elat2=p2info[2]
    elon2=p2info[3]
    
    slat1=p1info[4]
    slon1=p1info[5]
    slat2=p2info[4]
    slon2=p2info[5]
    
    out1=os.popen("distaz {0} {1} {2} {3}".format(slat1,slon1,elat1,elon1)).read()
    out2=os.popen("distaz {0} {1} {2} {3}".format(slat2,slon2,elat2,elon2)).read()
    
    dis1=out1.split(' ')[0]
    dis2=out2.split(' ')[0]
    
    p1_t_b=datetime.datetime.strptime(p1t,"%Y-%m-%d_%H:%M:%S.%f")
    p2_t_b=datetime.datetime.strptime(p2t,"%Y-%m-%d_%H:%M:%S.%f")

    p1_t_e=p1_t_b+last
    p2_t_e=p2_t_b+last

    b1=p1_t_b.isoformat()
    e1=p1_t_e.isoformat()
    b2=p2_t_b.isoformat()
    e2=p2_t_e.isoformat()

    print "download pair {1} {0}/{3} {2}".format(b1,p1s,b2,p2s)
    pairdir="{1}_{0}#{3}_{2}".format(b1,p1s,b2,p2s)

    if os.path.exists(pairdir)==False:
        os.mkdir(pairdir)

    os.chdir(pairdir)

    cmd="FetchData -S {0} -C BHZ -s {1} -e {2} -o {0}_{3}_{4}_{5}.mseed -m {0}_{3}_{4}_{5}.metadata -rd . -sd ."
    
    print cmd.format(p1s,b1,e1,elat1,elon1,dis1)
    os.system(cmd.format(p1s,b1,e1,elat1,elon1,dis1))
    
    print cmd.format(p2s,b2,e2,elat2,elon2,dis2)
    os.system(cmd.format(p2s,b2,e2,elat2,elon2,dis2))
    
    os.chdir(pdata)