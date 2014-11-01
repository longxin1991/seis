#!/bin/bash
gmtset LABEL_FONT_SIZE = 12p ANNOT_FONT_SIZE_PRIMARY = 12p
gmtset PAPER_MEDIA=a4
if [ $# -lt 2 ]
then
	echo "usage:pltrace.sh xyzdir [dep] output."
	exit 0
elif [ $# -eq 2 ]
then
	xyzdir=$1
	output=$2
	option=''
elif [ $# -eq 3 ]
then
	xyzdir=$1
	dep=$2
	output=$3
else
	echo "parameters error."
fi

current=`pwd`

if [ -f ".gmtdefaults4" ]
then
	mv .gmtdefaults4 $xyzdir
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

if [ $# -eq 3 ]
then
	option="-h $dep"
fi

t2=`taup_time -mod prem -deg $dmax -ph PKiKP $option |awk 'NF>9 {if(NR==6)print $4}'`
t1=`taup_time -mod prem -deg $dmax -ph PcP $option |awk 'NF>9 {if(NR==6)print $4}'`

num=`awk 'FNR==1 {print $2}' *.xyz| awk 'END{print NR}'`
ytic=`echo "scale=3;(($dmax-$dmin))/4"|bc`
dmax=`echo "scale=3;($dmax + $ytic/1.5)"|bc`
dmin=`echo "scale=3;($dmin - $ytic/1.5)"|bc`


scale=1
i=0

echo "$dmin $dmax $ytic $xtic"
tmp=(`ls`)
tmp=${tmp[0]}

tmin=`awk 'NR==1 {printf("%d\n",$1)}' ${tmp}`
tmax=`awk 'END {printf("%d\n",$1)}' ${tmp}`
xtic=`echo "scale=3;($tmax - $tmin)/10"|bc`

xpos=`echo "scale=3;$tmax + ($tmax-$tmin)/25 "|bc`
FRAME="-JX9i/3.5i"
BOX="-R${tmin}/${tmax}/${dmin}/${dmax}"
echo $BOX

psbasemap $FRAME $BOX  -B${xtic}:Time/s:/${ytic}:Distance/deg:\SWen  -K  > $output

tpos=`echo "scale=3;$dmax + ${ytic}/5"|bc`
echo "$t2 $tpos 10 0 0 LM PKiKP"|pstext -R -J -B  -N -O -K>> $output
echo "$t1 $tpos 10 0 0 LM PcP"|pstext -R -J -B -N  -O -K >> $output

for file in *.xyz
do
	i=$((i+1))
	tmp=(`echo ${file//./ }`)
	sta=${tmp[0]}
	ypos=`awk 'NR==1 {print $2}' $file`

	echo "$xpos $ypos 8 0 0 LM $sta"|pstext -R -J -B -N -O -K >> $output
	if [ $i -eq $num ]
	then
		pswiggle $file -J -R  -Wthinnest,blue -Z${scale} -B  -O  >> $output
	else
		pswiggle $file -J -R  -Wthinnest,blue -Z${scale} -B  -K -O  >> $output
	fi
done

cd ${current}
