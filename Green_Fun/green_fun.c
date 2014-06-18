#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <stdlib.h>
#include "numc.h"
#include "io.h"


int main(int argc,char *argv[])
{
    int n1;
    int n2;
    double vp;
    double vs;
    double rho;
    double mu;
    double x1,x2,x3;

    double complex M[3][3]={0},N[3][3]={0};
    double gp[3][3]={0},gs[3][3]={0};
    double R,r,sta,cta,sp,cp,C,*t,dt,p,dp,t2;
    double *GS[9],*GP[9];
    float para[5];

    FILE *fd;
    int i,j,k,l;

    double complex q,eta,etb,gm,sgm,xi;

    getdata("input_data",para,5);
    n1=(int)para[0];
    n2=(int)para[1];
    vp=para[2];
    vs=para[3];
    rho=para[4];
    t=(double *)malloc(n1*sizeof(double));
    

    for(i=0;i<9;i++)
    {
        GS[i]=(double *)malloc(n1*sizeof(double));
        GP[i]=(double *)malloc(n1*sizeof(double));
    }
    linspace(0,4,n1,t);
    dt=t[1]-t[0];
    x1=atof(argv[1]);
    x2=atof(argv[2]);
    x3=atof(argv[3]);
    
    mu=vs*vs*rho;
    R=sqrt(x1*x1+x2*x2);
    r=sqrt(R*R+x3*x3);

    sta=R/r;
    cta=x3/r;
    sp=x2/R;
    cp=x1/R;
    C=1/(PI*PI*mu*r);
    

    for(i=0;i<n1;i++)
    {   
        if((t[i]/r)<=1/vp)
            dp=0;
        else
            dp=sqrt(t[i]*t[i]/(r*r)-1/(vp*vp))/n2;
        //printf("%f\n",dp);
        for(j=0;j<n2;j++)
        {
            p=dp*j;
            q=-sta*t[i]/r+I*cta*csqrt(t[i]*t[i]/(r*r)-1/(vp*vp)-p*p);
            eta=csqrt(1/(vp*vp)+p*p-q*q);
            etb=csqrt(1/(vs*vs)+p*p-q*q);
            gm=etb*etb+p*p-q*q;
            sgm=gm*gm+4*eta*etb*(q*q-p*p);
            xi=csqrt(t[i]*t[i]/(r*r)-1/(vp*vp)-p*p);
            
            M[0][0]=2*etb*((q*q+p*p)*cp*cp-p*p);
            M[0][1]=2*etb*(q*q+p*p)*sp*cp;
            M[0][2]=2*q*eta*etb*cp;
            M[1][0]=M[0][1];
            M[1][1]=2*etb*((q*q+p*p)*sp*sp-p*p);
            M[1][2]=2*1*eta*etb*sp;
            M[2][0]=q*gm*cp;
            M[2][1]=q*gm*sp;
            M[2][2]=eta*gm;
            
            for(k=0;k<3;k++){
                for(l=0;l<3;l++)
                {   
                    //printf("%f %f ",creal(M[k][l]),cimag(M[k][l]));
                    gp[k][l]=gp[k][l] + C*hv(t[i]-r/vp)*creal(eta/sgm*M[k][l]/xi)*dp;
                }
                //printf("\n");
            }
            
        }

        for(k=0;k<3;k++){
            for(l=0;l<3;l++)
            {
                GP[3*k+l][i]=gp[k][l];
            }
        }
    }

    for(i=0;i<9;i++)
        der(GP[i],n1,dt);

    for(i=0;i<n1;i++)
    {
        if((vs/vp)>=sta)
        {   
            if((t[i]/r)<=1/vs)
                dp=0;
            else
                dp=sqrt(t[i]*t[i]/(r*r)-1/(vs*vs))/n2;
            t2=r/vs;
        }
        else
        {
            double tmp=(t[i]/r-sqrt(1/(vs*vs)-1/(vp*vp))*cta)/sta;
            if(tmp<=1/vp)
                dp=0;
            else
                dp=sqrt(tmp*tmp-1/(vp*vp))/n2;
            t2=r/vp*sta+r*sqrt(1/(vs*vs)-1/(vp*vp))*cta;
        }

        //printf("%f\n",dp);
        for(j=0;j<n2;j++)
        {
            p=dp*j;
            q=-sta*t[i]/r+I*cta*csqrt(t[i]*t[i]/(r*r)-p*p-1/(vs*vs));
            //printf("%f  %f\n",creal(q),cimag(q));
            eta=csqrt(1/(vp*vp)+p*p-q*q);
            etb=csqrt(1/(vs*vs)+p*p-q*q);
            gm=etb*etb+p*p-q*q;
            sgm=gm*gm+4*eta*etb*(q*q-p*p);
            xi=csqrt(t[i]*t[i]/(r*r)-1/(vs*vs)-p*p);

            N[0][0]=1/etb*(etb*etb*gm-(gm-4*eta*etb)*((q*q+p*p)*sp*sp-p*p));
            N[0][1]=1/etb*(q*q+p*p)*(gm-4*eta*etb)*sp*cp;
            N[0][2]=-q*gm*cp;
            N[1][0]=N[0][1];
            N[1][1]=1/etb*(etb*etb*gm-(gm-4*eta*etb)*((q*q+p*p)*cp*cp-p*p));
            N[1][2]=-q*gm*sp;
            N[2][0]=-2*q*eta*etb*cp;
            N[2][1]=-2*q*eta*etb*sp;
            N[2][2]=2*eta*(q*q-p*p);

            for(k=0;k<3;k++)
                for(l=0;l<3;l++)
                    gs[k][l]=gs[k][l]+ C*hv(t[i]-t2)*creal(etb/sgm*N[k][l]/xi)*dp;
        }

        for(k=0;k<3;k++)
            for(l=0;l<3;l++)
            {
                GS[3*k+l][i]=gs[k][l];
            }
    }

    for(i=0;i<9;i++)
        der(GS[i],n1,dt);
 
    fd=fopen("Gij.txt","w");
    for(i=0;i<n1;i++)
    {
        for(j=0;j<9;j++)
        {
            GS[j][i]=GP[j][i]+GS[j][i];
            fprintf(fd,"%6f ",GS[j][i]);
        }
        fprintf(fd,"\n");
    }

    for(i=0;i<9;i++)
    {
        free(GP[i]);
        free(GS[i]);
    }
    free(t);
    return 0;
}
