#!/bin/bash

if [ $# -ne 5 ]
then
	echo "usage:sbaz csla cslo dmin dmax evtfile"
	exit 0
fi
sla=$1
slo=$2
dmax=$4
dmin=$3
efile=$5

#n=`awk 'END{print NR}' $efile`
n=`cat $efile|wc -l`
i=0
while [ $i -lt $n ]
do
	i=$((i+1))
	p=`awk -F "|" -v v1=$sla -v v2=$slo -v v3=$i 'NR==v3 {print v1,v2,$3,$4}' $efile`
	baz=`distaz $p | awk '{print $2}'`
	deg=`distaz $p | awk '{print $1}'`
	res=`echo "$deg >= $dmin && $deg <=$dmax "|bc`
	if [[ $res -ne 0 ]]
	then
		awk -v v1=$i 'NR==v1' $efile
		#echo "Baz:$baz  Deg:$deg"
	fi
done
