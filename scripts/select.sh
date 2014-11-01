#!/bin/bash

RTDIR=~/data
evtlst=$1
trash=~/trash

if [ $# -ne 1 ]
then
	echo "usage:select.sh evtlst"
	exit 0
fi

if [ -d $RTDIRi ]
then
	cd $RTDIR
else
	echo "cd to data dir fail."
	exit 0
fi

evtn=`awk 'END{print NR}' $evtlst`
i=0

while [ $i -lt $evtn  ]
do
	i=$((i+1))
	evtid=`awk -v v1=$i 'NR==v1 {print $1}' $evtlst`

	if [ -d $evtid ]
	then
		cd ${evtid}/out
		echo "cd event data dir ${evtid}/out."
		filenum=`ls -l | grep "^-" | wc -l`
		
		if [ $filenum = "0" ]
		then
			echo "no file,skip event $evtid."
			continue
		fi
		
		for sacfile in *.SAC
		do

sac <<EOF
r $sacfile
fileid l ul
title "dist: &1,gcarc az: &1,az"
qdp off
ppk
q
EOF
			echo "delete $sacfile?y/n"
			echo "or skip current event selection?/s/"
			read choice

			if [ "$choice" = "n" ]
			then
				continue
			elif [ "$choice" = "y" ]
			then
				mv $sacfile $trash
			elif [ "$choice" = "s" ]
			then
				echo "skip event $evtid"
				break
			elif [[ "$choice" =~ "" ]]
			then
				continue
			else
				echo "no option $choice"
				exit 0;
			fi
		done
	fi

	cd $RTDIR
done
