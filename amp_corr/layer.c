#include <math.h>
#include "dtype.h"
#include "numc.h"
#include <stdio.h>
#include <stdlib.h>

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

/*The Earth-flattening transformation*/
void flat_tf(float r,float v,float *zf,float *vf)
{
	if (r-1<1.0)
	{		
		*zf = -1*Re*log(1.0/Re);
		*vf = v*Re/1.0;
	}
	else
	{
		*zf = -1*Re*log(r/Re);
		*vf = v*Re/r;
	}
}

void depth_to_thickness(float *depth,float *d,int ln)
{
    int i;
    for (i=0;i<ln;i++)
        {
            d[i] = depth[i+1] - depth[i];
        }
}

void cal_ic_time(float p,Model mod,float *t)
{
	int i,icb,ln,irtr;
	float *d,*vp,dx=0,dt=0;
	
	*t =0;
	icb = 74;
	ln = mod.n - 1;
	d = mod.h;
	vp = mod.Vp;

	for (i=74;i<ln;i++)
	{
		irtr = layerxt(p,d[i],1.0/vp[i],1.0/vp[i+1],&dx,&dt);
		*t += dt;
		if (irtr == 2)
			break;
	}
	*t = *t*2;
}

void layer_trace(Model mod,float p,float *t,float *x,int *m)
{
	int ln,i,j,irtr,stop=0,tn,local;
	float *d,*vp,tau,dx=0,dt=0,dis,time;

	time = 0;
	dis = 0;
	ln = mod.n - 1;
	d = mod.h;
	vp = mod.Vp;
	for (i=0;i<ln;i++)
	{	
		irtr = layerxt(p,d[i],1.0/vp[i],1.0/vp[i+1],&dx,&dt);
		dis += dx;
		time += dt;
		if (irtr == 2){
			if (i>73)
				*m = 2; /*Ray turns in the IC*/
			else if (i<50)
				*m = 0; /*Ray turns in the mantle*/
			else
				*m = 1; /*Ray turns in the OC*/
			break;
		}
	}
	*x = 360*dis/(PI*Re);
	*t = time*2;

}

void ray_shoot(Model mod,float p1,float p2,float deg,float *t,float *p)
{
	int i=0,m,m1,m2;
	float i1,i2,i0,x0,x1,x,t0,t1,t2,x2;
	/* p1 < p2 */
	i1 = p1;
	i2 = p2;
	x0 = 180;
	layer_trace(mod,i1,&t1,&x1,&m1);
	layer_trace(mod,i2,&t2,&x2,&m2);
	//printf("x1-> %.4f  x2-> %.4f\n",x1,x2);
	//printf("m1-> %d  m2-> %.d\n",m1,m2);

	while ((x0-deg)*(x0-deg) > 0.00001)
	{
		m =0;
		i0 = (i1+i2)/2;
		layer_trace(mod,i0,&t0,&x0,&m);
		
		//printf("%d %f  %f %f %d\n",i,i0,x0,t0,m);

		if (x0>deg)
			i1 = i0;
		else
			i2 = i0;
		i += 1;
		if (i>=30)
		{
			i0 = -1;
			break;
		}
	}
	x = x0;
	*t = t0;
	*p = i0;
}

void divide_layer(float v1,float v2,float d,int n,float *v)
{
	int i;
	float *z;

	z = (float *)malloc(n*sizeof(float));
	linspace(0,d,n,z);

	v[0] = v1;
	v[n-1] = v2;

	for (i=1;i<n-1;i++)
		v[i] = v1 + z[i]*(v2-v1)/d;

	free(z);
}

float tstar_ic_l(float dist,Model mod,int dn)
{
	int i,j,icb,irtr,ln;
	float p,t_ray,dx=0,dt=0,*d,*vp,*vs,Q,Qu,Qk,t=0;
	float *ivp,*ivs,dz,tmp,dts=0;

	Qu = 84.6;
	Qk = 1327.7;
	icb = 74;
	ln = mod.n - 1;
	d = mod.h;
	vp = mod.Vp;
	vs = mod.Vs;

	ivp = (float *)malloc(dn*sizeof(float));
	ivs = (float *)malloc(dn*sizeof(float));

	ray_shoot(mod,0.0017,0.017381,dist,&t_ray,&p);
	
	for (i=icb;i<ln;i++)
	{
		divide_layer(vp[i],vp[i+1],d[i],dn,ivp);	
		divide_layer(vs[i],vs[i+1],d[i],dn,ivs);
		dz = d[i]/(dn-1);

		for (j=0;j<dn-1;j++)
		{
			irtr = layerxt(p,dz,1.0/ivp[j],1.0/ivp[j+1],&dx,&dt);
			tmp = 4.0*ivs[j]*ivs[j]/(3.0*ivp[j]*ivp[j]);
			Q = tmp/Qu + (1 - tmp)/Qk;
			dts += dt*Q;
			t += dt;
			if (irtr == 2)
				break;
		}

		if (irtr == 2)
			break;
	}
	free(ivp);free(ivs);
	return dts*2;
}

void source_env(float evdp,Model mod,int *sp,float *vp,float *vs)
{
	int i=0,n,l;
	float d;

	n = mod.n;

	while ( mod.h[i] < evdp )
		i++;

	l = i;
	*sp = l-1;
	d = mod.h[l] - mod.h[l-1];
	*vp = mod.Vp[l-1] + (evdp - mod.h[l-1])*(mod.Vp[l] - mod.Vp[l-1])/d;
	*vs = mod.Vs[l-1] + (evdp - mod.h[l-1])*(mod.Vs[l] - mod.Vs[l-1])/d;
}
