#include <complex.h>
#include <math.h>
#include <stdio.h>
#include "numc.h"

void qresp(double tstar,int n,double df,double complex *qspec)
{
	int i;
	double f0 = 1.0,f;
	double complex q;

	qspec[0] = 1.0;
	qspec[n/2] = 0.0;
	
	for (i=1;i<n/2;i++)
	{
		f = df*i;
		q = I*2*tstar*f*clog(I*f/f0);
		qspec[i] = cexp(q);
		qspec[n-i] = conj(qspec[i]);
	}
}

/*Absorption band model defined by Cormier and Richards (1988). */
/*                      Velocity Dispersion                     */
double complex alpha_feq(double f,double tau_1,double tau_2,double q0)
{
	double complex D,Qm,t1,t2,d0,alpha;

	d0 = clog((I*2*PI+1/tau_1)/(I*2*PI+1/tau_2));
	D = clog((I*f*2*PI+1/tau_1)/(I*f*2*PI+1/tau_2));
	t1 = 2*PI*q0;
	t2 = (4-q0*q0)*cimag(d0)-4*q0*creal(d0);
	Qm = 1*t1/t2;
	//Qm = 0.0016;

	t1 = csqrt(1+2*Qm*D/PI);
	t2 = creal(csqrt(1+2*d0*Qm/PI));
	alpha = t1/t2;
	
	return alpha;
}

void qresp_abs(int n,double df,double tau_1,double tau_2,double q0,double t_ic,double complex *qs)
{
	int i;
	double f;
	double complex q,alpha,s;

	qs[0] = 1.0;
	qs[n/2] = 0.0;

	for (i=1;i<n/2;i++)
	{
		f = df*i;
		alpha = alpha_feq(f,tau_1,tau_2,q0);
		s = 1.0/alpha;
		q = -2*PI*I*f*(s-1.0)*t_ic;
		//printf("f->%f  rs->%f  is->%f rq->%f iq->%f\n",f,creal(s),cimag(s),creal(q),cimag(q));
		qs[i] = cexp(q);
		qs[n-i] = conj(qs[i]);
	}
}
