#ifndef ODESOLVE_H_INCLUDED
#define ODESOLVE_H_INCLUDED

void rk1(double (*f)(double ,double ),double a[],double b[],unsigned long int N);

void euler1(double (*f)(double ,double ),double a[],double b[],unsigned long int n);

void rkt2(double t,double h,double y[],unsigned long int n,void (*f)(double xx[],double yy[]));

void rks1(void (*f)(double xx[],double yy[]),unsigned long int n,unsigned long int dim,double t[],double *y);



#endif // ODESOLVE_H_INCLUDED
