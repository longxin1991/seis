#!/usr/bin/python
#python script to fetch seimic data from GSN for given station to event distance.
#by longxin
#2014.8.27

import sys
import os
import datetime
import time

EVDIR='/home/longxin/data/IMS/exotic/WRA'
EVENTFILE=EVDIR+'/evtlst.txt'
os.chdir(EVDIR)
loc='00'
argc=len(sys.argv)
"""
Set time window range
"""
pre=0
delay=3600
ISOTIMEFORMAT="%Y-%m-%dT%X"

if argc<5:
	if argc==3:
		stafile=sys.argv[1]
		fd=open(stafile,"r")
		stations=fd.readlines()
		ch=sys.argv[2]
	else:
		print "parameter wrong!"
		exit(0)
elif argc>5:
	print "parameter overflow!"
	exit(0)
else:
	stafile=sys.argv[3]
	ch=sys.argv[4]
	fd=open(stafile,"r")
	stations=fd.readlines()
	fd.close()
"""
Read staion and event files.
"""
fd=open(EVENTFILE,"r")
events=fd.readlines()
fd.close()

for ev in events:
	eitem=ev.strip()
	eitem=eitem.split('|')
	print "\nFetching Data for Event"+eitem[0]
	ela=eitem[2]
	elo=eitem[3]
	edep=eitem[4]
	
	'''
	Calculate start and end time.
	'''
	edatetime=eitem[1].split('.')[0]
	edt=time.strptime(edatetime,"%Y/%m/%d %H:%M:%S")
	edt=datetime.datetime(edt[0],edt[1],edt[2],edt[3],edt[4],edt[5])
	
	tpre=datetime.timedelta(seconds=pre)
	tdelay=datetime.timedelta(seconds=delay)
	start=edt-tpre
	end=edt+tdelay
	#edate=edatetime.split(' ')[0]
	#etime=edatetime.split(' ')[1]
	#ed=edate.split('/')
	#et=etime.split(':')
	
	catlog=eitem[0].strip()
	if os.path.exists(catlog)==False:
		os.mkdir(catlog)
	os.chdir(catlog)
	#os.system('rm -v *.mseed *.metadata sta.txt')
	print "Delete previous fetched mseed and metadata files."
	
#Get information of stations that satisfy the distance condition.
	fd=open('sta.txt',"w+")
	for sta in stations:
		sitem=sta.strip()
		sitem=sitem.split(',')
		sla=sitem[2]
		slo=sitem[3]
		snet=sitem[0]
		sna=sitem[1]
		stb=time.strptime(sitem[5],ISOTIMEFORMAT)
		ste=time.strptime(sitem[6],ISOTIMEFORMAT)
		cmd='distaz'+' '+ela+' '+elo+' '+sla+' '+slo
		stdout=os.popen(cmd)
		out=stdout.read()
		out=out.strip()
		res=out.split(' ')
		if argc==5:
			if(float(res[0])<=float(sys.argv[2]))&(float(res[0])>=float(sys.argv[1])):
				if (int(edt.year)>=int(stb[0]))&(int(edt.year)<=int(ste[0])):
					fd.write(snet+' '+sna+' '+sla+' '+slo+' '+res[0]+'\n')
		else:
			fd.write(snet+' '+sna+' '+sla+' '+slo+' '+res[0]+'\n')
#Fetch data for each event.
	fd=open('sta.txt',"r")
	lines=fd.readlines()
		
	S=start.isoformat()
	E=end.isoformat()
		
	for item in lines:
		item=item.strip()
		item=item.split(' ')
		cmd='FetchData -N '+item[0]+' -S '+item[1]+' -C '+ch+' -s '+S+' -e '+E+' -o '+item[1]+'_'+item[4]+'.mseed'
		print cmd
		os.system(cmd)
	
	'''
	Delete empty files.
	'''
	files=os.listdir('.')
	for item in files:
		stat=os.stat(item)
		if stat[6]==0:
			os.remove(item)
			print "Delete empty file "+item
	fd.close()	
	os.chdir(EVDIR)
