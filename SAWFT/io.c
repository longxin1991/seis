#include "io.h"


PARA read_para(const char *inf)
{
	PARA dt;
	FILE *fp;
	int i,n;
	char **fname;
	double *dis,*t_df,*t_bc,*t_ab;
	float a,b,c,d;

	n = get_line_num(inf);

	fname = (char **)malloc(n*sizeof(char *));
	dis = (double *)malloc(n*sizeof(double));
	t_df = (double *)malloc(n*sizeof(double));
	t_bc = (double *)malloc(n*sizeof(double));
	t_ab = (double *)malloc(n*sizeof(double));
	dis[0]=1;dis[1]=2;
	if((fp=fopen(inf,"r"))==NULL)
	{
		printf("Fail to open input file!\n");
		exit(0);
	}

	for (i=0;i<n;i++)
	{
		fname[i] = (char *)malloc(MAXFNLEN*sizeof(char));
		fscanf(fp,"%s %f %f %f %f",fname[i],&a,&b,&c,&d);
		dis[i] = a; t_df[i] = b; t_bc[i] = c; t_ab[i] = d;
	}
	dt.n = n;
	dt.fname = fname;
	dt.t_df = t_df;
	dt.t_bc = t_bc;
	dt.t_ab = t_ab;
	dt.dis = dis;

	return dt;
}

int get_line_num(const char *inf)
{
	char ch;
	int n=0;
	FILE *fp;
	if((fp=fopen(inf,"r"))==NULL)
	{
		printf("Fail to open %s!\n",inf);
		exit(0);
	}
	do{
		ch = fgetc(fp);
		if (ch == '\n')
			n++;
	}while(ch != EOF);
	fclose(fp);
	return n;
}

SacTrace read_sac_tr(const char *sacfile)
{
	SacTrace tr;
	tr.data = read_sac(sacfile,&tr.hd);
	return tr;
}

void cut_data(SacTrace tr,const char pick,double t_pre,int npts,double *data)
{
	int i,p_index,b,e,max_index,range;
	double t0,tp,delta;
	
	switch (pick){
		case '0':
			tp = tr.hd.t0;
			break;
		case '1':
			tp = tr.hd.t1;
			break;
		case '2':
			tp = tr.hd.t2;
			break;
		default:
			{
				printf("no such pick id\n");
				exit(0);
			}
	}
	
	t0 = tr.hd.b;
	delta = tr.hd.delta;
	p_index = (int)((tp - t0)/delta) + 1;
    range = (int)(0.25/delta);

	max_index = get_max_index(tr.data,p_index,range,range);
    
	b = max_index - (int)(t_pre/delta);
	e = b + npts;
	for (i=b;i<e;i++)
		data[i-b] = tr.data[i];

}

int get_max_index(float *data,int p_index,int pre,int post)
{
	int i,b,e,max_index=0;
	float tmp,max=0;
	
	b = p_index - pre;
	e = p_index + post;

	for(i=b;i<e;i++)
	{
		tmp = data[i]*data[i];
		if (tmp > max)
		{
			max = tmp;
			max_index = i;
		}
	}
	return max_index;
}

void trim(char *str)
{
  char* i = str;
  char* j = str;
  while(*j != 0)
  {
    *i = *j++;
    if(*i != ' ')
      i++;
  }
  *i = 0;	
}
