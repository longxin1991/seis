#!/bin/bash
#Extract SAC files from miniseed volume combined with metadata, and 
#Write Event and distance information to the extracted SAC files.
#BY longxin 
#2014.8.27

EVENTFILE=$1
DATADIR=~/data

if [ $# -ne 1 ]
then
	echo "usage:extdt.sh eventfile."
	exit 0
fi
para=`cat $EVENTFILE |awk '{print $1}'`

cd $DATADIR

#Select event directory.
# if [  -n "$1" ]
# then
# 	para=$1
# fi

#Processing for each event.
for item in $para
do
if [ -d $item ]
then
ID=$item
event=`awk -v var=${ID} '$1 ~ var' ${EVENTFILE}`

#processing date
einfo=(${event//|/ })
edp=${einfo[5]}
ela=${einfo[3]}
elo=${einfo[4]}
edate=(${einfo[1]//// })
ey=${edate[0]}
emo=${edate[1]}
ed=${edate[2]}
etime=(${einfo[2]//:/ })
eh=${etime[0]}
emi=${etime[1]}
es=${etime[2]}
etf="${ey}-${emo}-${ed} ${eh}:${emi}:${es}"

SACDIR=~/data/$ID/sac
EVENTDIR=~/data/$ID
if [ ! -d ${SACDIR} ]
then
	mkdir ${SACDIR}
fi

cd $EVENTDIR
stalist=sta.txt

for rawfile in *
do
	if [[ $rawfile =~ .*mseed ]]
	then
		filepre=${rawfile%.mseed}
		staname=${rawfile%_*}
		dis=${filepre#*_}
		mseed2sac $rawfile  -m "${filepre}.metadata"
		echo "Extracting file $rawfile."

#write GCARC and station info to SAC files.
for rsacfile in *.SAC
do
if [[ $rsacfile =~ $staname ]]
then
staloc=(`awk -v sta=$staname '{if ($2==sta) print $3,$4}' $stalist`)
sla=${staloc[0]}
slo=${staloc[1]}

da=(`distaz $sla $slo $ela $elo`)
baz=${da[1]}
az=${da[2]}
#计算理论到时
theory=`taup_time -mod prem -deg $dis -ph PKiKP -h $edp |awk 'NF>9 {if(NR==6)print $4}'`
sac <<EOF
echo on
r $rsacfile
chnhdr GCARC $dis STLA $sla STLO $slo BAZ $baz AZ $az T1 $theory
w $rsacfile
quit
EOF
fi
done
fi
done

mv *.SAC ${SACDIR}/
echo "Moving extracted file to SACDIR"

cd ${SACDIR}
#Write Event info to SAC files.
for sacfile in $SACDIR/*
do
echo $sacfile
fsplit=(${sacfile//./ })
bt=${fsplit[-2]}
btsplit=(${bt//,/ })
btime=${btsplit[2]}
ets=`date +%s -d "$etf"`
bts=`date +%s -d "${ey}-${emo}-${ed} $btime"`
tdiff=$((ets - bts))
echo "eft: $etf, ${ey}-${emo}-${ed} $btime"
sac <<EOF
echo on
r ${sacfile}
chnhdr O $tdiff EVLA $ela EVLO $elo EVDP $edp
w ${sacfile}
quit
EOF
done

cd $DATADIR
fi

done