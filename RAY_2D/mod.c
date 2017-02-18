#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mod.h"
#include "numc.h"

void read_mod(char *model_file,Model *mod)
{
    int stat,i=0;
    FILE *fp;
    char buff[40];

    if((fp=fopen(model_file,"r"))==NULL)
    {
        printf("Fail to read model file!\n");
        exit(0);
    }
    
    fgets(buff,39,fp);/*skip the file header*/

	if (VERBOSE == 1)
    {
        printf("---------------------\n");
        printf("Read model parameter.\n");
        printf("---------------------\n");
        printf("%s",buff);
    }
    
    while((fscanf(fp,"%f%f%f%f%f%f",&(mod->h[i]),&(mod->rho[i]),\
    &(mod->Vp[i]),&(mod->Vs[i]),&(mod->Qp[i]),&(mod->Qs[i]))) != EOF)
    {
        if (VERBOSE == 1)
            printf("%.6f\t\t%.6f\t\t%.6f\n",mod->Vp[i],mod->Vs[i],mod->h[i]);
        i += 1;
    }
    mod->n = i;
}

float prem_iso(float r,char *param,int idom)
{
	float x,ro,vp,vs,Qmu,Qkappa;

	x = r/6371.;

	if (idom == 0) /* upper crustal layer */
	{	
		ro = 2.6;
		vp = 5.8;
		vs = 3.2;
		Qmu = 600.0;
		Qkappa = 57827.0;
	}
	else if (idom == 1) /* lower crustal layer */
	{
		ro = 2.9;
		vp = 6.8;
		vs = 3.9;
		Qmu = 600.0;
		Qkappa = 57827.0;
	}
	/* upper mantle */
	else if (idom == 2) /* LID */
	{
		ro = 2.691  + 0.6924 * x;             
		vp = 4.1875 + 3.9382 * x;
		vs = 2.1519 + 2.3481 * x;
		Qmu = 600.0;
		Qkappa = 57827.0;
	}
	else if (idom == 3) /* LVZ */
	{
		ro = 2.691  + 0.6924 * x;
		vp = 4.1875 + 3.9382 * x;
		vs = 2.1519 + 2.3481 * x;
		Qmu = 80.0;
		Qkappa = 57827.0;
	}
	/* Transition zone */
	else if (idom == 4) /* 220->400 */
	{
		ro =  7.1089 -  3.8045 * x;
		vp = 20.3926 - 12.2569 * x;
		vs =  8.9496 -  4.4597 * x;
		Qmu = 143.0;
		Qkappa = 57827.0;
	}
	else if (idom == 5) /* 400->600 */
	{
		ro = 11.2494 -  8.0298 * x;
		vp = 39.7027 - 32.6166 * x;
		vs = 22.3512 - 18.5856 * x;
		Qmu = 143.0;
		Qkappa = 57827.0;
	}
	else if (idom == 6) /* 600->670 */
	{
		ro =  5.3197 - 1.4836 * x;
		vp = 19.0957 - 9.8672 * x;
		vs =  9.9839 - 4.9324 * x;
		Qmu = 143.0;
		Qkappa = 57827.0;
	}
	/* lower mantle */
	else if (idom == 7) /* 670->770 */
	{
		ro =  7.9565 -  6.4761 * x + 5.5283 * pow(x,2) - 3.0807 * pow(x,3);
		vp = 29.2766 - 23.6027 * x + 5.5242 * pow(x,2) - 2.5514 * pow(x,3);
		vs = 22.3459 - 17.2473 * x - 2.0834 * pow(x,2) + 0.9783 * pow(x,3);
		Qmu = 312.0;
		Qkappa = 57827.0;
	}
	else if(idom == 8) /* 770->top D'' */
	{
		ro  =  7.9565 -  6.4761 * x +  5.5283 * pow(x,2) -  3.0807 * pow(x,3);
		vp  = 24.9520 - 40.4673 * x + 51.4832 * pow(x,2) - 26.6419 * pow(x,3);
		vs  = 11.1671 - 13.7818 * x + 17.4575 * pow(x,2) -  9.2777 * pow(x,3);
		Qmu = 312.0;
		Qkappa = 57827.0;
	}
	else if (idom == 9) /* D'' -> CMB */
	{
		ro =  7.9565 - 6.4761 * x + 5.5283 * pow(x,2) - 3.0807 * pow(x,3);
		vp = 15.3891 - 5.3181 * x + 5.5242 * pow(x,2) - 2.5514 * pow(x,3);
		vs =  6.9254 + 1.4672 * x - 2.0834 * pow(x,2) + 0.9783 * pow(x,3);
		Qmu = 312.0;
		Qkappa = 57827.0;
	}
	/* outer core */
	else if (idom == 10)
	{
		ro = 12.5815 - 1.2638 * x - 3.6426 * pow(x,2) -  5.5281 * pow(x,3);
		vp = 11.0487 - 4.0362 * x + 4.8023 * pow(x,2) - 13.5732 * pow(x,3);
		vs = 0.0;
		Qmu = 0.0;
		Qkappa = 57827.0;
	}
	/* inner core */
	else if (idom == 11)
	{
		ro = 13.0885 - 8.8381 * pow(x,2);
		vp = 11.2622 - 6.3640 * pow(x,2);
		vs =  3.6678 - 4.4475 * pow(x,2);
		Qmu = 84.6;
		Qkappa = 1327.7;
	}
	else
	{
		printf("Domain %d does not exist for prem_iso.\n",idom);
		exit(0);
	}

	if (strcmp(param,"rho") == 0)
		return ro;
	else if (strcmp(param,"vp") == 0)
		return vp;
	else if (strcmp(param,"vs") == 0)
		return vs;
	else if (strcmp(param,"Qmu") == 0)
		return Qmu;
	else if (strcmp(param,"Qka") == 0)
		return Qkappa;
	else
	{
		printf("No option %s for prem_iso.\n",param);
		exit(0);
	}
}

void prem_disc(Disc **discont_prem,int *ndisc)
{
	int ndisc_prem=11;
	Disc *discont;

	*ndisc = ndisc_prem;
	discont = (Disc *)malloc(ndisc_prem*sizeof(Disc));
	
	discont[0].radius = 6356.;
	discont[0].vp1 = 5.8;
	discont[0].vp2 = 6.8;

	discont[1].radius = 6346.6;
	discont[1].vp1 = 6.8;
	discont[1].vp2 = 8.11061;

	discont[2].radius = 6291.;
	discont[2].vp1 = 8.0762;
	discont[2].vp2 = 8.0762;

	discont[3].radius = 6151.;
	discont[3].vp1 = 7.9897;
	discont[3].vp2 = 8.55896; 

	discont[4].radius = 5971.;
	discont[4].vp1 = 8.90522;
	discont[4].vp2 = 9.133917;

	discont[5].radius = 5771.;
	discont[5].vp1 = 10.15783;
	discont[5].vp2 = 10.15776;

	discont[6].radius = 5701.;
	discont[6].vp1 = 10.26617;
	discont[6].vp2 = 11.7513;

	discont[7].radius = 5600.;
	discont[7].vp1 = 11.0656;
	discont[7].vp2 = 11.0656;

	discont[8].radius = 3630.;
	discont[8].vp1 = 13.6804;
	discont[8].vp2 = 13.6805;

	discont[9].radius = 3480.;
	discont[9].vp1 = 13.7166;
	discont[9].vp2 = 8.0650;

	discont[10].radius = 1221.5;
	discont[10].vp1 = 10.3557;
	discont[10].vp2 = 11.0283;

	*discont_prem = discont;
}

void generate_rgrid(Disc *discont,int ndisc,float **h,float **vp,int *nr)
{
	float maxstep=50.0,d,lthickness;
	float ur,lr,*r,*v_p;
	int i,j,n,ln[ndisc+1];

	r = (float *)malloc(1000*sizeof(float));
	v_p = (float *)malloc(1000*sizeof(float));

	n = 0;
	/* loop of domain separated by discontinuities.*/
	for (i=0;i<=ndisc;i++)
	{
		d = 0;
		j = 0;
		if (i == 0)
		{	
			ur = 6371.0;
			lr = discont[0].radius;
			lthickness = ur - lr;
		}
		else if (i == ndisc)
		{
			ur = discont[ndisc-1].radius;
			lr = 0.0;
			lthickness = ur;
		}	
		else 
		{
			ur = discont[i-1].radius;
			lr = discont[i].radius;
			lthickness = ur - lr;
		}
		r[n] = ur;
		v_p[n] = prem_iso(r[n],"vp",i);
		n += 1;
		if (lthickness < maxstep )
			j = 0;
		else
		{
			do
			{
				d += maxstep;
				j += 1;
				r[n] = ur - d;
				v_p[n] = prem_iso(r[n],"vp",i);
				n += 1;
			}while(d<lthickness);
		}
		if (j>0)
		{
			n -= 1;
			r[n] = lr;
			v_p[n] = prem_iso(r[n],"vp",i);
			n += 1;
		}
		else
		{
			r[n] = lr;
			v_p[n] = prem_iso(r[n],"vp",i);
			n += 1;
		}

		ln[i] = j;
	}
	*h = (float *)malloc(n*sizeof(float));
	*vp= (float *)malloc(n*sizeof(float));

	for (i=0;i<n;i++)
	{
		(*h)[i] = 6371.0 - r[i];
		(*vp)[i] = v_p[i];
	}
	*nr = n;
	free(r);
	free(v_p);
}

void mk_1d_prem(Model *mod)
{
	Disc *discont;
	int ndisc,nr,i;
	float *h,*vp;

	prem_disc(&discont,&ndisc);
	generate_rgrid(discont,ndisc,&h,&vp,&nr);

	mod->Vp = vp;
	mod->h = h;
	mod->n = nr;
	/*
	for (i=0;i<nr;i++)
	{
		printf("%10.1f %10.6f\n",mod->h[i],mod->Vp[i]);
	}
	*/
}
