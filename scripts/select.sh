#!/bin/bash
#按事件挑选记录的脚本
#last modified:2014/11/04
#	增加每页显示道的个数的选项
#2014/10/25 initial coding
#2014/12/24 fix bug of multitrace display 
#2015/05/13 增加判断要显示的是单个事件还是要显示事件列表文件中的所有事件的功能
#by longxin,	igg,cas

RTDIR=`pwd`
evtlst=$1
trash=~/trash
savedir=best
#limit="940 1060"
#limit="1300 1400"
tmpfile=/tmp/filelst.txt
#限制每页显示的道的个数　
tn=6

if [ $# -lt 1 ]
then
	echo "usage:select.sh evtlst [sacdir]"
	exit 0
else
	sacdir=$2
fi

if [ -z $sacdir ]
then
	sacdir="out"
fi

if [ -d $RTDIR ]
then
	cd "$RTDIR"
else
	echo "cd to data dir fail."
	exit 0
fi

#判断要显示的是单个事件还是要显示事件列表文件中的所有事件
if [ -f $evtlst ]
then
	evtn=`awk 'END{print NR}' $evtlst`
	TYPE="FILE"
elif [ -d $evtlst ]
then
	evtn=1
	TYPE="DIR"
else 
	echo "parameter error!"
	exit 0
fi

i=0

while [ $i -lt $evtn ] 
do
	i=$((i+1))
	j=0
	if [ $TYPE = "FILE" ]
	then
		evtid=`awk -v v1=$i 'NR==v1 {print $1}' $evtlst`
	else
		evtid=$evtlst
	fi

	if [ -d $evtid ]
	then
		if [ ! -d ${evtid}/${savedir} ]
		then
			mkdir ${evtid}/${savedir}
		fi
		
		cd ${evtid}/${sacdir}
		echo "cd event data dir ${evtid}/${sacdir}."
		filenum=`ls -l | grep "^-" | wc -l`
		
		if [ $filenum = "0" ]
		then
			echo "no file,skip event $evtid."
			cd ${RTDIR}
			continue
		fi
		if [ -f $tmpfile ]
		then
			rm $tmpfile
		fi
		
		resnum=`echo "${filenum}/${tn}"`

		for sacfile in *.SAC
		do
			j=$((j+1))
			rem=`echo "${j}%(${tn})"|bc`
			if [ $rem -ne "0" ] && [ $j -lt $filenum ]
			then
				echo $sacfile >>$tmpfile
				continue
			else if [ "$rem" -eq 0 ] || [ "$j" -eq $filenum ]
			then
				echo $sacfile >>$tmpfile
				flst=`cat $tmpfile`
				flst=`echo $flst`
sac <<EOF
echo on
cut $limit
r $flst
echo off
fileid l ul
fileid t l gcarc baz
qdp off
window 1 x 0.2 0.7 y 0.1 1 ASPECT off
ppk p ${tn}
q
EOF
				echo "which file to save?/[0-5]/"
				echo "skip event:/s/skip page:/enter or n/delete:/d/"
				read choice
				if [ "$choice" = "n" ]
				then
					echo "next"
					rm $tmpfile
					continue
				elif [ "$choice" = "m" ]
				then
					echo "Mark event $evtid"
					touch $RTDIR/$evtid/marker
					rm $tmpfile
					continue
				elif [ "$choice" = "s" ]
				then
					echo "skip event $evtid"
					rm $tmpfile
					break
				elif [ "$choice" = "dd" ]
				then
					echo "delete event $evtid"
					echo "rm -r $RTDIR/$evtid"
					rm -r $RTDIR/$evtid
					rm $tmpfile
					break
				elif [[ "$choice" = "" ]]
				then
					echo "next" 
					rm $tmpfile
					continue
				elif [ "$choice" = "d" ]
				then
					echo "delect which file?[0-5]"
					read c
					c=($c)
					k=0
					n=`echo ${#c[@]}`
					f=($flst)
					while [ $k -lt $n ]
					do
						fn=${c[$k]}
						echo "delete file ${f[$fn]}"
						mv ${f[$fn]} $trash
						k=$((k+1))
					done
				else
					choice=($choice)
					k=0
					n=`echo ${#choice[@]}`
					f=($flst)
					while [ $k -lt $n ]
					do
						fn=${choice[$k]}
						echo "save file ${f[${fn}]}"
						cp  ${f[${fn}]} ../${savedir}
						k=$((k+1))
					done
				fi
				
				rm $tmpfile
			fi
		fi
		done
	fi

	cd "$RTDIR"
done
