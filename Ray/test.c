#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include "numc.h"
#include "ray.h"
#include "mod.h"

int main(int argc,char *argv[])
{
	char *phase = "PKiKP";
	float i1=0.0;
	float i2=PI/20;
	RayDataSP *Data;
	Model *mod;
	FILE *fp;
	int j,i;
	float delta;
	float *t,*d;

	struct raycor_sphere R1,R2;

	//delta = atof(argv[1]);
	R1.r = Re;
	R1.theta = i1;
	R1.phi = 0;

	R2.r = Re;
	R2.theta = i2;
	R2.phi = 0;
	
	Data = (RayDataSP *)malloc(1*sizeof(RayDataSP));
	Data->phase = phase;
	mod = (Model *)malloc(sizeof(Model));
	mod->h = (float *)malloc(MAXLAYER*sizeof(float));
    mod->Vp = (float *)malloc(MAXLAYER*sizeof(float));
    mod->Vs = (float *)malloc(MAXLAYER*sizeof(float));

	t = (float *)malloc(200*sizeof(float));
	d = (float *)malloc(200*sizeof(float));

	linspace(0,50,200,d);

	//read_mod("prem_cmb.txt",mod);
	read_mod("prem.nd",mod);

	//Data = ray_shoot_sp(R1,R2,mod,delta,phase);
	//ray_tracing_sp(R2,mod,Data);
	//exit(0);
	for (i=0;i<200;i++){
		//Data = ray_shoot_sp(R1,R2,mod,d[i],phase);
		//t[i] = Data->t;
		;
	}
	free_path_sp(Data);
	free(Data);
	free(t);
	free(d);
	free(mod->h);
	free(mod->Vp);
	free(mod->Vs);
	free(mod);
	return 0;
}
