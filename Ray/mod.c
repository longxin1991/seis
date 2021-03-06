#include <stdio.h>
#include <stdlib.h>
#include "dtype.h"

void read_mod(char *model_file,Model *mod)
{
    int stat,i=0;
    FILE *fp;
    char buff[20];

    if((fp=fopen(model_file,"r"))==NULL)
    {
        printf("Fail to read model file!\n");
        exit(0);
    }
    
    fgets(buff,19,fp);/*skip the file header*/
	if (VERBOSE == 1)
    {
        printf("---------------------\n");
        printf("Read model parameter.\n");
        printf("---------------------\n");
        printf("%s",buff);
    }
    while((fscanf(fp,"%f\t%f\t%f",&(mod->Vp[i]),&(mod->Vs[i]),&(mod->h[i]))) != EOF)
    {
        if (VERBOSE == 1)
            printf("%.1f\t%.1f\t%.1f\n",mod->Vp[i],mod->Vs[i],mod->h[i]);
        i += 1;
    }

    mod->n = i;
}
