#ifndef INTEGRATION_H_INCLUDED
#define INTEGRATION_H_INCLUDED

typedef gsl_function Function;
typedef gsl_monte_function MCFunction;

Function tofunction(double (*func)(double x,void *params));

MCFunction tomontefunction(double (*func)(double *x,size_t M,void *p),unsigned long int m);

double integration(double (*func)(double x,void *params),double low,double up);

double iiintegration(double (*func)(double x,void *params));

double niintegration(double (*func)(double x,void *params),double low);

double inintegration(double (*func)(double x,void *params),double up);

double glintegration(double (*func)(double x,void *params),double low,double up);

double sintegration(double (*func)(double x,void *params),double low,double up);

double spintegration(double (*func)(double x,void * params),double sp[],unsigned long int m);

double mcintegration1(double (*func)(double *x,size_t M,void *p),unsigned long int m,double low[],double up[],unsigned long int iter);

double mcintegration2(double (*func)(double *x,size_t M,void *p),unsigned long int m,double low[],double up[],unsigned long int iter);

double mcintegration3(double (*func)(double *x,size_t M,void *p),unsigned long int m,double low[],double up[],unsigned long int iter);

double mcintegration(double (*func)(double *x,size_t M,void *p),unsigned long int m,double low[],double up[],unsigned long int iter,unsigned short int n);

double gaussintegration(int n,int js[],void (*ss[])(double x[],double y[]),double (*f)(double x[]));



#endif // INTEGRATION_H_INCLUDED
