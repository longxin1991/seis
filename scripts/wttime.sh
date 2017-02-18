#!/bin/bash

phase=$1
marker=$2

for sacfile in *.SAC
do
    phase=$1
    deg=`saclst gcarc f $sacfile|awk '{print $2}'`
    evdp=`saclst evdp f $sacfile|awk '{print $2}'`
    ttime=`taup_time -mod prem -h $evdp -ph $phase -deg $deg|\
        awk '{if(NF>9 && NR<7&& NR>4) print $4}'`

    if [[ $phase = 'P' ]] && [[ $ttime = '' ]]
    then
        phase='p'
    elif [[ $phase = 'S' ]] && [[ $ttime = '' ]]
    then
        phase='s'
    fi

    ttime=`taup_time -mod prem -h $evdp -ph $phase -deg $deg|\
        awk '{if(NF>9 && NR<7&& NR>4) print $4}'`
    #echo "$evdp $deg $ttime" 
    echo -e "r $sacfile\nch $marker $ttime\nwh\nq\n"
    echo -e "r $sacfile\nch $marker $ttime\nwh\nq\n"|sac
    #echo $ttime

done
