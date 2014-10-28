#include "dist.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc,char *argv[])
{
	cor res;
	double clat,clon,slat,slon,rx,ry,delay,ux,uy;
	
	if (argc != 7) {
		printf("Usage: distaz sta_lat sta_lon evt_lat evt_lon ux uy\n");
		exit(1);
	} 
	else {
	clat = atof(argv[1]);
	clon = atof(argv[2]);
	slat = atof(argv[3]);
	slon = atof(argv[4]);
	ux = atof(argv[5])/111.13;
	uy = atof(argv[6])/111.13;
	}
	
	cal_rela_cor(clat,clon,slat,slon,&res);
	rx=res.x_pos;
	ry=res.y_pos;
	
	delay=rx*ux+ry*uy;
	printf("%f\n",delay);
}