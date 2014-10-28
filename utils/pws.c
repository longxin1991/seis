#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sacio.h>
#include <unistd.h>
#include <math.h>
#include "sac.h"
#include "dist.h"

float p=0.6;
/* calculate the time delay of each station relative to the 
stack center.
*/
float delay(float cstla,float cstlo,float stla,float stlo,float ux,float uy);

int main(int argc,char *argv[])
{
	char *path,*filename;
	int j=0,i=0,npts,stkpts,*shift,maxsft=0,minsft=0,num;
	SACHEAD hd,chd;
	char *cstnm,*kstnm;
	float **data,*sum,baz,delta,stla,stlo,ux,uy,dtime,*pt;
	float cstla=62.6059;
	float cstlo=-114.606;

	if(argc!=3)
	{
		printf("usage:pstack sacdir outputs\n");
		exit(0);
	}
	else
		path=argv[1];
	
	DIR *d;
	char *name;
	struct dirent *file;

	if(!(d = opendir(path)))
	{
		printf("error opendir %s!!!\n",path);
		exit(0);
	}
	else
		chdir(path);

	while((file=readdir(d)) != NULL)
	{
		if(strncmp(file->d_name, ".", 1) == 0)
			continue;
		filename=file->d_name;
		//allocate data space.
		if (i==0)
		{
			data=(float **)malloc(1*sizeof(float *));
			shift=(int *)malloc(1*sizeof(int));
		}
		else
		{
			data=(float **)realloc(data,(i+1)*sizeof(float *));
			shift=(int *)realloc(shift,(i+1)*sizeof(int));
		}
		data[i]=read_sac(filename,&hd);
		kstnm=hd.kstnm;
		npts=hd.npts;
		delta=hd.delta;
		
		if ((strcmp(kstnm,"YKAB0"))==0)
		{
			chd=hd;
			chd.b=hd.b;
			chd.e=hd.e;
		}

		baz=hd.baz;
		stla=hd.stla;
		stlo=hd.stlo;
		baz=baz*PI/180;
		ux=p*sin(baz);
		uy=p*cos(baz);

		dtime=delay(cstla,cstlo,stla,stlo,ux,uy);
		
		shift[i]=(int)(dtime/delta);
		if (shift[i] > maxsft)
			maxsft=shift[i];

		if (shift[i] < minsft)
			minsft=shift[i];

		i=i+1;
	}
	num=i;
	stkpts=npts-(maxsft-minsft);
	sum=(float *)malloc(stkpts*sizeof(float));
	memset(sum,0,stkpts);
	
	for(i=0;i<num;i++)
		for(j=0;j<stkpts;j++)
		{
			sum[j]=sum[j]+data[i][(maxsft-shift[i]+j+1)];
		}
	
	for(i=0;i<stkpts;i++)
	{		
		sum[i]=sum[i]/num;
	}
	free(shift);
	free(data);
	chd.npts=stkpts;
	chd.b=chd.b+maxsft*delta;
	chd.e=chd.e+minsft*delta;
	write_sac(argv[2],chd,sum);
	free(sum);
	closedir(d);

	printf("b:%.3f,e:%.3f,stkpts:%d,maxsft:%d,minsft:%d\n",chd.b,chd.e,stkpts,maxsft,minsft);
	return 0;

}

float delay(float cstla,float cstlo,float stla,float stlo,float ux,float uy)
{
	float res,rx,ry;
	cor pos;
	ux=ux/111.13;
	uy=uy/111.13;
	cal_rela_cor(cstla,cstlo,stla,stlo,&pos);
	rx=pos.x_pos;
	ry=pos.y_pos;

	res=rx*ux+ry*uy;
	return res;
}
