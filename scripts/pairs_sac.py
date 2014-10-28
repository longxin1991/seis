#!/usr/bin/python

import os
import sys
import re
import subprocess

DIR='/home/longxin/data/pairs_data'
dirlist=os.listdir(DIR)
freq="0.005 1 2 4"
flag=''
if len(sys.argv)==2:
	flag=sys.argv[1]
os.chdir(DIR)

for pair in dirlist :
	os.chdir(pair)
	print pair
	flist=os.listdir('.')
	
	for item in flist :
		if os.path.exists('out')==False:
			os.mkdir('out')
		
		if re.match(".*mseed",item) :
			base=item.split('.mseed')[0]
			if flag=='e' :
				cmd="mseed2sac {0} -m {1}.metadata".format(item,base)
				print cmd
				os.system(cmd)
			elif flag=='r' :
				h=base.split('_')
				dis=h[3]
				elat=h[1]
				elon=h[2]
				sta=h[0]
				out=os.popen('ls *.SAC')
				sacfiles=out.read()
				sacfiles=sacfiles.strip().split('\n')
			
				for sac in sacfiles:
					if re.match(".*"+sta+".*",sac):
						print "process file {0}".format(sac)
						p=subprocess.Popen(['sac'],\
						stdout=subprocess.PIPE,\
						stdin=subprocess.PIPE,\
						stderr=subprocess.STDOUT)
					
						scmd='''
echo on
r {3}
chnhdr GCARC {0} EVLA {1} EVLO {2}
taper
rtr
trans from evalresp to none freq {4}
trans to WWSP
w out/{3}
quit
'''.format(dis,elat,elon,sac,freq)
						sacout=p.communicate(scmd)
					
						print sacout[0]
			else :
				print "usage:pairs_sac [switch]"
				exit(0)
	os.chdir(DIR)