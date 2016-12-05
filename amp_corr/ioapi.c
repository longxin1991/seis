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
