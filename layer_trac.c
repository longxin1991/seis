#include <stdio.h>
#include "layer.h"
#include "numc.h"

#define N 100
#define LN 3
//float h[4]={1.5,4.5,0.5,3.5};
//float vp[5]={4.5,6.8,7.0,8.0,8.1};
float vp[5]={4.5,7.5,7.0,12};
float h[4]={4.5,0,10};
float vs[5]={2.4,3.75,3.85,4.6,4.7};

int main(int argc,char *argv[])
{
	int irtr,i,j;
	float dx=0,dt=0,p[N],x[N]={0.0},t[N]={0.0},tau[N]={0.0};
	
	/*linspace(0.1236,0.2217,N,p);*/
	linspace(0.1,0.2217,N,p);

	for(i=0;i<N;i++)
	{
		for(j=0;j<LN;j++)
		{
			irtr = layerxt(p[i],h[j],1.0/vp[j],1.0/vp[j+1],&dx,&dt);
			x[i] += dx;
			t[i] += dt;
		}
		tau[i] = t[i]-p[i]*x[i];
	}
	for(i=0;i<N;i++)
		printf("%f	%f	%f	%f\n",p[i],x[i],t[i],tau[i]);
	return 0;
}
