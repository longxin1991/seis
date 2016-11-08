#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "sac.h"

typedef struct sta_para
{
	char **fname;
	double *dis;
	double *t_df;
	double *t_bc;
	double *t_ab;
	int n;
}PARA;

PARA read_para(const char *);
int get_line_num(const char *);
SacTrace read_sac_tr(const char *);
void cut_data(SacTrace,const char,double,int,double *);
int get_max_index(float *,int ,int ,int);
void trim(char *);
