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
	char *path,*path1,*filename,*output,tmp[100];
	int j=0,i=0,k,npts,stkpts,*shift,maxsft=0,minsft=0,num;
	SACHEAD hd,chd;
	char *cstnm,*kstnm;
	float **data,*rsum,*isum,baz,delta,stla,stlo,ux,uy,dtime,*pti,*uh,*az,time=0,a,p,**image,amp,*sum;
	float cstla=62.6059,im,re,**real,*phase;
	float cstlo=-114.606;

	if(argc!=10)
	{
		printf("usage:pstack realdir imagedir -s p baz/-a baz p cstla cstlo cstnm outputs\n");
		exit(0);
	}
	else
	{
		output=argv[9];
		path=argv[1];
		path1=argv[2];
		cstla=atof(argv[6]);
		cstlo=atof(argv[7]);
		cstnm=argv[8];
	}
	
	DIR *d;
	char *name;
	FILE *fp;
	struct dirent *file;

	fp=fopen(output,"w");
	
	if((strcmp(argv[3],"-s")==0))   /*fix back azimuth,slowness varies.*/
	{
		p=atof(argv[4]);
		baz=atof(argv[5]);
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
					{
						data=(float **)malloc(1*sizeof(float *));
						image=(float **)malloc(1*sizeof(float *));
						real=(float **)malloc(1*sizeof(float *));
					}
					shift=(int *)malloc(1*sizeof(int));
				}
				else
				{	
					shift=(int *)realloc(shift,(i+1)*sizeof(int));
					if(k==0)
					{
						data=(float **)realloc(data,(i+1)*sizeof(float *));
						image=(float **)realloc(image,(i+1)*sizeof(float *));
						real=(float **)realloc(real,(i+1)*sizeof(float *));
					}
				}
			
				strcpy(tmp,path);
				strcat(tmp,filename);
				//printf("%s\n",tmp);
				data[i]=read_sac(tmp,&hd);
				kstnm=hd.kstnm;
				npts=hd.npts;
				delta=hd.delta;
				real[i]=read_sac(tmp,&hd);
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
				
				strcpy(tmp,path1);
				strcat(tmp,filename);
				//printf("%s\n",tmp);
				image[i]=read_sac(tmp,&hd);
				
				shift[i]=(int)(dtime/delta);
				if (shift[i] > maxsft)
					maxsft=shift[i];

				if (shift[i] < minsft)
					minsft=shift[i];

				i=i+1;
			}
			//continue;
			closedir(d);
			num=i;
			stkpts=npts-(maxsft-minsft);
			rsum=(float *)malloc(stkpts*sizeof(float));
			isum=(float *)malloc(stkpts*sizeof(float));
			sum=(float *)malloc(stkpts*sizeof(float));
			phase=(float *)malloc(stkpts*sizeof(float));
			memset(rsum,0,stkpts);
			memset(isum,0,stkpts);
			memset(sum,0,stkpts);
			memset(phase,0,stkpts);
			
			if(k==0)
			{
				for(i=0;i<num;i++)
					for(j=0;j<npts;j++)
					{
						amp=sqrtf(real[i][j]*real[i][j]+image[i][j]*image[i][j]);
						image[i][j]=image[i][j]/amp;
						real[i][j]=real[i][j]/amp;	
					}
			}
			
			for(i=0;i<num;i++)
				for(j=0;j<stkpts;j++)
				{
					sum[j]=sum[j]+data[i][(maxsft-shift[i]+j+1)];
					isum[j]=isum[j]+image[i][(maxsft-shift[i]+j+1)];
					rsum[j]=rsum[j]+real[i][(maxsft-shift[i]+j+1)];
				}
	
			for(i=0;i<stkpts;i++)
			{	
				phase[i]=sqrtf(rsum[i]*rsum[i]+isum[i]*isum[i]);
				phase[i]=phase[i]/num;
				sum[i]=sum[i]*phase[i]/num;
				//sum[i]=sum[i]/num;
				
				time=i*delta;
				fprintf(fp,"%f %f %f\n",time+chd.b+maxsft*delta,uh[k],sum[i]);
			}
			free(shift);
		
			//chd.npts=stkpts;
			//chd.b=chd.b+maxsft*delta;
			//chd.e=chd.e+minsft*delta;
			//write_sac(argv[2],chd,sum);
			free(sum);
			free(rsum);
			free(isum);
			free(phase);
			//printf("b:%.3f,e:%.3f,stkpts:%d,maxsft:%d,minsft:%d\n",chd.b,chd.e,stkpts,maxsft,minsft);
		}
		free(data);
		free(image);
		free(real);
		fclose(fp);
	}
	else if(strcmp(argv[3],"-a")==0) /*fix slowness,back azimuth varies.*/
	{
		a=atof(argv[4]);
		p=atof(argv[5]);
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
