#!/bin/bash

INFILE=$1
POINTS=21
ODIR=/home/longxin
n=`seq $POINTS`
SCALE=2e19
awk -v var=$SCALE 'NR>1 {for(i=2;i<=NF;i++) print i-1,$1,$i*var}' $INFILE >$ODIR/test.txt

rm $ODIR/t2.txt
for i in $n
do
	awk -v var="$i" '$1==var {print ($1-1)*10+30,$2,$3}' $ODIR/test.txt>>$ODIR/t2.txt
	echo ">" >> $ODIR/t2.txt
done

FRAME="-JX5i/5i"
BOX="-R30/230/-10/60"
SCALE=1
FR=$ODIR/t2.txt
YLABEL="Times,s"
XLABEL="Epicentral distance,deg"
pswiggle $FR $FRAME $BOX -Z$SCALE -M -G0/0/255 -W1/0 -B20/a5::\SWen -P  > $INFILE".ps"
#pswiggle $FR $FRAME $BOX -Z$SCALE -W1/0  -M -P -O -K>>$INFILE".ps"

echo 0 -2.4 15 0 0 6 $XLABEL | pstext $FRAME -R-1/1/-2/2 -N -K -O >> $INFILE".ps"
echo -1.25 0 15 90 0 6 $YLABEL | pstext $FRAME -R-1/1/-2/2 -N  -O >>$INFILE".ps"
