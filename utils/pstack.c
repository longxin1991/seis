#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sacio.h>
#include <unistd.h>
#include "sac.h"

void listdir(char *path,int *len,char *filename[])
{

}

int main(int argc,char *argv[])
{
	char *path,*filename;
	int j=0,i=0,npts;
	SACHEAD hd,chd;
	char *cstnm,*kstnm;
	float *data,*sum;
	if(argc!=3)
	{
		printf("usage:pstack sacdir outputs\n");
		exit(0);
	}
	else
		path=argv[1];
	
	DIR *d;
	char *name;
	struct dirent *file;

	if(!(d = opendir(path)))
	{
		printf("error opendir %s!!!\n",path);
		exit(0);
	}
	else
		chdir(path);
	
	while((file=readdir(d)) != NULL)
	{
		if(strncmp(file->d_name, ".", 1) == 0)
			continue;
		filename=file->d_name;
		data=read_sac(filename,&hd);
		npts=hd.npts;
		kstnm=hd.kstnm;
		if (i==0)
			sum=read_sac(filename,&hd);
		else
		{
			for (j=0;j<npts;j++)
			{
				sum[j]+=data[j];
			}
			if((strcmp(kstnm,"HOT26"))==0)
				chd=hd;
		}
		i++;
	}
	
	for (j=0;j<npts;j++)
		sum[j]=sum[j]/i;
	
	write_sac(argv[2],chd,sum);
	closedir(d);
	return 0;
}
