#!/bin/bash
#对每个事件产生符合要求的台站列表，文件格式如下
#台站名，震中距，台站纬经度，射线拐点纬经度
#每个列表文件命名格式如下
#事件ID_时间发生时间_事件纬经度

dmin=$1
dmax=$2
evtfile=~/data/events
stafile=~/data/GSN.stations

i=0
j=0
enum=$(awk 'END {print NR}' $evtfile)
snum=$(awk 'END {print NR}' $stafile)


while [ $i -lt $enum ]
do
	i=$((i+1))
	evt=`awk -v var=$i 'NR==var' $evtfile`
	einfo=(${evt//|/ })
	edp=${einfo[5]}
	ela=${einfo[3]}
	elo=${einfo[4]}
	edate=(${einfo[1]//// })
	ey=${edate[0]}
	emo=${edate[1]}
	ed=${edate[2]}
	etime=(${einfo[2]//:/ })
	eh=${etime[0]}
	emi=${etime[1]}
	es=${etime[2]}
	etf="${ey}-${emo}-${ed}_${eh}:${emi}:${es}"
	
	filename="${einfo[0]}_${etf}_${ela}_${elo}"
	evtmemo=~/data/pairs/$filename
	
	echo "For event $etf"
	touch $evtmemo
	
	while [ $j -lt $snum ]
	do
		j=$((j+1))
		sta=`awk -v var=$j 'NR==var' $stafile`
		sinfo=(${sta//,/ })
		sla=${sinfo[2]}
		slo=${sinfo[3]}
		sna=${sinfo[1]}
		nna=${sinfo[0]}
		
		dinfo=(`distaz $sla $slo $ela $elo`)
		dis=${dinfo[0]}
		
		comp=`echo "${dis} <= ${dmax} && ${dis} >= ${dmin}"|bc`
 		
		if [ $comp -ne 0 ]
		then
		taup_pierce -mod prem -ph PKiKP -evt $ela $elo -sta $sla $slo -turn | tail -n 1|\
		awk -v v1=$sna -v v2=$dis -v v3=$sla -v v4=$slo '{print v1,v2,v3,v4,$4,$5}' >>$evtmemo
		fi
	
	done
	
	j=0
	
	block=(`ls -s $evtmemo`)
	if [ $block -eq 0 ]
	then
	rm $evtmemo
	echo "not found stations for this event,remove empty file."
	fi

done
