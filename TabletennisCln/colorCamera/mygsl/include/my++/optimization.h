#ifndef OPTIMIZATION_H_INCLUDED
#define OPTIMIZATION_H_INCLUDED

typedef gsl_multimin_function OptFunction;

Matrix *fminsearchs(double (*func)(Vector *V),Vector *low,Vector *up,unsigned long int num,unsigned long int iter);

Matrix *xfminsearchs(double (*func)(Vector *V),Vector *low,Vector *up,unsigned long int num,unsigned long int iter);

Matrix *fmins(double (*func)(Vector *V),Vector *low,Vector *up,unsigned long int num,unsigned long int iter,unsigned short int m);

OptFunction tooptfunction(double (*func)(const Vector *V,void *p),unsigned long int M);

Matrix *nmsimplex(double (*fun)(const Vector *V,void *P),Vector *point,unsigned short int m);

Matrix *PSO(double (*func)(Vector *V),Vector *low,Vector *up,unsigned long int num,unsigned long int iter);

Matrix *Lnlfmins(double (*obj)(unsigned n, const double *x, double *grad, void *params),unsigned long int constrnum,double (*constr[])(unsigned n, const double *x, double *grad, void *params),Vector *inipop,Vector *low,Vector *up,unsigned short int method);

Matrix *Gnlfmins(double (*obj)(unsigned n, const double *x, double *grad, void *params),unsigned long int constrnum,double (*constr[])(unsigned n, const double *x, double *grad, void *params),Vector *inipop,Vector *low,Vector *up,unsigned short int method);

Matrix *nlfmins(double (*obj)(unsigned n, const double *x, double *grad, void *params),unsigned long int constrnum,double (*constr[])(unsigned n, const double *x, double *grad, void *params),unsigned long int pop,Vector *low,Vector *up,unsigned short int method);

Matrix *Gnmsimplex(double (*fun)(const Vector *V,void *P),unsigned long int pop,Vector *low,Vector *up,unsigned short int method);


#endif // OPTIMIZATION_H_INCLUDED
