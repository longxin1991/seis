#include <math.h>

int layerxt(float p,float h,float utop,float ubot,float *dx,float *dt)
{
	int irtr;
	float eta1,eta2,tau1,tau2,dtau,x1,x2,u1,u2,v1,v2,b;
	if(p > utop)
	{
		*dx=0.0;
		*dt=0.0;
		irtr=0;
		return irtr;
	}
	else if(h == 0.0)
	{
		*dx=0.0;
		*dt=0.0;
		irtr=-1;
		return irtr;
	}

	u1=utop;
	u2=ubot;
	v1=1./u1;
	v2=1./u2;
	b=(v2-v1)/h;

	eta1=sqrt(u1*u1-p*p);

	if(b ==0)
	{
		*dx=h*p/eta1;
		*dt=h*u1*u1/eta1;
		irtr=1;
		return irtr;
	}

	x1=eta1/(u1*b*p);
	tau1=(log((u1+eta1)/p)-eta1/u1)/b;

	if(p > ubot)
	{
		*dx=x1;
		dtau=tau1;
		*dt=dtau+p*(*dx);
		irtr=2;
		return irtr;
	}

	irtr=1;

	eta2=sqrt(u2*u2-p*p);
	x2=eta2/(u2*b*p);
	tau2=(log((u2+eta2)/p)-eta2/u2)/b;

	*dx=x1-x2;
	dtau=tau1-tau2;

	*dt=dtau+p*(*dx);

	return irtr;
}
