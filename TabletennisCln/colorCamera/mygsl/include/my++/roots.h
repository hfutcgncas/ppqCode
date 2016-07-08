#ifndef ROOTS_H_INCLUDED
#define ROOTS_H_INCLUDED


typedef gsl_multiroot_function RootFunction;

RootFunction torootfunction(int (*f)(const Vector *X,void *P,Vector *F),unsigned long int M);

Matrix *gslroots(int (*f)(const Vector *X,void *P,Vector *F),Vector *x,unsigned short int m);

//#ifndef __cplusplus

Matrix *roots(Vector *(*f)(Vector *X),Vector *low,Vector *up,unsigned long int pop,unsigned long int iter,unsigned short int m);

Matrix *gslrootsC(Vector *(*f)(const Vector *xx),Vector *x,unsigned short int m);


//#endif




#endif // ROOTS_H_INCLUDED
