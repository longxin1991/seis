#!/bin/bash
#plot cross section of p wave radiation pattern
#by longxin
#2014.10.30

if [ $# -lt 2 ]
then
	echo "usage:plpolar.sh datafile [takeoff1 takeoff2] output"
	exit 0
elif [  $# -eq 2 ]
then
	data=$1
	ps=$2
	tkf1=12
	tkf2=9
elif [  $# -eq 3]
then
	data=$1
	ps=$4
	tkf1=$2
	tkf2=$3
fi


mimx=`minmax ${data}`

rlimt=`echo $mimx|awk '{print $6}'`
tmp=(`echo ${rlimt//// }`)
rmax=`echo ${tmp[1]%>}`

psbasemap -R0/360/0/${rmax} -JP6i/90 -B30p/100 -K > $ps
cat ${data} |psxy -R -J -Wthin,red  -O -K>> $ps

echo -e "0 0\n""$tkf1 $rmax"|psxy -R -J -Wthin,blue -O -K >> $ps
echo -e "0 0\n""$tkf2 $rmax"|psxy -R -J -Wthin,blue -O -K >> $ps

a1=`echo "scale=2;${tkf1}+5"|bc`
a2=`echo "scale=2;${tkf2}-10"|bc`
r=`echo "scale=2;${rmax}/1.2"|bc`

pstext -R -J -B -N -O <<EOF >> $ps
$a1 $r 10 0 0 LM PcP
$a2 $r 10 0 0 LM PKiKP
EOF

