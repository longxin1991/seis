#include "grid.h"
#include <stdio.h>
#include "numc.h"

void get_index(double r,double theta,int nth,int nr,float *h,Ind ind[2])
{
	int i;
	double dtheta,z;

	dtheta = 180.0/nth;
	ind[0].x = (int)(theta/dtheta);
	ind[1].x = ind[0].x + 1;

	z = 6371.0 - r;
	for (i=1;i<nr;i++)
	{
		if (z >= h[i-1] && z < h[i])
			break;
	}

	ind[0].y = i-1;
	ind[1].y = i;
}

double interp_sl(double uls,double lrs,double ur,double lr,double r,double *k)
{
	double s,k0;

	k0 = (uls - lrs)/(ur-lr);
	s = uls - k0*(ur - r);
	*k = k0;
	return s;
}

double bilinear(double uls,double urs,double lls,double lrs,double ur,double lr,double lt,double rt,double r,double theta,double *kr,double *kt)
{
	double s,K,L,M,N,O;

	O = (rt - lt)*(ur - lr)*1000;
	K = (rt - theta)*(ur - r)*1000;
	L = (theta - lt)*(ur - r)*1000;
	M = (rt - theta)*(r - lr)*1000;
	N = (theta - lt)*(r - lr)*1000;

	s = (lls*K + lrs*L + uls*M + urs*N)/O;

	*kr = 1000*(urs*(theta - lt) + uls*(rt - theta) - lrs*(theta - lt) - lls*(rt - theta))/O;
	*kt = 1000*(urs*(r - lr) - uls*(r - lr) + lrs*(ur - r) - lls*(ur - r))/O;
	return s;
}

int incell(double ur,double lr,double lt,double rt,double r,double theta)
{
	//if (r>ur || r<lr || theta <lt || theta > rt)
	if (r>ur || r<lr)
		return 0;
	else
		return 1;
}

int pass_boundary(double rt,double theta,double hit_miss,double lmiss)
{
	if (theta>rt && lmiss>hit_miss)
		return 1;
	else
		return 0;
}
