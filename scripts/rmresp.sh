#!/bin/bash 

if [ $# -ne 1 ]
then
	echo "usage:rmresp.sh eventfile."
	exit 0
fi

EVENTFILE=$1
FILTRE=(1.0 2.0)
FREQ="0.8 1 2 2.2"
#FREQ="1.5 1.9 2.1 2.5"
RTDIR=~/data
EVENTS=(`cat $EVENTFILE|awk '{print $1}'`)

N=`seq ${#EVENTS[@]}`

for i in $N;do
i=$(($i-1))
echo "Processing Event ${EVENTS[$i]} \n"
SACDIR=${RTDIR}/${EVENTS[$i]}/sac

OUTDIR=${RTDIR}/${EVENTS[$i]}/out


if [ ! -d $OUTDIR ]
then
	mkdir $OUTDIR
fi

cd ${RTDIR}/${EVENTS[$i]}
SACFILE=`ls sac/`
for file in $SACFILE ;do
sac << EOF
echo on
r ./sac/${file}
taper
rtr
rmean
trans from evalresp to none freq $FREQ
trans to WWSP
w ${OUTDIR}/${file}
q
EOF
done
#cp ${OUTDIR}/${file} $RTDIR
cd $RTDIR
done
