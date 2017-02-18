#include "io.h"
#include "transform.h"
#include "waveform.h"
#include "utils.h"
#include "numc.h"
#include <math.h>
#include <time.h>
#include <omp.h>


void average(double **,double *,double *,int,int);

int main(int argc,char *argv[])
{
	PARA dt;
	SacTrace *st;
	int i,j,k,l,m,n,nsta,npts=900,nloop,nrun,nret,ktau,kave,N,mnam;
	char inf[20] = "theo.txt",*energy = "energy.txt",buf[20];
	int *dfshift,*abshift,*kappa,u,se,sb,tid;
	double **data,*s,*tau,*tdf,*tab,*adf,*aab,*amp,*tsta;
	double delta,tbs,nadf,naab,tsmax,stime,rtime,ds;
	double e1,e2,gamma,temp,der,alpha,p,rand;
	double **obc,**odf,**oab,**nbc,**ndf,**nab,**qq,*ns,**q,*namp,*derr,*ntsta;
	double *ntau,*dummy,**atsta,**aamp,**aadf,**aaab,**atau,**atdf,**atab,*tstastd;
	double t1,t2,*ampstd,*adfstd,*aabstd,*taustd,*tdfstd,*tabstd;
	FILE **ofp,*rfp,*sfp,*efp;

	tbs = -20;
	nadf = 3; naab = 3; tsmax = 2;
	stime = -2.0; rtime = 6.0;
	
	if (argc < 4 || argc > 7)
	{
		printf("usage:%s nloop nrun nret [input] [stime] [rtime]\n",argv[0]);
		exit(0);
	}
	else if (argc >=4 )
	{	
		nloop = (int)(atof(argv[1]));
		nrun = (int)(atof(argv[2]));
		nret = (int)(atof(argv[3]));
		if (argc >= 5)
		{
			memset(inf,0,20);
			memcpy(inf,argv[4],strlen(argv[4]));
		}
		if (argc == 7)
		{
			stime = atof(argv[5]);
			rtime = atof(argv[6]);
		}
	}

	dt = read_para(inf);
	nsta = dt.n;

	/*Prepare data and allocate memory for each variable*/
	st = (SacTrace *)malloc(nsta*sizeof(SacTrace));
	data = (double **)malloc(nsta*sizeof(double *));
	dfshift = (int *)malloc(nsta*sizeof(int));
	abshift = (int *)malloc(nsta*sizeof(int));
	kappa = (int *)malloc(nsta*sizeof(int));
	tau = (double *)malloc(nsta*sizeof(double));
	tdf = (double *)malloc(nsta*sizeof(double));
	tab = (double *)malloc(nsta*sizeof(double));
	adf = (double *)malloc(nsta*sizeof(double));
	aab = (double *)malloc(nsta*sizeof(double));
	amp = (double *)malloc(nsta*sizeof(double));
	namp = (double *)malloc(nsta*sizeof(double));
	tsta = (double *)malloc(nsta*sizeof(double));
	s = (double *)malloc(npts*sizeof(double));
	ns = (double *)malloc(npts*sizeof(double));
	dummy = (double *)malloc(npts*sizeof(double));
	obc = (double **)malloc(nsta*sizeof(double *));
	odf = (double **)malloc(nsta*sizeof(double *));
	oab = (double **)malloc(nsta*sizeof(double *));
	nbc = (double **)malloc(nsta*sizeof(double *));
	ndf = (double **)malloc(nsta*sizeof(double *));
	nab = (double **)malloc(nsta*sizeof(double *));
	derr = (double *)malloc(nsta*sizeof(double));
	ntsta = (double *)malloc(nsta*sizeof(double));
	tstastd = (double *)malloc(nsta*sizeof(double));
	ampstd = (double *)malloc(nsta*sizeof(double));
	adfstd = (double *)malloc(nsta*sizeof(double));
	aabstd = (double *)malloc(nsta*sizeof(double));
	taustd = (double *)malloc(nsta*sizeof(double));
	tdfstd = (double *)malloc(nsta*sizeof(double));
	tabstd = (double *)malloc(nsta*sizeof(double));
	tstastd = (double *)malloc(nsta*sizeof(double));
	atsta = (double **)malloc(nsta*sizeof(double *));
	aamp = (double **)malloc(nsta*sizeof(double *));
	aadf = (double **)malloc(nsta*sizeof(double *));
	aaab = (double **)malloc(nsta*sizeof(double *));
	atau = (double **)malloc(nsta*sizeof(double *));
	atdf = (double **)malloc(nsta*sizeof(double *));
	atab = (double **)malloc(nsta*sizeof(double *));
	ntau = (double *)malloc(nsta*sizeof(double));
	qq = (double **)malloc(nsta*sizeof(double *));
	q = (double **)malloc(nsta*sizeof(double *));
	
	ofp = (FILE **)malloc(nsta*sizeof(FILE *));

	for (i=0;i<nsta;i++)
	{
		atsta[i] = (double *)malloc(nrun*sizeof(double));
		aamp[i] = (double *)malloc(nrun*sizeof(double));
		aadf[i] = (double *)malloc(nrun*sizeof(double));
		aaab[i] = (double *)malloc(nrun*sizeof(double));
		atau[i] = (double *)malloc(nrun*sizeof(double));
		atdf[i] = (double *)malloc(nrun*sizeof(double));
		atab[i] = (double *)malloc(nrun*sizeof(double));
		
	}

	for (i=0;i<nsta;i++){
		st[i] = read_sac_tr(dt.fname[i]);
		if (i==0)
			delta = st[i].hd.delta;
		data[i] = (double *)malloc(npts*sizeof(double));
		cut_data(st[i],'1',20,900,data[i]);
		normalize(data[i],npts);
		dfshift[i] = (int)((dt.t_df[i] - dt.t_bc[i])/delta);
		abshift[i] = (int)((dt.t_ab[i] - dt.t_bc[i])/delta);
		obc[i] = (double *)malloc(npts*sizeof(double));
		odf[i] = (double *)malloc(npts*sizeof(double));
		oab[i] = (double *)malloc(npts*sizeof(double));
		nbc[i] = (double *)malloc(npts*sizeof(double));
		ndf[i] = (double *)malloc(npts*sizeof(double));
		nab[i] = (double *)malloc(npts*sizeof(double));
		qq[i] = (double *)malloc(npts*sizeof(double));
		q[i] = (double *)malloc(npts*sizeof(double));
	}
	/*Set length and maximum perturbation of reference wave*/ 	
	//smax = 0;
	/*for (i=0;i<nsta;i++)
	{
		for (j=0;j<npts;j++)
			if (fabs(data[i][j]) > smax)
			{
				smax = fabs(data[i][j]);
				ismax = i;
			}
	}
	*/
	ds = 0.01;
	sb = (int)((stime-tbs)/delta);
	se = (int)((stime-tbs+rtime)/delta);
	/*-----------------------------------------------------*/
	
	/*Define SA parameter*/
	e1 = 0;
	for (i=0;i<nsta;i++)
	{	
		for (j=0;j<npts;j++)
			e1 += fabs(data[i][j]);
	}
	gamma = 0.99;
	//test_fp = fopen("tmp.txt","w+");
	t1 = dclock();
	/*------------------------------Begin  Run Loop of SA---------------------------*/
	for (i=0;i<nrun;i++)
	{
		if ((efp=fopen(energy,"w+"))==NULL)
			printf("Fail to write result to file %s",energy);

		srandom(time(NULL));
		memset(s,0,npts*sizeof(double));
		memset(ns,0,npts*sizeof(double));

		for (j=0;j<nsta;j++)
		{
			tau[j] = 0;tdf[j] = 0;tab[j] = 0;
			adf[j] = 1;aab[j] = 1;amp[j] = 1;tsta[j] = 0.1;
		}
		
		for (j=0;j<nloop;j++)
		{
			temp = pow(gamma,j+1)*e1;
			printf("run:%d loop:%d\n",i,j);
			
			for (k=0;k<nret;k++)
			{
				e2 = 0;
				#pragma omp parallel for default(shared) private(l,m)
				for (l=0;l<nsta;l++)
				{
					mkwave(npts,0,delta,s,amp[l],tau[l],obc[l]);
					mkdf(npts,dfshift[l],delta,s,adf[l],tdf[l],tsta[l],odf[l]);
					mkab(npts,abshift[l],delta,s,aab[l],tab[l],oab[l]);
					for (m=0;m<npts;m++)
						qq[l][m] = obc[l][m] + odf[l][m] + oab[l][m];
				}

				for (l=sb;l<se;l++)
				{
					u = l-sb;
					for (m=0;m<npts;m++)	
					{
						if (m == l)
						{
							alpha=get_random();
							if (alpha < 0.5)
								ns[m] = s[m] - ds;
							else
								ns[m] = s[m] + ds;
						}
						else
							ns[m] = s[m];
					}
					der = 0;
					#pragma omp parallel for default(shared) private(m,n) reduction(+:der)
					for (m=0;m<nsta;m++)
					{
						mkwave(npts,0,delta,ns,amp[m],tau[m],nbc[m]);
						mkdf(npts,dfshift[m],delta,ns,adf[m],tdf[m],tsta[m],ndf[m]);
						mkab(npts,abshift[m],delta,ns,aab[m],tab[m],nab[m]);

						for (n=0;n<npts;n++)
						{
							q[m][n] = nbc[m][n] + ndf[m][n] + nab[m][n];
							der += fabs(data[m][n]-q[m][n]) - fabs(data[m][n]-qq[m][n]);
						}
					}

					if (der < 0)
						s[l] = ns[l];
					//else
					//	nns[u] = s[l];
				}

				//for (l=0;l<sb;l++)
				//	s[l] = 0;
				//for (l=sb;l<se;l++)
				//	s[l] = nns[l-sb];
				//for (l=se;l<npts;l++)
				//	s[l] = 0;
				//printf("node 1\n");
			    /*--------------Perturbate the amplitude parameters-------------*/
			#pragma omp parallel default(shared) private(l,m,p,alpha,tid)
			{
				#pragma omp for
				for (l=0;l<nsta;l++)
				{
					/*------------------------ BC -----------------------------*/
					mkwave(npts,0,delta,s,amp[l],tau[l],obc[l]);
					mkdf(npts,dfshift[l],delta,s,adf[l],tdf[l],tsta[l],odf[l]);
					mkab(npts,abshift[l],delta,s,aab[l],tab[l],oab[l]);
					
					for (m=0;m<npts;m++)
						qq[l][m] = obc[l][m] + odf[l][m] + oab[l][m];
					
					//if (l != ismax)
					//{
						namp[l] = get_random();
						mkwave(npts,0,delta,s,namp[l],tau[l],nbc[l]);
						
						derr[l] = 0;

						for (m=0;m<npts;m++)
						{
							q[l][m] = nbc[l][m] + odf[l][m] + oab[l][m];
							derr[l] += fabs(data[l][m]-q[l][m]) - fabs(data[l][m]-qq[l][m]);
						}

						if (derr[l] < 0)
						{
							amp[l] = namp[l];
							for (m=0;m<npts;m++)
								obc[l][m] = nbc[l][m];
						}
					//}
					/*--------------------------- DF -------------------------------*/ 	
					namp[l] = get_random()*nadf;
					mkdf(npts,dfshift[l],delta,s,namp[l],tdf[l],tsta[l],ndf[l]);

					derr[l] = 0;
					for (m=0;m<npts;m++)
					{
						qq[l][m] = obc[l][m] + odf[l][m] + oab[l][m];
						q[l][m] = obc[l][m] + ndf[l][m] + oab[l][m];
						derr[l] += fabs(data[l][m] - q[l][m]) - fabs(data[l][m] - qq[l][m]);
					}

					if (derr[l] < 0)
					{
						adf[l] = namp[l];
						for (m=0;m<npts;m++)
							odf[l][m] = ndf[l][m];
					}
					/*---------------------------- AB ------------------------------*/
					namp[l] = get_random()*naab;
					mkab(npts,abshift[l],delta,s,namp[l],tab[l],nab[l]);

					derr[l] = 0;
					for (m=0;m<npts;m++)
					{
						qq[l][m] = obc[l][m] + odf[l][m] + oab[l][m];
						q[l][m] = obc[l][m] + odf[l][m] + nab[l][m];
						derr[l] +=  fabs(data[l][m] - q[l][m]) - fabs(data[l][m] - qq[l][m]);
					}

					if (derr[l] <0)
					{
						aab[l] = namp[l];
						for (m=0;m<npts;m++)
							oab[l][m] = nab[l][m];
					}
				}
				//printf("node 2\n");
				/*------------------- Perturbate tstar----------------------*/
				#pragma omp for
				for (l=0;l<nsta;l++)
				{
					mkwave(npts,0,delta,s,amp[l],tau[l],obc[l]);
					mkdf(npts,dfshift[l],delta,s,adf[l],tdf[l],tsta[l],odf[l]);
					mkab(npts,abshift[l],delta,s,aab[l],tab[l],oab[l]);
					
					for (m=0;m<npts;m++)
						qq[l][m] = obc[l][m] + odf[l][m] + oab[l][m];
					
					ntsta[l] = tsmax*get_random();
					mkdf(npts,dfshift[l],delta,s,adf[l],tdf[l],ntsta[l],ndf[l]);

					derr[l] = 0;
					for (m=0;m<npts;m++)
					{
						q[l][m] = obc[l][m] + ndf[l][m] + oab[l][m];
						derr[l] += fabs(data[l][m] - q[l][m]) - fabs(data[l][m] - qq[l][m]);
					}

					if (derr[l] < 0)
						tsta[l] = ntsta[l];
					else if (derr[l] > 0)
					{
						p = exp(-1*derr[l]/temp);
						alpha = get_random();
						if (alpha < p)
							tsta[l] = ntsta[l];
					}
				}
			}
				//printf("node 3\n");
				/*-----------------Perturbate time delay--------------------*/
				/*----------------------   BC ------------------------------*/
				#pragma omp parallel for default(shared) private(l,m,rand,p,alpha)
				for (l=0;l<nsta;l++)
				{
					rand = get_random();
					kappa[l] = (int)(2*(rand - 0.5)/delta);
					ntau[l] = kappa[l]*delta;

					mkwave(npts,0,delta,s,amp[l],tau[l],obc[l]);
					mkdf(npts,dfshift[l],delta,s,adf[l],tdf[l],tsta[l],odf[l]);
					mkab(npts,abshift[l],delta,s,aab[l],tab[l],oab[l]);

					for (m=0;m<npts;m++)
						qq[l][m] =  obc[l][m] + odf[l][m] + oab[l][m];
					
					mkwave(npts,0,delta,s,amp[l],ntau[l],nbc[l]);
					
					derr[l] = 0;
					for (m=0;m<npts;m++)
					{
						q[l][m] = nbc[l][m] + odf[l][m] + oab[l][m];
						derr[l] += fabs(data[l][m] - q[l][m]) - fabs(data[l][m] - qq[l][m]);
					}

					if (derr[l] < 0)
					{
						tau[l] = ntau[l];
						for (m=0;m<npts;m++)
							obc[l][m] = nbc[l][m];
					}
					else if (derr[l] > 0)
					{
						p =  exp(-1*derr[l]/temp);
						alpha = get_random();
						if (alpha < p)
						{
							tau[l] = ntau[l];
							for (m=0;m<npts;m++)
								obc[l][m] = nbc[l][m];
						}
					}
				}

				/*------------Impose tau equals zero----------------------*/
				ktau = 0;
				for (l=0;l<nsta;l++)
					ktau += (int)(tau[l]/delta);
				kave = (int)((double)ktau/(double)nsta);
				
				for (l=0;l<nsta;l++)
					tau[l] -= kave*delta;
	
				N = kave;

				for (l=0;l<npts;l++)
					dummy[l] = s[l];

				if (N > 0)
				{
					for (l=0;l<N;l++)
						s[l] = 0;
					for (l=N;l<npts;l++)
						s[l] = dummy[l-N];
				}
				else if (N < 0)
				{
					for (l=0;l<npts+N;l++)
						s[l] = dummy[l-N];
					for (l=npts+N;l<npts;l++)
						s[l] = 0;
				}

				for (l=0;l<sb;l++)
					s[l] = 0;
				for (l=se;l<npts;l++)
					s[l] = 0;		
				/*----------------------  DF -----------------------------*/
				#pragma omp parallel for default(shared) private(l,m,mnam,rand,p,alpha,tid) \
				reduction(+:e2)
				for (l=0;l<nsta;l++)
				{

					//tid = omp_get_thread_num();
					//if (tid == 2)
					//	printf("Thread %d does something different\n",tid);
					mkwave(npts,0,delta,s,amp[l],tau[l],obc[l]);
					mkdf(npts,dfshift[l],delta,s,adf[l],tdf[l],tsta[l],odf[l]);
					mkab(npts,abshift[l],delta,s,aab[l],tab[l],oab[l]);

					for (m=0;m<npts;m++)
						qq[l][m] =  obc[l][m] + odf[l][m] + oab[l][m];

					mnam = 0;
					do
					{
						rand = get_random();
						kappa[l] = (int)(2*(rand - 0.5)/delta);
						ntau[l] =  kappa[l]*delta;
						mnam += 1;
					}while(dfshift[l]+ kappa[l] > tau[l]/delta && mnam < 500);

					mkdf(npts,dfshift[l],delta,s,adf[l],ntau[l],tsta[l],ndf[l]);
					
					derr[l] = 0;
					for (m=0;m<npts;m++)
					{
						q[l][m] = obc[l][m] + ndf[l][m] + oab[l][m];
						derr[l] += fabs(data[l][m] - q[l][m]) - fabs(data[l][m] - qq[l][m]);
					}

					if (derr[l] < 0)
					{
						tdf[l] = ntau[l];
						for (m=0;m<npts;m++)
							odf[l][m] = ndf[l][m];
					}
					else if (derr[l] > 0)
					{
						p =  exp(-1*derr[l]/temp);
						alpha = get_random();
						if (alpha < p)
						{
							tdf[l] = ntau[l];
							for (m=0;m<npts;m++)
								odf[l][m] = ndf[l][m];
						}
					}
					/*----------------------- AB ---------------------------*/
					mnam = 0;
					do
					{
						rand = get_random();
						kappa[l] = (int)(2*(rand - 0.5)/delta);
						ntau[l] =  kappa[l]*delta;
						mnam += 1;
					}while(abshift[l]+kappa[l] < tau[l]/delta && mnam < 500);

					mkab(npts,abshift[l],delta,s,aab[l],ntau[l],nab[l]);
					derr[l] = 0;
					for (m=0;m<npts;m++)
					{	
						qq[l][m] =  obc[l][m] + odf[l][m] + oab[l][m];
						q[l][m] = obc[l][m] + odf[l][m] + nab[l][m];
						derr[l] += fabs(data[l][m] - q[l][m]) - fabs(data[l][m] - qq[l][m]);
					}
					
					if (derr[l] < 0)
					{
						tab[l] = ntau[l];
						for (m=0;m<npts;m++)
							oab[l][m] = nab[l][m];
					}
					else if (derr[l] > 0)
					{
						p =  exp(-1*derr[l]/temp);
						alpha = get_random();
						if (alpha < p)
						{
							tab[l] = ntau[l];
							for (m=0;m<npts;m++)
								oab[l][m] = nab[l][m];
						}
					}
				
				/*--------------------------------------------------------------*/
				/*-----------------------Calculate Energy ----------------------*/
					mkwave(npts,0,delta,s,amp[l],tau[l],obc[l]);
					mkdf(npts,dfshift[l],delta,s,adf[l],tdf[l],tsta[l],odf[l]);
					mkab(npts,abshift[l],delta,s,aab[l],tab[l],oab[l]);
					for (m=0;m<npts;m++)
					{
						qq[l][m] = obc[l][m] + odf[l][m] + oab[l][m];
						e2 += fabs(data[l][m]-qq[l][m]);
					}
				}
				if (k==nret-1)
					fprintf(efp,"%d %f\n",j,e2/e1);
			}
		}
		/*--------------------------- End Temperature Loop ----------------------*/
		for (j=0;j<nsta;j++)
		{
			atsta[j][i] = tsta[j];
			aamp[j][i] = amp[j];
			aadf[j][i] = adf[j];
			aaab[j][i] = aab[j];
			atau[j][i] = tau[j];
			atdf[j][i] = tdf[j];
			atab[j][i] = tab[j];
		}
		fclose(efp);
	/*-------------------------------End N Run Loop------------------------------*/
	}
	t2 = dclock();
	average(atsta,tsta,tstastd,nrun,nsta);
	average(aamp,amp,ampstd,nrun,nsta);
	average(aadf,adf,adfstd,nrun,nsta);
	average(aaab,aab,aabstd,nrun,nsta);
	average(atau,tau,taustd,nrun,nsta);
	average(atdf,tdf,tdfstd,nrun,nsta);
	average(atab,tab,tabstd,nrun,nsta);
	/*------------------------------Output results-------------------------------*/
	if ((rfp=fopen("result.txt","w+"))==NULL)
		printf("Fail to write result to file %s","result.txt");
	for (i=0;i<nsta;i++)
		fprintf(rfp,"%8.3f %.3f %.3f %.3f %5.3f %5.3f %5.3f %5.3f\n",\
				dt.dis[i],tdf[i],tau[i],tab[i],tsta[i],tstastd[i],adf[i],adfstd[i]);
	if ((sfp=fopen("refwave.txt","w+"))==NULL)
		printf("Fail to write result to file %s","refwave.txt");
	for (i=0;i<npts;i++)
		fprintf(sfp,"%6.3f %f\n",delta*i,s[i]);
	fclose(rfp); fclose(sfp);

	for (i=0;i<nsta;i++)
	{
		mkwave(npts,0,delta,s,amp[i],tau[i],obc[i]);
		mkdf(npts,dfshift[i],delta,s,adf[i],tdf[i],tsta[i],odf[i]);
		mkab(npts,abshift[i],delta,s,aab[i],tab[i],oab[i]);

		memset(buf,0,sizeof(buf));
		memcpy(buf,st[i].hd.kstnm,8);
		trim(buf);

		if((ofp[i]=fopen(buf,"w+"))==NULL)
		{
			printf("Fail to write data to file %s.\n",buf);
			continue;
		}
		for (j=0;j<npts;j++)
		{
			qq[i][j] = obc[i][j] + odf[i][j] + oab[i][j];
			fprintf(ofp[i],"%6.3f %f %f\n",delta*j,data[i][j],qq[i][j]);
		}
		
		fclose(ofp[i]);
	}
	printf("elapsed time: %f\n",t2 - t1);
	return 0;
}

void average(double **m,double *avg,double *std,int nrun,int nsta)
{
	int i,j;
	double sum;

	for (i=0;i<nsta;i++)
	{
		sum = 0;
		for (j=0;j<nrun;j++)
			sum += m[i][j];
		avg[i] = sum/nrun;
		sum = 0;
		for (j=0;j<nrun;j++)
			sum += (m[i][j] - avg[i])*(m[i][j] - avg[i]);
		std[i] = sqrt(sum/nrun);
	}
}
