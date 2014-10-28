#!/bin/bash

pfile=$1
ps=$2

pn=`awk 'END {print NR}' $pfile`
i=0
#ps的O/K转换标志，判断gmt是否写入结束的作用
off="-K"

pscoast -Rg -JH9i -Glightgreen -Sblue -A1000 -Dc -Bg30 -K -Wthinnest -V> $ps

while [ $i -lt $pn ]
do
	
	i=$((i+1))
	
	if [ $i -eq $pn  ]
	then
		off=" "
	fi
	pair=`awk -v var=$i 'NR==var' $pfile`
	path1=(${pair%|*})
	path2=(${pair#*|})
	
	elat1=${path1[2]}
	elon1=${path1[3]}
	slat1=${path1[4]}
	slon1=${path1[5]}
	
	elat2=${path2[2]}
	elon2=${path2[3]}
	slat2=${path2[4]}
	slon2=${path2[5]}
	
	(echo $elon1 $elat1;echo $slon1 $slat1)|psxy -R -J -O -K -Wthick,red -V>> $ps
	(echo $elon2 $elat2;echo $slon2 $slat2)|psxy -R -J -O -K -Wthick,red -V>> $ps
	
	echo "$elon1 $elat1" | psxy -R -J -O -K -Sa0.1i -Gyellow -Wthin >> $ps	
	echo "$elon2 $elat2" | psxy -R -J -O $off -Sa0.1i -Gyellow -Wthin >> $ps
	
done

