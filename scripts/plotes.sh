#!/bin/bash

if [ $# -ne 3 ]
then
	echo "usage:plotes.sh evtlst stalst output"
	exit 0
fi
evt=$1
sta=$2

ps=$3

ps1=station.ps
enum=`awk 'END{print NR}' $evt`
i=0
#eloc=(`awk -F "|" '{print $4,$3}' $evt`)
#elat=${eloc[1]}
#elon=${eloc[0]}


#pscoast -R-25/-12/63/67 -JM6i -Glightgreen -Sblue -A100 -Dc -Ba5/a1 -K \
#-Wthinnest -V > $ps1

#echo "-18.33170 65.02763 $a 2"| psxy -Svt -R -J -V -O -K >> $ps1
#awk -F "," '{print $4,$3}' $sta|psxy -R -J -O -Si0.1i -Gyellow -Wthinnest>>$ps1


pscoast -R-180/180/-70/70 -JM6i -Glightgreen -Sblue -A1000 -Dc -Bg30a30 -K -Wthinnest -V> $ps
#pscoast -Rg -JH6i -Glightgreen -Sblue -A1000 -Dc -Bg30a30 -K -Wthinnest -V> $ps

awk -F "," '{print $4,$3}' $sta|psxy -R -J -O -K -Si0.1i -Gyellow -Wthinnest >>$ps

#awk -F "[,|]" -v v1=$elon -v v2=$elat '{print v1,v2"\n"$4,$3}' $evt $sta|psxy -R -J -O -K -Wthin,black >>$ps
while [ $i -lt $enum ]
do
	i=$((i+1))
	eloc=(`awk -F "|" -v eid=$i 'NR==eid {print $4,$3}' $evt`)
	elat=${eloc[1]}
	elon=${eloc[0]}
	awk -F "," -v v1=$elon -v v2=$elat '{print v1,v2"\n"$4,$3}' $sta |psxy -R \
		-J	-O -K -Wthinnest,black >> $ps
done

awk -F "|" '{print $4,$3}' $evt|psxy -R -J -O -Sa0.1i -Gred -Wthinnest >>$ps
