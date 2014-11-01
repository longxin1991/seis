#!/bin/bash

if [ $# -ne 2 ]
then
	echo "usage:norm.sh sacfiledir  outputdir"
	exit 0
elif [ ! -d $2 ]
then
	mkdir $2
fi

OUT=$2
sacdir=$1

if [ ! -d $OUT ]
then
	mkdir $OUT
fi

for file in ${sacdir}/*.SAC
do
file=`basename $file`
echo "normalize file $file"
sac << EOF
cut b 10 e -10
r ${sacdir}/${file}
setbb A &1,depmax
setbb B &1,depmin
setbb C (MAX (absolute %B) %A)
div %C
w ${OUT}/${file}
q
EOF

done
