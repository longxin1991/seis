/**\file discorr.c
 * \brief discorr calculate the correction value of epicentral
 * distance for earthquake at different depth.
 *
 *
 * The distance after correction corresponds to that of a earth 
 *quake occurs at the Earth's surface. this program uses ttimes
 * to do the ray-tracing, then the obtained ray parameter is used
 * to calculate the distance correction using the flattening
 * transformation.
 *
 * \author longxin eri,u-tokyo
 * \date 15 Dec 2016
*/

#include "ioapi.h"
#include "mod.h"
#include "layer.h"
#include "numc.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc,char *argv[])
{
	float deg,evdp,svp,svs,svpf,svsf,szf,ds;
	int i,id,max=60,pn,n,sp,irtr;
	char *phase,phlst[max][8],phcd[max][8];
	float tt[max],dtdd[max],dtdh[max],dddp[max],p,dx,dt,x,t;
	float *vpf,*vsf,*d,*r,*zf;
	Model mod;


	if (argc < 4)
	{
		printf("usage:%s phase deg depth\n",argv[0]);
		exit(0);
	}
	else
	{
		phase = argv[1];
		deg = atof(argv[2]);
		evdp = atof(argv[3]);
	}

	mod.Vp = (float *)malloc(MAXLAYER*sizeof(float));
	mod.Vs = (float *)malloc(MAXLAYER*sizeof(float));
	mod.h = (float *)malloc(MAXLAYER*sizeof(float));
	mod.rho = (float *)malloc(MAXLAYER*sizeof(float));
	mod.Qp = (float *)malloc(MAXLAYER*sizeof(float));
	mod.Qs = (float *)malloc(MAXLAYER*sizeof(float));

	read_mod("/home/longxin/Workspace/seis/DisCorr/prem.nd",&mod);
	n = mod.n;
	source_env(evdp,mod,&sp,&svp,&svs);


	vpf = (float *)malloc(n*sizeof(float));
	vsf = (float *)malloc(n*sizeof(float));
	d = (float *)malloc((n-1)*sizeof(float));
	r = (float *)malloc(n*sizeof(float));
	zf = (float *)malloc(n*sizeof(float));
	
	for (i=0;i<n;i++)
	{
		r[i] = Re - mod.h[i];
		flat_tf(r[i],mod.Vp[i],&zf[i],&vpf[i]);
		flat_tf(r[i],mod.Vs[i],&zf[i],&vsf[i]);
	}

	depth_to_thickness(zf,d,n-1);
	flat_tf(Re - evdp,svp,&szf,&svpf);
	flat_tf(Re - evdp,svs,&szf,&svsf);
	ds = fabsf(szf - zf[sp]);

	//printf("%f %d %f\n",evdp,sp,ds);
	
	if ( strncmp("PKP",phase,3) == 0 )
	{
		strcpy(phlst[0],"PKP");
	}
	else
	{
		strcpy(phlst[0],phase);
	}

	c2fstr(phlst[0],8);
	pn = 1;

	getttimes(phlst,&evdp,&deg,&pn,tt,dtdd,dtdh,dddp,phcd,max,max,max,max,max,max);

	for (i=0;i<pn;i++)
	{
		f2cstr(phcd[i],8);
		//printf("ttime->%f rp->%f  dt2->%f  phase->%s\n",tt[i],dtdd[i],dddp[i],phcd[i]);
	}

	id = phase_id(phase,phcd,pn);

	if (id == -1)
	{
		printf("No %s arrive at %.3f\n",phase,deg);
		exit(0);
	}

	p = dtdd[id]*DEG2RAD/Re;
	x = 0;
	t = 0;
	for (i=0;i<=sp;i++)
	{
		if (i==sp)
		{
			d[i] = ds;
			vpf[i+1] = svpf;
			vsf[i+1] = svsf;
		}

		irtr = layerxt(p,d[i],1/vpf[i],1/vpf[i+1],&dx,&dt);
		x += dx;
		t += dt;
	}

	x = 180*x/(PI*Re);

	printf("%f %f %f\n",x,t,x+deg);

	return 0;
}
