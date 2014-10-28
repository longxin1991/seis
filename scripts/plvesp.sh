#!/bin/bash

if [ $# -ne 2  ]
then
	echo "usage:plvesp.sh xyzfile psfile"
	exit 0
fi

xyzfile=$1
ps=$2
sp=0.025
#get grid limitation
mimx=`minmax ${xyzfile}`

tlimt=`echo $mimx|awk '{print $5}'`
tmp=(`echo ${tlimt//// }`)
tmin=`echo ${tmp[0]#<}`
tmax=`echo ${tmp[1]%>}`

slimt=`echo $mimx|awk '{print $6}'`
tmp=(`echo ${slimt//// }`)
smin=`echo ${tmp[0]#<}`
smax=`echo ${tmp[1]%>}`

elimt=`echo $mimx|awk '{print $7}'`
tmp=(`echo ${elimt//// }`)
emax=`echo ${tmp[1]%>}`

#plot
LIMIT=${tmin}/${tmax}/${smin}/${smax}
echo $LIMIT
# echo "blockmean $xyzfile -R$LIMIT -I0.025/$sp -V > temp.xyz"
# echo "surface temp.xyz -T1 -R$LIMIT -I0.025/$sp -Gtemp.grd"
# exit 0
blockmean $xyzfile -R$LIMIT -I0.025/$sp -V > temp.xyz
surface temp.xyz -T1 -R$LIMIT -I0.025/$sp -Gtemp.grd
grdmath temp.grd 2 POW = temp1.grd
grdmath temp1.grd SQRT = vesp.grd

dt=`echo "scale=3;$emax/10"|bc`
makecpt -Crainbow -T0/$emax/$dt -Z > stack.cpt
grdimage vesp.grd -Cstack.cpt -JX16c/10c -R -Ba10/a0.4WSne -P -V >$ps
