#include <stdio.h>
#include <stdlib.h>
#include "mod.h"
#include "ray.h"
#include "numc.h"
#include "grid.h"
#include "hetero.h"

//int turn=0,disc=0,cstep=0;
//float sigma,rdis,s1,s2;
int hetero,sampray;

int main(int argc,char *argv[])
{
    Model mod;
    Ray2d rvar,rvar0;
    Raysamp rsamp;
    int i,j,nth,nr,npts,stat;
    double *sp,delta,ttime,deg,delta1,delta2;
    double *r,*theta,*dvp,ndelta=300;
    Ind id[2];
    char *het_file = "hetero_disc.txt";
   
	if (argc !=2 )
	{
		printf("usage:%s deg\n",argv[0]);
		exit(0);
	}
	else
		deg = atof(argv[1]);
	
    hetero = 1;
    sampray = 0;
    
   	mod.Vp = (float *)malloc(MAXLAYER*sizeof(float));
    mod.Vs = (float *)malloc(MAXLAYER*sizeof(float));
	mod.h = (float *)malloc(MAXLAYER*sizeof(float));
	mod.rho = (float *)malloc(MAXLAYER*sizeof(float));
	mod.Qp = (float *)malloc(MAXLAYER*sizeof(float));
	mod.Qs = (float *)malloc(MAXLAYER*sizeof(float));
	
	read_mod("prem.nd",&mod);
	
	/*read discret point that describ lateral heterogeneity*/
	read_hetero(het_file,&r,&theta,&dvp,&npts);
    
    
    nth = 360;
	
	rsamp.r = (double*)malloc(nth*sizeof(double));
	rsamp.theta = (double*)malloc(nth*sizeof(double));

    nr = mod.n;
    sp = (double *)malloc(nr*nth*sizeof(double));
    
       
    /*Assign velocity value of backgound model to grid points */
    for (i=0;i<nr;i++)
        for (j=0;j<nth;j++)
        {
            sp[i*nth+j] = powf((1/mod.Vp[i]),2);
        }
    
    if (hetero==1)
        load_hetero(r,theta,dvp,npts,sp,nr,nth,mod.h);

	//linspace(4,5.4,ndelta,delta);
	//linspace(6.3,10.05,ndelta,delta);
	delta1 = 4.0;
	delta2 = 5.4;
	stat = ray_shoot(delta1,delta2,nth,nr,mod.h,sp,deg,&ttime,&delta,&rsamp);

	printf("%.3f %f %f\n",deg,ttime,delta);
	if (sampray == 1)
	{   
		printf("%d\n",rsamp.n);
		
		for (j=0;j<=rsamp.n;j++)
		{
			printf("%f %f\n",rsamp.theta[j],rsamp.r[j]);
		}
	}
	
    return 0;
}
