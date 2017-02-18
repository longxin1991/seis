#include "numc.h"

void linspace(float begin,float end,int n,float p[])
{
	int i;
	p[0]=begin;
	for(i=1;i<n-1;i++)
		p[i]=begin+(end-begin)/(n-1)*i;
	p[i]=end;
}

int hv(float x){

    if(x>=0)
        return 1;
    else
        return 0;
}

void der(float y[],int n,float dt)
{
    int i;
    for(i=0;i<n-1;i++)
        y[i]=(y[i+1]-y[i])/dt;

    y[n-1]=y[n-2];
}

void expspace(double max,double min,int n,double p[])
{
	int i;
	double q;

	for (i=0;i<n;i++)
	{
		q = pow(10,log10(max/min)/(n-1));
		p[i] = min*pow(q,i);
	}
}
