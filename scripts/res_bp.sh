#!/bin/bash

evtlst=tmp.txt
BCAR='-141.8277 63.0617'
BMAR='-144.5271 67.4507'
IMAR='-153.7225 65.9969'
ILAR='-146.8861 64.7716'
YKA='-114.606 62.6059'

NVAR='-118.3037 38.4295'
PDAR='-109.5831 42.7765'
TXAR='-103.6669 29.3337'

ASAR='133.9508 -23.6647'
PSAR='119.8458 -21.5725'
WRA='134.3927 -19.7671'

BURAR='25.2168 47.6148'
GERES='13.7039 48.8363'
EKB='-3.1923 55.3339'

SPB='16.3906 78.1795'
ABKAR='59.9431 49.2556'
KKAR='70.5601 43.1054'
MKAR='82.3003 46.77'

cat res.txt|\
    while read line
    do
        line=($line)
        evtid=${line[0]}
        evla=`grep $evtid $evtlst|awk -F"|" '{print $3}'`
        evlo=`grep $evtid $evtlst|awk -F"|" '{print $4}'`
        
        loc=($TXAR)
        stla=${loc[1]}
        stlo=${loc[0]}
    
        #echo $evla $evlo $stla $stlo
        #exit 0
        bp_loc=`taup_pierce -mod prem -ph PKiKP -evt $evla $evlo -sta \
            $stla $stlo -turn|tail -n 1|awk  '{print $4,$5}'`
        echo "${line[@]} $bp_loc"
        #exit 0
    done
