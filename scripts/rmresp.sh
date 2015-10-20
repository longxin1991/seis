#!/bin/bash 

if [ $# -ne 1 ]
then
	echo "usage:rmresp.sh eventfile."
	exit 0
fi

EVENTFILE=$1

if [ -f $EVENTFILE ]
then
	TYPE=FILE
	EVENTS=(`cat $EVENTFILE|awk '{print $1}'`)
else
	TYPE=DIR
	EVENTS=($EVENTFILE)
fi

FILTRE=(1.0 2.0)
FREQ="0.8 1.0 10 12"
#FREQ="0.8 1 2 2.2"
#FREQ="0.8 1.0 3.0 3.2"
#FREQ="0.1 0.2 0.4 0.6"
#FREQ="0.6 0.8 1.8 2.0"
#FREQ="1.8 2.0 3.0 3.2"
#FREQ="1.2 2.0 3.0 3.8"
RTDIR=`pwd`
respdir="${RTDIR}"/resp

N=`seq ${#EVENTS[@]}`

for i in $N;do
i=$(($i-1))

if [ -d ${EVENTS[$i]} ]
then

echo "Processing Event ${EVENTS[$i]}"
SACDIR="${RTDIR}/${EVENTS[$i]}/sac"

OUTDIR="${RTDIR}/${EVENTS[$i]}/out"


if [ ! -d $OUTDIR ]
then
	mkdir $OUTDIR
fi

cd "${RTDIR}"/${EVENTS[$i]}
SACFILE=`ls sac/`
for file in $SACFILE
do

a=(`echo ${file//./ }`)
size=`echo ${#a[@]}`

#deal with the situation of file without loc number.
if [ $size = 7 ]
then
	sta=${a[1]}
	loc=${a[2]}
	ch=${a[3]}
else
	sta=${a[1]}
	loc="--"
	ch=${a[2]}
fi

if [ $size = 12 ]
then
	net=${a[6]}
	sta=${a[7]}
fi


#pzfile=`ls SACPZ*|grep "\.${sta}\."`
pzfile=`ls ${respdir}/SACPZ*|grep ".*${sta}\.${loc}\.${ch}.*"`
#pzfile=`ls ${respdir}/SAC_PZ*|grep ".*${net}_${sta}.*"`
#pzfile=`ls resp/SAC_PZ*|grep ".*${net}_${sta}.*"`
#pzfile=`ls SACPZ*|grep ".*${sta}\.${loc}\.${ch}.*"`
#pzfile=$RTDIR/SACPZ.IM.TXAR.--.SHZ

sac << EOF
setbb pzfile $pzfile
echo on
cut 0 e
r ./sac/${file}
rtr
rmean
trans from polezero s $pzfile to none freq $FREQ
mul 1.0e9
taper
w ${OUTDIR}/${file}
q
EOF
done
#cp ${OUTDIR}/${file} $RTDIR
cd "$RTDIR"
fi
done
