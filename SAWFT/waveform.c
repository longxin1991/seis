#include "transform.h"
#include "attenuation.h"

void mkwave(int npts,int nshift,double dt,double *s,double amp,double tau,double *w)
{
	int n,i;
	n = nshift + (int)(tau/dt);
	if (n > 0)
	{	
		for (i=0;i<n;i++)
			w[i] = 0;
		for (i=n;i<npts;i++)
			w[i] = amp*s[i-n];
	}
	else if (n < 0)
	{
		for (i=0;i<npts+n;i++)
			w[i] = amp*s[i-n];
		for (i=npts+n;i<npts;i++)
			w[i] = 0;
	}
	else
	{
		for (i=0;i<npts;i++)
			w[i] = amp*s[i];
	}
}

void mkdf(int npts,int nshift,double dt,double *s,double amp,double tau,double tq,double *w)
{
	double df,s1[npts],d[2*npts];
	int i;
	double complex qs[npts],tmp;
	gsl_fft_complex_wavetable *wavetable;
	gsl_fft_complex_workspace *workspace;

	//fftw_complex in1[npts],in2[npts],out1[npts],out2[npts];
	//fftw_plan fplan,iplan;	
	
	//fplan = fftw_plan_dft_1d(npts,in1,out1,-1,FFTW_ESTIMATE);
	//iplan = fftw_plan_dft_1d(npts,in2,out2,1,FFTW_ESTIMATE);
	
	for (i=0;i<npts;i++)
	{
		//in1[i][0]=s[i];
		//in1[i][1]=0;
		REAL(d,i) = s[i];
		IMAG(d,i) = 0;
	}
	wavetable = gsl_fft_complex_wavetable_alloc (npts);
	workspace = gsl_fft_complex_workspace_alloc (npts);
	gsl_fft_complex_forward(d,1,npts,wavetable,workspace);
	//fftw_execute(fplan);
	df = 1.0/(dt*(npts-1));
	qresp(tq,npts,df,qs);
	for (i=0;i<npts;i++)
	{
		//tmp = (out1[i][0] + I*out1[i][1])*qs[i];
		tmp = (REAL(d,i)+I*IMAG(d,i))*qs[i];
		REAL(d,i) = creal(tmp);
		IMAG(d,i) = cimag(tmp);
		//in2[i][0] = creal(tmp);
		//in2[i][1] = cimag(tmp);
	}
	gsl_fft_complex_inverse(d,1,npts,wavetable,workspace);
	//fftw_execute(iplan);
	for (i=0;i<npts;i++){
		//s1[i] = out2[i][0]/(npts-1);
		s1[i] = REAL(d,i);
	}
	gsl_fft_complex_wavetable_free (wavetable);
	gsl_fft_complex_workspace_free (workspace);
	mkwave(npts,nshift,dt,s1,amp,tau,w);
	
}

void mkab(int npts,int nshift,double dt,double *s,double amp,double tau,double *w)
{
	double complex in[npts],out[npts];
	int i;
	double s1[npts];

	for (i=0;i<npts;i++)
	{
		in[i] = s[i];
	}

	hilbert_t(npts,in,out);
	for (i=0;i<npts;i++)
		s1[i] = creal(out[i]);
	
	for (i=0;i<100;i++)
		s1[i] = 0;

	mkwave(npts,nshift,dt,s1,amp,tau,w);
}
