#ifndef FIT_H_INCLUDED
#define FIT_H_INCLUDED





//#ifndef __cplusplus

Matrix *fit(double (*f)(Vector *X,Vector *P),Matrix *x,Vector *y,Vector *low,Vector *up,unsigned long int pop,unsigned long int iter);

Matrix *xfit(double (*f)(Vector *X,Vector *P),Matrix *x,Vector *y,Vector *low,Vector *up,unsigned long int pop,unsigned long int iter);

Matrix *PSOfit(double (*f)(Vector *X,Vector *P),Matrix *x,Vector *y,Vector *low,Vector *up,unsigned long int pop,unsigned long int iter);

Matrix *nmsimplexfit(double (*f)(Vector *x,const Vector *pp),Matrix *X,Vector *Y,Vector *point,unsigned short int m);

//#endif





#endif // FIT_H_INCLUDED
