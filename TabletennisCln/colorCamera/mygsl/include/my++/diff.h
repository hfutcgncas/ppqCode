#ifndef DIFF_H_INCLUDED
#define DIFF_H_INCLUDED

double diff(double (*f)(double x,void *params),double x,unsigned short int method);


Vector *grad(double (*f[])(double x,void *params),Vector *x,unsigned short int method);


#endif // DIFF_H_INCLUDED
