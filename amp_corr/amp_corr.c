#include "corr.h"
#include "layer.h"
#include "numc.h"
#include "ioapi.h"
#include <stdio.h>
#include <stdlib.h>

int phase_id(char const *phase,char (*phcd)[8],int pn);

int main(int argc,char *argv[])
{
	Model mod;
	CMT mt;
	char *phase1,*phase2;
	int i,j,stat,n,sp,nd=100,id1,id2;
	float svp,svs,rvp,rvs,gs,rt,rp,evdp=588,az=310,*dis,cc;
	/* define variable for ttime calculation */
	int max = 60,strl,pn=2;
	char phlst[max][8],phcd[max][8];
	float depth,delta,deg;
	float tt[max],dtdd[max],dtdh[max],dddp[max];

	if (argc < 4)
	{
		printf("usage:%s phase1 phase2 < stdin\n\n",argv[0]);
		printf("deg: Epicentral distance\n");
		printf("phase1: PKPdf\n");
		printf("phase2: PKPbc or PKPab\n");
		printf(" stdin: evdp mrr mtt mff mrt mrf mtf\n");
		exit(0);
	}
	else
	{
		deg = atof(argv[1]);
		phase1 = argv[2];
		phase2 = argv[3];
	}

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

	//for (i=0;i<nd;i++)
	//{
		depth = evdp;
		delta = deg;
		pn  = 2;	
		getttimes_(phlst,&depth,&delta,&pn,tt,dtdd,dtdh,dddp,phcd,max,max,max,max,max,max);
		
		for (j=0;j<pn;j++)
		{
			f2cstr(phcd[j],8);
			//printf("ttime->%f rp->%f  dt2->%f  phase->%s\n",tt[j],dtdd[j],dddp[j],phcd[j]);
		}
		id1 = phase_id(phase1,phcd,pn);	
		id2 = phase_id(phase2,phcd,pn);

		if (id1 == -1 || id2 == -1 )
		{
			if (id1 == -1)
				printf("No %s arrive at %.3f\n",phase1,deg);
			if (id2 == -1)
				printf("No %s arrive at %.3f\n",phase2,deg);
			exit(0);
		}

		gs = corr_gs(evdp,svp,rvp,dtdd[id1],dtdd[id2],dddp[id1],dddp[id2]);
		rt = corr_rt(mod,dtdd[id1],dtdd[id2]);
		rp = corr_rp(mt,evdp,svp,dtdd[id1],dtdd[id2],az);
		cc = gs*rt*rp;
		printf("%.3f %f %f %f %f\n",deg,gs,rt,rp,cc);
	//}
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

int phase_id(char const *phase,char (*phcd)[8],int pn)
{
	int i,slen,stat=-1;
	
	for (i=0;(i<pn && stat!=0);i++)
	{
		slen = strlen(phcd[i]);
		stat = strcmp(phase,phcd[i]);
	}
	//printf("%s->%d\n",phase,i-1);
	if (stat == 0)
		return i-1;
	else
		return stat;
}
