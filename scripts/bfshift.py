#!/usr/bin/python

import os
import sys
import string
import subprocess
from math import pi,sin,cos

if len(sys.argv)!=2:
    print "missing sacfile dir."
    exit(0)
else:
    sacdir=sys.argv[1]

sacfiles=os.listdir(sacdir)
os.chdir(sacdir)

baz=124.05*pi/180
clat=33.65
clon=-117.009
p=2.4
ux=p*sin(baz)
uy=p*cos(baz)

print "{0} {1}".format(ux,uy)
for file in sacfiles:
    print "shift time for file {0}".format(file)
    p=subprocess.Popen('sac',stdout=subprocess.PIPE,\
            stdin=subprocess.PIPE,\
            stderr=subprocess.STDOUT)

    out=os.popen("saclst stla f {0}".format(file))
    slat=out.read()
    slat=slat.split()[1]

    out=os.popen("saclst stlo f {0}".format(file))
    slon=out.read()
    slon=slon.split()[1]

    out=os.popen("delay {0:.6f} {1:.6f} {2} {3} {4:.6f} {5:.6f}".format(clat,clon,slat,slon,ux,uy))
    delay=out.read()
    delay=delay.strip()

    delay=string.atof(delay)

    scmd='''
    echo on
r {0}
w ../shift/{0}
r ../shift/{0}
ch allt {1}
wh
q
'''.format(file,delay)

    sacout=p.communicate(scmd)
    print sacout[0]
