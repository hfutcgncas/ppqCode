#ifndef INTERP_H_INCLUDED
#define INTERP_H_INCLUDED

Vector *interp(Vector *x,Vector *y,unsigned long int z);

Matrix *spline1(double x[],double y[],unsigned long int n,unsigned long int dim);

Matrix *spline2(double x[],double y[],unsigned long int n,unsigned long int dim);

double evalspline1(double x[],double y[],unsigned long int n,double z);

double evalspline2(double x[],double y[],unsigned long int n,double z);



double lag1(unsigned long int n,double x[],double y[],double u);

double lag2(unsigned long int n,unsigned long int m,double x[],double y[],double *z,double u,double v);

double laq1(unsigned long int n,double x[],double y[],double u);

double laq2(unsigned long int n,unsigned long int m,double x[],double y[],double *z,double u,double v);




#endif // INTERP_H_INCLUDED
