#!/bin/bash
gmtset LABEL_FONT_SIZE = 12p ANNOT_FONT_SIZE_PRIMARY = 12p

if [ $# -ne 2 ]
then
	echo "usage:pltrace.sh xyzdir output."
	exit 0
else
	xyzdir=$1
	output=$2
fi

cd $xyzdir

#for sac in *.SAC
#do
#	tmp=(`echo ${sta//./ }`)
#	sta=${tmp[1]}
#	sac2xyz $sac ${sta}.xyz 
#done
#exit 0
#找出最大和最小震中距

dmax=`awk 'FNR==1 {print $2}' *.xyz | awk 'BEGIN{max=0} {if($1>max) max=$1} \
	END{print max}'`

dmin=`awk 'FNR==1 {print $2}' *.xyz | awk 'BEGIN{min=180} {if($1<min) min=$1} \
	END{print min}'`
num=`awk 'FNR==1 {print $2}' *.xyz| awk 'END{print NR}'`
ytic=`echo "scale=1;(($dmax-$dmin))/4"|bc`
dmax=`echo "scale=1;($dmax + $ytic/1.5)"|bc`
dmin=`echo "scale=1;($dmin - $ytic/1.5)"|bc`


xtic=10
scale=4
i=0

echo "$dmin $dmax $ytic $xtic"
tmin=960
tmax=1000
xpos=`echo "scale=2;$tmax + ($tmax-$tmin)/25 "|bc`
FRAME="-JX3i/5i"
BOX="-R${tmin}/${tmax}/${dmin}/${dmax}"
echo $BOX
psbasemap $FRAME $BOX  -B${xtic}:Time/s:/${ytic}:Distance/deg:\SWen -V -K -P > $output

for file in *.xyz
do
	i=$((i+1))
	tmp=(`echo ${file//./ }`)
	sta=${tmp[0]}
	ypos=`awk 'NR==1 {print $2}' $file`
	echo "$xpos $ypos 8 0 0 LM $sta"|pstext -R -J -B -N -O -K -P>> $output
	if [ $i -eq $num ]
	then
		pswiggle $file -J -R  -Wthinnest,blue -Z${scale} -B -V -O -P >> $output
	else
		pswiggle $file -J -R  -Wthinnest,blue -Z${scale} -B -V -K -O -P >> $output
	fi
done
