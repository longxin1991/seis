#!/usr/bin/python

import sys
import math
import os

def findpairs(file1,file2):
	os.chdir('/home/longxin/data/pairs')
	dlim=1.0
	fd1=open(file1,"r")
	evt1=fd1.readlines()
	f1info=file1.split('_')
	f2info=file2.split('_')
	e1lat=float(f1info[3])
	e1lon=float(f1info[4])
	e1dt=f1info[1]+'_'+f1info[2]
	e2lat=float(f2info[3])
	e2lon=float(f2info[4])
	e2dt=f2info[1]+'_'+f2info[2]
	
	fd2=open(file2,"r")
	evt2=fd2.readlines()
	
	fd3=open("../pairs.txt","a")
	
	for s1 in evt1:
		info1=s1.strip()
		info1=info1.split(' ')
		lat1=float(info1[4])
		lon1=float(info1[5])
		s1lat=float(info1[2])
		s1lon=float(info1[3])
		for s2 in evt2:
			info2=s2.strip()
			info2=info2.split(' ')
			lat2=float(info2[4])
			lon2=float(info2[5])
			s2lat=float(info2[2])
			s2lon=float(info2[3])
			latdif=abs(lat2-lat1)
			londif=abs(lon2-lon1)
			
			slatd=abs(s2lat-s1lat)
			slond=abs(s2lon-s1lon)
			elatd=abs(e2lat-e1lat)
			elond=abs(e2lon-e1lon)
			
			if (latdif<=dlim)&(londif<=dlim) :
				#print latdif,londif
				if info1[0] != info2[0] :
					if (slatd >=10.0) | (slond >=10.0) :
						fd3.write("{3} {0} {1:.4f} {2:.4f} {4:.4f} {5:.4f}|{9} {6} {7:.4f} {8:.4f} {10:.4f} {11:.4f}\n".format(e1dt,e1lat,e1lon,info1[0],s1lat,s1lon,e2dt,e2lat,e2lon,info2[0],s2lat,s2lon))
				
	fd1.close()
	fd2.close()
	fd3.close()

DIR='/home/longxin/data/pairs'
pairsfile='home/longxin/data/pairs.txt'
evtlist=os.listdir(DIR)
evtnum=len(evtlist)

if os.path.exists(pairsfile)==True:
	print "remove previous pairsfile."
	os.remove(pairsfile)

for i in range(0,evtnum-1):
	for j in range(i+1,evtnum):
		#print "compare"+evtlist[i]+" and "+evtlist[j]
		findpairs(evtlist[i],evtlist[j])
		