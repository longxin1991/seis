#!/usr/bin/python
#scripy used to find event in GCMTT Catlog ndk file.
#by longxin 
#first version 2014.10.29

import datetime
import sys
import os
import re

argc=len(sys.argv)
catlog="/home/longxin/info/CMT/1976_2014.ndk"

if argc != 11 :
    print "usage:findcmt.py -s -e -m min:max -lat min:max -lon min:max"
    print '''
    -s:search start time
    -e:search end time 
    -m:event magnitude
    -lat:event latitude
    -lon:event longtitude
    '''
    exit(0)

for i in range(1,argc):
    if sys.argv[i][0] == '-':
        if sys.argv[i][1] == 's':
            start=sys.argv[i+1]
        elif sys.argv[i][1] == 'e':
            end=sys.argv[i+1]
        elif sys.argv[i][1] == 'm':
            mag=sys.argv[i+1]
            magmin = float(mag.split(':')[0])
            magmax = float(mag.split(':')[1])
        elif sys.argv[i][1:] == 'lat' :
            lat = sys.argv[i+1]
            latmin = float(lat.split(':')[0])
            latmax = float(lat.split(':')[1])
        elif sys.argv[i][1:] == 'lon' :
            lon = sys.argv[i+1]
            lonmin = float(lon.split(':')[0])
            lonmax = float(lon.split(':')[1])


fp=open(catlog,"r")
z=datetime.timedelta(seconds=0)
timeformat='%Y/%m/%d %H:%M:%S.%f'
start=datetime.datetime.strptime(start,"%Y-%m-%dT%H:%M:%S")
end=datetime.datetime.strptime(end,"%Y-%m-%dT%H:%M:%S")
i=0

while True:
    i +=1
    event=fp.read(81*5)
    
    if event == '' or event == '\n':
        break

    evtinfo=event.strip().split('\n')
    hyperinfo=evtinfo[0].split()

    if not re.match("[A-Z]+",hyperinfo[0]):
        evla=float(hyperinfo[2])
        evlo=float(hyperinfo[3])
        emag=float(hyperinfo[5])
        dt=hyperinfo[0]+' '+hyperinfo[1]
    else:
        evla=float(hyperinfo[3])
        evlo=float(hyperinfo[4])
        emag=float(hyperinfo[6])
        if hyperinfo[2][6]=='6':
            tmp=list(hyperinfo[2])
            tmp[6]='5'
            tmp[7]='9'
            tmp[9]='9'
            hyperinfo[2]=''.join(tmp)

        dt=hyperinfo[1]+' '+hyperinfo[2]
    
    dt=datetime.datetime.strptime(dt,timeformat)

    if start-dt<z and end-dt>z and magmin-emag<0 and magmax-emag>0 and \
            latmin-evla<0 and latmax-evla>0 and lonmin-evlo<10 and \
            lonmax-evlo>0 :
                cmt=evtinfo[3].split()
                dt=dt.isoformat()
                output="{0} {1} {2} {3} {4} {5} {6} {7}"
                print output.format(dt,cmt[1],cmt[3],cmt[5],cmt[7],cmt[9],\
                        cmt[11],cmt[0])
