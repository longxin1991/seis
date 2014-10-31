#!/bin/bash
#Script to plot trace for each event and
#view the expected conherent phases.if the
#expected phases are observed,then record the
#according event to evtmemo file.
#By longxin
#2014.10.31

if [ $# -ne 1 ]
then
	echo "usage:sortawf.sh evtfile."
	exit 0
fi

evtlst=$1
current=`pwd`
rtdir=~/data
normdir=norm
xyzdir=xyz
sacdir=out
psdir=${rtdir}/ps
evtmemo=evtmemo.txt

if [ ! -d $rtdir ]
then
	echo "data dir is not given."
	exit 0
fi

cd ${rtdir}

evtnum=`awk 'END {print NR}' ${evtlst}`
i=0

while [ $i -lt $evtnum ]
do
	i=$((i+1))
	evtid=`awk -v n=$i 'NR==n {print $1}' ${evtlst}`

	cd $evtid
	echo "cd to event $evtid"

	norm.sh out $normdir

	if [ ! -d $normdir ]
	then
		echo "normlized data dir not found."
		continue
	else
		if [ ! -d $xyzdir ]
		then
			mkdir $xyzdir
		fi

		for  sacfile in ${normdir}/*.SAC
		do
			sac=`basename ${sacfile}`
			tmp=(`echo ${sac//./ }`)
			sta=${tmp[1]}
			sac2xyz $sacfile ${xyzdir}/${sta}.xyz
		done
	fi

	if [ ! -d $xyzdir ]
	then
		echo "xyz data dir not found."
		continue
	else
		output=${psdir}/${evtid}.ps
		pltrace.sh $xyzdir $output
	fi
	
	gv $output &>/dev/null
	
	echo "save this event?y/n"
	read choice

	if [ $choice = "y" ]
	then
		echo "$evtid" >> ${rtdir}/$evtmemo
	elif [ $choice = "n" ]
	then
		rm $output
		echo "remove $output"
	fi
	cd $rtdir
done
