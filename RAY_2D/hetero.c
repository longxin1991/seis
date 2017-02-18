#include <stdio.h>
#include <stdlib.h>
#include "numc.h"

void read_hetero(char *het_file,double **r,double **theta,double **dvp,int *npts)
{
	int i,n;
	float a,b,c;
	FILE *fp;
	if((fp=fopen(het_file,"r"))==NULL)
	{
		printf("fail to read %s\n",het_file);
		exit(0);
	}
	else
	{
		fscanf(fp,"%d",npts);
		n = *npts;
	}

	*r = (double *)malloc(n*sizeof(double));
	*theta = (double *)malloc(n*sizeof(double));
	*dvp = (double *)malloc(n*sizeof(double));

	for (i=0;i<n;i++)
	{
		fscanf(fp,"%f %f %f",&a,&b,&c);
		(*r)[i]=a;
		(*theta)[i]=b;
		(*dvp)[i]=c;
	}
	fclose(fp);
}

void load_hetero(double *r,double *theta,double *dvp,int npts,double *sp,int nr,int nth,float *h)
{
	int nhet,ulx,uly,urx,ury,llx,lly,lrx,lry;
	double ur[2],lr[2],lt[2],rt[2],dth,v;
	int i,j,k;

	nhet = npts/2;
	dth = 180.0/nth;
	for (i=0;i<nhet;i++)
	{
		lr[i] = r[i*2];
		ur[i] = r[i*2+1];
		lt[i] = theta[i*2];
		rt[i] = theta[i*2+1];
		
		uly = r_index(ur[i],nr,h);
		lry = r_index(lr[i],nr,h);
		
		lrx = (int)(rt[i]/dth);
		if (i==0)
			ulx = (int)(lt[i]/dth);
		else
		{
			if (lt[i]-rt[i-1] < dth && lt[i] - rt[i-1]>-0.000001)
				ulx = (int)(lt[i]/dth)+1;
			else if(lt[i] -rt[i-1] >= dth)
				ulx = (int)(lt[i]/dth);
			else
			{
				printf("model error, check the model file.%f\n",lt[i]-rt[i-1]);
				exit(0);
			}
		}
		
		for (j=ulx;j<=lrx;j++)
		{
			for (k=uly;k<=lry+1;k++)
			{
				v = 1.0/sqrt(sp[k*nth+j]);
				v *= (1.0 + dvp[i*2]/100.0);
				//printf("i->%d,j->%d,k->%d,v->%f\n",i,j,k,v);
				sp[k*nth+j] = pow(1/v,2);
			}
		}
		//printf("i->%d,ulx->%d,lrx->%d\n",i,ulx,lrx);
	}
}

int r_index(double r,int nr,float *h)
{
	double z;
	int i;
	z = 6371.0 - r - 0.1;
	for (i=1;i<nr;i++)
	{
		if (z>= h[i-1] && z < h[i])
			break;
	}
	return i-1;
}
