#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sacio.h>
#include <unistd.h>
#include <math.h>
#include "sac.h"
#include "dist.h"
#include "numc.h"

#define N 50 /*slowness sampling number*/

/* calculate the time delay of each station relative to the 
stack center.
*/
float delay(float cstla,float cstlo,float stla,float stlo,float ux,float uy);

int main(int argc,char *argv[])
{
	char *path,*filename,*output,tmp[100];
	int j=0,i=0,k,npts,stkpts,*shift,maxsft=0,minsft=0,num;
	SACHEAD hd,chd;
	char *cstnm,*kstnm;
	float **data,*sum,baz,delta,stla,stlo,ux,uy,dtime,*pti,*uh,*az,time=0,a,p;
	float cstla=62.6059;
	float cstlo=-114.606;

	if(argc!=9)
	{
		printf("usage:pstack sacdir -s p baz/-a baz p cstla cstlo cstnm outputs\n");
		exit(0);
	}
	else
	{
		output=argv[8];
		path=argv[1];
		cstla=atof(argv[5]);
		cstlo=atof(argv[6]);
		cstnm=argv[7];
	}
	
	DIR *d;
	char *name;
	FILE *fp;
	struct dirent *file;

	fp=fopen(output,"w");
	
	if((strcmp(argv[2],"-s")==0))   /*fix back azimuth,slowness varies.*/
	{
		p=atof(argv[3]);
		baz=atof(argv[4]);
		uh=(float *)malloc(N*sizeof(float)); /*slowness array*/
		linspace(p-2.0,p+1.0,N,uh);
		for (k=0;k<N;k++)
		{
			if(!(d = opendir(path)))
			{
				printf("error opendir %s!!!\n",path);
				exit(0);
			}
		//printf("%d,%f\n",k,uh[k]);
		//continue;
			i=0;
			while((file=readdir(d)) != NULL)
			{
				if(strncmp(file->d_name, ".", 1) == 0)
					continue;
				filename=file->d_name;
				//allocate data space.
				if (i==0)
				{
					if(k==0)
						data=(float **)malloc(1*sizeof(float *));
					shift=(int *)malloc(1*sizeof(int));
				}
				else
				{	
					shift=(int *)realloc(shift,(i+1)*sizeof(int));
					if(k==0)
						data=(float **)realloc(data,(i+1)*sizeof(float *));
				}
			
				strcpy(tmp,path);
				strcat(tmp,filename);
				//printf("%s\n",tmp);
				data[i]=read_sac(tmp,&hd);
				kstnm=hd.kstnm;
				npts=hd.npts;
				delta=hd.delta;
		
				if ((strcmp(kstnm,cstnm))==0)
				{	
					chd=hd;
					chd.b=hd.b;
					chd.e=hd.e;
				}
				stla=hd.stla;
				stlo=hd.stlo;
				baz=baz*PI/180;
				ux=uh[k]*(sin(baz));
				uy=uh[k]*(cos(baz));

				dtime=delay(cstla,cstlo,stla,stlo,ux,uy);
		
				shift[i]=(int)(dtime/delta);
				if (shift[i] > maxsft)
					maxsft=shift[i];

				if (shift[i] < minsft)
					minsft=shift[i];

				i=i+1;
			}
		
			closedir(d);
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
				time=i*delta;
				fprintf(fp,"%f %f %f\n",time+chd.b+maxsft*delta,uh[k],sum[i]);
			}
			free(shift);
		
			//chd.npts=stkpts;
			//chd.b=chd.b+maxsft*delta;
			//chd.e=chd.e+minsft*delta;
			//write_sac(argv[2],chd,sum);
			free(sum);
			//printf("b:%.3f,e:%.3f,stkpts:%d,maxsft:%d,minsft:%d\n",chd.b,chd.e,stkpts,maxsft,minsft);
		}
		free(data);
		fclose(fp);
	}
	else if(strcmp(argv[2],"-a")==0) /*fix slowness,back azimuth varies.*/
	{
		a=atof(argv[3]);
		p=atof(argv[4]);
		az=(float *)malloc(N*sizeof(float));
		linspace(a-10,a+10,N,az);
		
		for (k=0;k<N;k++)
		{
			if(!(d = opendir(path)))
			{
				printf("error opendir %s!!!\n",path);
				exit(0);
			}
			i=0;
			while((file=readdir(d)) != NULL)
			{
				if(strncmp(file->d_name, ".", 1) == 0)
					continue;
				filename=file->d_name;
				//allocate data space.
				if (i==0)
				{
					if(k==0)
						data=(float **)malloc(1*sizeof(float *));
					shift=(int *)malloc(1*sizeof(int));
				}
				else
				{	
					shift=(int *)realloc(shift,(i+1)*sizeof(int));
					if(k==0)
						data=(float **)realloc(data,(i+1)*sizeof(float *));
				}
				
				strcpy(tmp,path);
				strcat(tmp,filename);
				data[i]=read_sac(tmp,&hd);
				kstnm=hd.kstnm;
				npts=hd.npts;
				delta=hd.delta;
				
				if ((strcmp(kstnm,cstnm))==0)
				{	
					chd=hd;
					chd.b=hd.b;
					chd.e=hd.e;
				}
				
				baz=az[k];
				stla=hd.stla;
				stlo=hd.stlo;
				baz=baz*PI/180;
				ux=p*(sin(baz));
				uy=p*(cos(baz));
				
				dtime=delay(cstla,cstlo,stla,stlo,ux,uy);
				
				shift[i]=(int)(dtime/delta);
				
				if (shift[i] > maxsft)
					maxsft=shift[i];
				
				if (shift[i] < minsft)
					minsft=shift[i];
				i=i+1;
				
			}
			
			closedir(d);
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
				time=i*delta;
				fprintf(fp,"%f %f %f\n",time+chd.b+maxsft*delta,az[k],sum[i]);
			}
			free(shift);
			free(sum);
		}
		free(data);
		fclose(fp);
	}
	else  /*parameter switch error.*/
	{	
		printf("parameter wrong\n");
		exit(0);
	}
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
