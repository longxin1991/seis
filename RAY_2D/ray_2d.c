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
    int i,j,nth,nr,npts;
    double *sp,delta0=4.70,ttime,deg;
    double *r,*theta,*dvp,*delta,ndelta=300;
    Ind id[2];
    char *het_file = "hetero_disc.txt";
    
    hetero = 0;
    sampray = 0;
    
   	//mod.Vp = (float *)malloc(MAXLAYER*sizeof(float));
    //mod.Vs = (float *)malloc(MAXLAYER*sizeof(float));
	//mod.h = (float *)malloc(MAXLAYER*sizeof(float));
	//mod.rho = (float *)malloc(MAXLAYER*sizeof(float));
	//mod.Qp = (float *)malloc(MAXLAYER*sizeof(float));
	//mod.Qs = (float *)malloc(MAXLAYER*sizeof(float));
	
	//read_mod("prem.nd",&mod);
	mk_1d_prem(&mod);
	
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
    {
        load_hetero(r,theta,dvp,npts,sp,nr,nth,mod.h);
        printf("Heterogeneity loaded.\n");
    }

	delta = (double *)malloc(ndelta*sizeof(double));
	linspace(4,5.4,ndelta,delta);
	//linspace(6.3,10.05,ndelta,delta);

	//for (i=243;i<244;i++)
	for (i=0;i<ndelta;i++)
	{
		delta0 = PI - delta[i]/DEG2RAD;
		rvar0.r = 6371.0;
		rvar0.theta = 0.0;
		rvar0.tr  = cosf(delta0)/5.8;
		rvar0.ttheta = rvar0.r*sinf(delta0)/5.8;
    
		ray_tracer2d(rvar0,nth,nr,mod.h,sp,&ttime,&deg,&rsamp);
    
		//printf("deg->%.3f, ttime->%.3f, i->%d, delta->%.3f\n",deg,ttime,i,delta[i]);
		printf("%f %f %d %.3f\n",deg,ttime,i,delta[i]);
		if (sampray == 1)
		{   
			printf("%d\n",rsamp.n);
			
			for (j=0;j<=rsamp.n;j++)
			{
				printf("%f %f\n",rsamp.theta[j],rsamp.r[j]);
			}
		}
	}
	free(mod.Vp); 
	free(mod.h); 
	free(sp);
	free(delta);
	free(rsamp.r);
	free(rsamp.theta);
    return 0;
}
