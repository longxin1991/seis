#include <stdio.h>

void getdata(char const *file,float para[],int n)
{
    int i=0;
    char chr;
    FILE *fd;
    fd=fopen(file,"r");

    while(i<n)
    {
        if((chr=fgetc(fd)) != '#' && chr !=' '){
            fseek(fd,-1,SEEK_CUR);
            fscanf(fd,"%f",&para[i]);
            i+=1;
        }
        else
        {
            while((chr=fgetc(fd)) !='\n');
        }
    }
    fclose(fd);
}
