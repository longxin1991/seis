#/bin/bash

TIME1=-10
TIME2=30
FILE=wf.ps
FRAME="-JX6i/2i"
BOX="-R${TIME1}/${TIME2}/140/150"
pswiggle 1.xyz $FRAME $BOX -Wthinnest,blue -Z1 -B10/a10::\SWen  -V -K > $FILE
pswiggle 2.xyz  -R -J -Wthinnest,red -Z1  -V -O >> $FILE
