#!/bin/bash


if [ $# -ne 3 ]
then
	echo "usage:fetchresp.sh stalst respdir metadir"
	exit 0
fi
stalst=$1
respdir=$2
metadir=$3
#time="-s 2014-01-01T00:00:00 -e 2014-12-31T00:00:00 "
time="-s 2011-01-01T00:00:00 -e 2011-12-31T00:00:00 "
#time="-s 1994-12-01T00:00:00 -e 2015-06-16T00:00:00 "
#time="-s 2009-01-01T00:00:00 -e 2011-12-31T00:00:00 "


if [ ! -d $respdir  ] 
then
	mkdir $respdir
fi

if [ ! -d $metadir ]
then
	mkdir $metadir
fi

n=`cat $stalst|wc -l`
i=0
while [ $i -lt $n ]
do
	i=$((i+1))
	sta=`awk -v v1=$i -F"," 'NR==v1 {print $2}' ${stalst}`
	cmd="FetchData -S $sta -C BHZ $time -m ${metadir}/${sta}.meta -sd \
		$respdir "
	echo $cmd
	${cmd}

done
