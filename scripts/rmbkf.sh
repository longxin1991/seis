#!/bin/bash

evtlst=$1
full=$2

evtn=`cat $evtlst|wc -l`
i=0

while [ $i -lt $evtn ]
do
	i=$((i+1))
	evtid=`awk -v v1=$i 'NR==v1 {print $1}' $evtlst`

	if [ -d $evtid ]
	then
		filenum=`ls -l ${evtid}/sac| grep "^-" | wc -l`
		if [ $filenum = "0" ]
		then
			echo "$evtid" >> empty.txt
			continue
		fi

		for sacfile in ${evtid}/sac/*.SAC
		do
			#npts=`echo "r $sacfile;mes &1,npts;quit"|sac`
			npts=(`saclst npts f $sacfile`)
			npts=${npts[1]}
			if [ ${npts} -lt $full ]
			then
				echo "$evtid"
				echo "remove `basename $sacfile` npts=$npts"
				rm $sacfile
			fi
		done
	fi
done
