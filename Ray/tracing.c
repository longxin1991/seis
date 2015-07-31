#include "mem.h"
#include "struct.h"
#include "numc.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

float *phase_parse(const char phase,Model *mod)
{
    float *V;
    switch(phase){
        case 'S':
            V = mod->Vs;
            break;
        case 'P':
            V = mod->Vp;
            break;
        default:
            printf("only parse P or S phase.\n");
            exit(0);
    }
    return V;
}

int point_layer(float *h,int ln,float z0)
{
    int i=0,dep=0;
    for(i=0;i<ln;i++)
    {
        dep += h[i];
        if (z0<=dep)
            break;
    }
    return i;
}

void ray_tracing_m(struct raycor R,Model *mod, RayData *Data){

    int k=0,i=0,turn=0,n,sn,cn=100,cnk,pass_slab=0,dip=0;
    float dt = 0.1,t,dep=0,slab_dep;
    float a,p,K1,K2,*x,*z,h_cmb=0,th=PI/6.0,ph,cdep,diff;
    float *Vp,*Vs,*h,*V,i0;

    x = (float *)malloc(MAXLEN*sizeof(float));
    z = (float *)malloc(MAXLEN*sizeof(float));

    /*preparation for the ray tracing*/
    Data->x = x;
    Data->z = z;
    h = mod->h;
    n = mod->n;
    //Vp = mod->Vp;
    //Vs = mod->Vs;
    V  = phase_parse(Data->phase,mod);
    //V = Vs;
    for(i=0;i<n;i++)
        h_cmb += h[i];
    x[0] = R.rx;
    z[0] = R.rz;
    i0 = R.ri;
    sn = point_layer(h,n,z[0]);/*get the layer number where the source is in*/
    for (i=1;i<=sn;i++)
    {
        dep += h[i-1];
    }
    p = sin(i0)/V[0];

    /*------------------------------*/
    /*tracing the downgoing part of the ray*/
    for (i=sn;i<n;i++){

        dep += h[i];

    while (k<MAXLEN)
    {
        if (k==0)
        {
            K1 = V[i]*p;
            K2 = V[i]*p;
            x[1] = x[0] + dt*K2;

            K2 = V[i]*sqrt(1-p*p*V[i]*V[i]);
            z[1] = z[0] + dt*K2;
            k = k+1;
            continue;
        }
        
            K1 = V[i]*p;
            K2 = V[i]*p;
            x[k] = x[k-1] + dt*K2;
            
            K2 = V[i]*sqrt(1-p*p*V[i]*V[i]);
            
            if (z[k-1]>=h_cmb && turn ==0)
            {
                z[k-1]=h_cmb;
                turn=1;
                k += 1;
                break;
            }
            /*
            if (turn==1)
                K2=-1*K2;
            */
            z[k] = z[k-1] + dt*K2;
            
           if (z[k]>=dep && K2>0 &&i<n){
               k += 1;
               break;
           }
           k = k + 1;
    }
    }
    /*---------------------------------------*/
    
    /*tracing the upgoing part of the ray*/
     for (i=n-1;i>=0;i--){

        dep -= h[i];
        a = asin(p*V[i]);
        //printf("%d %d\n",i,pass_slab);

    while (k<MAXLEN)
    {		
    		if(dip==1){
    		slab_dep = slab(th,x[k-1]);
    		diff = z[k-1] - slab_dep;
    		if(diff <=0 && fabs(diff) <5 && pass_slab==0)
    		{
    			pass_slab=1;
    			cdep=slab_dep;
    			cn = point_layer(h,n,cdep);
    			if (th>=a)
    				ph = th - a;
           		else
               		ph = a - th;
               	V = mod->Vp;
               	p = sin(ph)/V[i];
    			//printf("%d %.3f\n",i,z[0]);
    		}
    		}
    /*
    		if(i==0 && pass_slab==0)
    		{
    			slab_dep=slab(th,x[k-1]);
    			diff = slab_dep - z[k-1];
    			if ( diff >=0 )
    			{
    				cdep=slab_dep;
    				cn = point_layer(h,n,cdep);
    				pass_slab=1;
    				if (th>=a)
    					ph = th - a;
            		else
                		ph = a - th;
                	V[i] = mod->Vp[i];
                	p = sin(ph)/V[i];
    			}
    		}
    */
            K1 = V[i]*p;
            K2 = V[i]*p;
            x[k] = x[k-1] + dt*K2;
            
            K2 = -1*V[i]*sqrt(1-p*p*V[i]*V[i]);

            z[k] = z[k-1] + dt*K2;
            
           if (z[k]<=dep && K2<0){
               k += 1;
               break;
           }
           
           if ( k >100 && z[k]< 0)
           {
                   break;
           }
           k = k + 1;
    }
     }
        /*------------------------------------*/

     /*store the tracing result to RayData*/
    Data->dis = x[k-1];
    t = dt*k;
    Data->np = k;
    Data->t = t;
    //printf("%d %.3f\n",k,cdep);
    /*-------------------------------------*/
}

RayData *ray_shoot(struct raycor R1,struct raycor R2,Model *mod,float dis,char phase)
{
    float d1,d2,d0=0,i0=0,i1,i2,z0,x0;
    int i=0;
    RayData *Data;
    struct raycor R0;

    Data = (RayData *)malloc(1*sizeof(RayData));
    Data->phase = phase;
    /*bisection method*/
    ray_tracing_m(R1,mod,Data);
    free_path(Data); 
    d1 = Data->dis;
    i1 = R1.ri;
    ray_tracing_m(R2,mod,Data);
    free_path(Data); 
    d2 = Data->dis;
    i2 = R2.ri;
    z0 = R2.rz;
    x0 = R2.rx;
    R0.rx = x0;
    R0.rz = z0;
    while(fabs(d0 - dis)>=0.15)
    {
        i0 =(i1+i2)/2;
        R0.ri = i0;
        ray_tracing_m(R0,mod,Data);
        d0 = Data->dis;
        
        if (fabs(d0-dis)>=0.15)
            free_path(Data);
        if (d0>dis){
            i2 = i0;
        }
        else{
            i1 = i0;
        }
        i=i+1;
        if (i>1000)
        {
            printf("Shooting Fail! Target may not be reached.\n");
            exit(0);
        }
    }
    /* ---------------- */
    printf("------\n");
    printf("Result\n");
    printf("---------------------------------\n");
    printf("Steps |Date Points|Time |Distance\n");
    printf("---------------------------------\n");
    printf("%d\t%d\t%.2f\t%.3f\n",i,Data->np,Data->t,Data->dis);
    return Data;
}

void ray_tracing(float i0,float x0,float z0,float v, RayData *Data){

    int k=0,i=0,turn=0;
    float dt = 0.1,t;
    float p,K1,K2,*x,*z,h_cmb=100;

    x = (float *)malloc(MAXLEN*sizeof(float));
    z = (float *)malloc(MAXLEN*sizeof(float));

    Data->x = x;
    Data->z = z;

    p = sin(i0)/v;
    while (k<MAXLEN)
    {
        if (k==0)
        {
            K1 = v*p;
            K2 = v*p;
            x[1] = x0 + dt*K2;

            K2 = v*sqrt(1-p*p*v*v);
            z[1] = z0 + dt*K2;
            k = k+1;
            continue;
        }
        
        
        K1 = v*p;
        K2 = v*p;
        x[k] = x[k-1] + dt*K2;

        K2 = v*sqrt(1-p*p*v*v);
       
        if (z[k-1]>=h_cmb && turn ==0)
        {
            z[k-1]=h_cmb;
            turn=1;
        }

        if (turn==1)
            K2=-1*K2;
        z[k] = z[k-1] + dt*K2;

        if ( k >100 && z[k]< 0.01)
            break;
        
        Data->dis = x[k];
        k = k + 1;
        t = dt*k;
        Data->np = k;
        Data->t = t;
    }
}
