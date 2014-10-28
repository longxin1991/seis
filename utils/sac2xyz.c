#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <sacio.h>
#include "sac.h"

int main(int argc,char *argv[])
{
	SACHEAD hd;
	float *data,delta,gcarc,b,time;
	int npts,i;
	FILE *fp;
	data=read_sac(argv[1],&hd);
	npts=hd.npts;
	gcarc=hd.gcarc;
	delta=hd.delta;
	b=hd.b;
	
	fp=fopen(argv[2],"w");
	
	for(i=0;i<npts;i++)
	{
		time=b+i*delta;
		fprintf(fp,"%.3f	%f	%.6e\n",time,gcarc,data[i]);
	}
	
	return 0;
}
