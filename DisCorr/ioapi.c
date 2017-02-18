#include "ioapi.h"

void c2fstr(char *str,int size)
{
	int i,slen;
	slen = strlen(str);
	/*pad space behind the string*/
	for (i=slen;i<size;i++)
	{
		str[i] = ' ';	
	}
}

void f2cstr(char *str,int size)
{
	int i,slen;
	for (i=size-1;str[i] == ' ';i--);
	slen = i+1;
	str[slen] = '\0';
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
