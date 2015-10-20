#!/bin/bash
#Extract SAC files from miniseed volume combined with metadata, and 
#Write Event and distance information to the extracted SAC files.
#BY longxin 
#2014.8.27

EVENTFILE=$1
DATADIR=`pwd`
metadir=${DATADIR}/meta

if [ $# -ne 1 ]
then
	echo "usage:extdt.sh eventfile."
	exit 0
fi

if [ -f $EVTNTFILE ]
then
	para=`cat $EVENTFILE |awk '{print $1}'`
else
	para=$EVENTFILE
fi


cd "$DATADIR"

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

SACDIR="${DATADIR}/$ID/sac"
EVENTDIR="${DATADIR}/$ID"
if [ ! -d "${SACDIR}" ]
then
	mkdir "${SACDIR}"
fi

cd "$EVENTDIR"
stalist=sta.txt

fn=`ls *.mseed|wc -l`
if [ $fn -eq 0 ]
then
	echo "No mseed file for event $EVENTDIR, skip"
	continue
fi

for rawfile in *
do
	if [[ $rawfile =~ .*mseed ]]
	then
		filepre=${rawfile%.mseed}
		staname=${rawfile%_*}
		dis=${filepre#*_}
		mseed2sac $rawfile  -m "${metadir}/${staname}.meta"
		#mseed2sac $rawfile  -m "${staname}_${dis}.metadata"
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
#temp=(`taup_time -mod prem -deg $dis -ph PKiKP,PKIIKP -h $edp |awk 'NF>9{if(NR>=6)print $4}'`)
#t2=${temp[0]}
#t5=${temp[1]}
t1=`taup_time -mod prem -deg $dis -ph PcP -h $edp |awk 'NF>9 {if(NR==6)print $4}'`
t2=`taup_time -mod prem -deg $dis -ph PKiKP -h $edp |awk 'NF>9 {if(NR==6)print $4}'`
#d=`echo "scale=3;180-$dis/2"|bc`
#t4=`taup_time -mod prem -deg $d -ph PKIKP -h $edp |awk 'NF>9 {if(NR==6)print $4}'`
#t4=`echo "$t4 * 2"|bc`
sac <<EOF
echo on
r $rsacfile
chnhdr GCARC $dis STLA $sla STLO $slo BAZ $baz AZ $az T1 $t1 T2 $t2
w $rsacfile
quit
EOF
fi
done
fi
done

mv *.SAC "${SACDIR}"/
echo "Moving extracted file to SACDIR"

cd "${SACDIR}"
#Write Event info to SAC files.
for sacfile in ${SACDIR}/*
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

cd "$DATADIR"
fi

done
