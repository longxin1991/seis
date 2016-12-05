#include "corr.h"
#include "layer.h"
#include "numc.h"
#include "ioapi.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char *argv[])
{
	Model mod;
	CMT mt;
	char *funcname = "geo_spread";
	int i,j,stat,n,sp,nd=100;
	float svp,svs,rvp,rvs,gs,rt,rp,evdp=588,az=310,*dis,cc;
	/* define variable for ttime calculation */
	int max = 60,strl,pn=2;
	char phlst[max][8],phcd[max][8];
	float depth,delta;
	float tt[max],dtdd[max],dtdh[max],dddp[max];


	dis = (float *)malloc(nd*sizeof(float));

	linspace(145,154,nd,dis);
	mod.Vp = (float *)malloc(MAXLAYER*sizeof(float));
    mod.Vs = (float *)malloc(MAXLAYER*sizeof(float));
	mod.h = (float *)malloc(MAXLAYER*sizeof(float));
	mod.rho = (float *)malloc(MAXLAYER*sizeof(float));
	read_mod("prem.nd",&mod);

	read_mt(&mt,&evdp);
	//printf("%.2f %.2f %.2f %.2f %.2f %.2f\n",mt.mrr,mt.mtt,mt.mff,mt.mrt,mt.mrf,mt.mtf);

	n = mod.n;
	
	source_env(evdp,mod,&sp,&svp,&svs);

	rvp = mod.Vp[0];
	rvs = mod.Vs[0];
	
	strcpy(phlst[0],"PKP");
	strcpy(phlst[1],"PKiKP");
	pn = 2;

	for (i=0;i<pn;i++)
	{
		c2fstr(phlst[i],8);
	}

	for (i=0;i<nd;i++)
	{
		depth = evdp;
		delta = dis[i];
		pn  = 2;	
		getttimes_(phlst,&depth,&delta,&pn,tt,dtdd,dtdh,dddp,phcd,max,max,max,max,max,max);
		/*
		for (j=0;j<pn;j++)
		{
			f2cstr(phcd[j],8);
			printf("ttime->%f rp->%f  dt2->%f  phase->%s\n",tt[j],dtdd[j],dddp[j],phcd[j]);
		}
		*/
		gs = corr_gs(evdp,svp,rvp,dtdd[0],dtdd[1],dddp[0],dddp[1]);
		rt = corr_rt(mod,dtdd[0],dtdd[1]);
		rp = corr_rp(mt,evdp,svp,dtdd[0],dtdd[1],az);
		cc = gs*rt*rp;
		printf("%.3f %f %f %f %f\n",dis[i],gs,rt,rp,cc);
	}
	return 0;
}

void read_mt(CMT *m,float *evdp)
{
	float mrr,mtt,mff,mrt,mrf,mtf;
	scanf("%f %f %f %f %f %f %f",evdp,&mrr,&mtt,&mff,&mrt,&mrf,&mtf);

	m->mrr = mrr;
	m->mtt = mtt;
	m->mff = mff;
	m->mrt = mrt;
	m->mrf = mrf;
	m->mtf = mtf;
}
