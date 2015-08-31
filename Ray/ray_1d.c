#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include "numc.h"
#include "ray.h"
#include "mod.h"

/*Intial Parameter*/
float i1=PI/96;
//float i1=0.816;
float i2=PI/2;

int main(int argc,char *argv[])
{
    RayData *Data;
    Model *mod;
    int i=0,j=0,n,opt;
    float d1,d2,i0=0,d0=0,*Vp,*Vs,*h,x0,z0,d;
    FILE *fp;
    char *model_file,phase,*output;

    struct raycor R1,R2;

/* parse parameter*/
    if (argc < 12 ){
        fprintf(stderr, "Usage: %s output -x xloc -z zloc -d dis [-m] model_file [-p] phase\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    //output = argv[1];
    while((opt=getopt(argc,argv,"h:x:z:m:p:d:"))!=-1)
    {
        switch(opt){
            case 'm':
                model_file=optarg;
                break;
            case 'p':
                phase=optarg[0];
                break;
            case 'x':
            	x0=atof(optarg);
            	break;
            case 'z':
            	z0=atof(optarg);
            	break;
           	case 'd':
           		d=atof(optarg);
           		break;
            default:
                fprintf(stderr, "Usage: %s -m model_file -p phase\n",argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    argc -= optind;
    argv += optind;
    //exit(0);
    /*define angle for bisecition method*/
    R1.rx=x0;
    R1.rz=z0;
    R1.ri=i1;

    R2.rx=x0;
    R2.rz=z0;
    R2.ri=i2;
   /*-----------------------------------*/ 
/* read model parameter */
    mod = (Model *)malloc(sizeof(Model));
    mod->h = (float *)malloc(MAXLAYER*sizeof(float));
    mod->Vp = (float *)malloc(MAXLAYER*sizeof(float));
    mod->Vs = (float *)malloc(MAXLAYER*sizeof(float));

    output = argv[0];

    read_mod(model_file,mod);
    //exit(0);
    //printf("%d\n",source_layer(mod->h,mod->n,1000));
    //exit(0);
/* -------------------- */
    Data = ray_shoot(R1,R2,mod,d,phase);
  
/*write ray data to file*/
    fp=fopen(output,"w");
    for (j=0;j<Data->np-1;j++)
        fprintf(fp,"%.3f %.3f\n",Data->x[j],-1*Data->z[j]);
	fclose(fp);
/* ---------------------*/
    free_path(Data);
    free(Data);
    return 0;
}
