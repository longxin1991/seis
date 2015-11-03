#include <stdio.h>
#include "dtype.h"

void getdata(char const *file,ITF *intf,int n)
{
    int i=0;
    char chr;
    FILE *fd;
    fd=fopen(file,"r");
	float *a1,*a2,*b1,*b2,*p1,*p2;
    while(i<n)
    {
		a1 = &(intf[i].a1);
		a2 = &(intf[i].a2);
		b1 = &(intf[i].b1);
		b2 = &(intf[i].b2);
		p1 = &(intf[i].p1);
		p2 = &(intf[i].p2);
        if((chr=fgetc(fd)) == '#'){
			while((chr=fgetc(fd)) != '\n');
		}
		else
		{
			fseek(fd,-1,SEEK_CUR);
			fscanf(fd,"%f %f %f %f %f %f",a1,b1,p1,a2,b2,p2);
			while((chr=fgetc(fd)) != '\n');
			i+=1;
		}
    }
    fclose(fd);
}

void print_matrix(int N,float *M,int m,int n)
{
	int i,j;
	switch(N){
		case 1:
			for (i=0;i<m;i++){
				for (j=0;j<n;j++)
					printf("%.3f ",M[i+m*j]);
				printf("\n");
			}
			break;
		case 0:
			for (i=0;i<m;i++){
				for (j=0;j<n;j++)
					printf("%.3f ",M[n*i+j]);
				printf("\n");
			}
			break;
		default:
			printf("strore method of the matrix should be given.\n");
			break;
	}
}

void print_parameter(ITF intf)
{
	float a1,b1,p1,a2,b2,p2;
	a1 = intf.a1;
	b1 = intf.b1;
	p1 = intf.p1;
	a2 = intf.a2;
	b2 = intf.b2;
	p2 = intf.p2;
	
	printf("%.2f %.2f %.2f %.2f %.2f %.2f\n",a1,b1,p1,a2,b2,p2);
}

