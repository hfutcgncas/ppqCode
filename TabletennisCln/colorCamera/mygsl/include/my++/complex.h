#ifndef COMPLEX_H_INCLUDED
#define COMPLEX_H_INCLUDED


typedef gsl_complex Complex;

#define conjComplex(x)              gsl_complex_conjugate(x)
#define invComplex(x)               gsl_complex_inverse(x)
#define negComplex(x)               gsl_complex_negative(x)
#define sqrtComplex(x)              gsl_complex_sqrt(x)
#define powComplex(x)               gsl_complex_pow(x,y)
#define expComplex(x)               gsl_complex_exp(x)
#define logComplex(x)               gsl_complex_log(x)
#define log10Complex(x)             gsl_complex_log10(x)
#define sinComplex(x)               gsl_complex_sin(x)
#define cosComplex(x)               gsl_complex_cos(x)
#define tanComplex(x)               gsl_complex_tan(x)
#define secComplex(x)               gsl_complex_sec(x)
#define cscComplex(x)               gsl_complex_csc(x)
#define cotComplex(x)               gsl_complex_cot(x)
#define arcsinComplex(x)            gsl_complex_arcsin(x)
#define arccosComplex(x)            gsl_complex_arccos(x)
#define arctanComplex(x)            gsl_complex_arctan(x)
#define arcsecComplex(x)            gsl_complex_arcsec(x)
#define arccscComplex(x)            gsl_complex_arccsc(x)
#define arccotComplex(x)            gsl_complex_arccot(x)
#define sinhComplex(x)               gsl_complex_sinh(x)
#define coshComplex(x)               gsl_complex_cosh(x)
#define tanhComplex(x)               gsl_complex_tanh(x)
#define sechComplex(x)               gsl_complex_sech(x)
#define cschComplex(x)               gsl_complex_csch(x)
#define cothComplex(x)               gsl_complex_coth(x)

#define arcsinhComplex(x)            gsl_complex_arcsinh(x)
#define arccoshComplex(x)            gsl_complex_arccosh(x)
#define arctanhComplex(x)            gsl_complex_arctanh(x)
#define arcsechComplex(x)            gsl_complex_arcsech(x)
#define arccschComplex(x)            gsl_complex_arccsch(x)
#define arccothComplex(x)            gsl_complex_arccoth(x)


Complex newComplex(void);

double getrealComplex(Complex x);

double getimagComplex(Complex x);

void printComplex(Complex x);

void setComplex(Complex *x,double y,double z);

void setrealComplex(Complex *x,double y);

void setimagComplex(Complex *x,double y);

double argComplex(Complex x);

double absComplex(Complex x);

Complex copyComplex(Complex x);

Complex algComplex(Complex x,Complex y,unsigned short int z);

Complex algrealComplex(Complex x,double y,unsigned short int z);

Complex algimagComplex(Complex x,double y,unsigned short int z);




#endif // COMPLEX_H_INCLUDED
