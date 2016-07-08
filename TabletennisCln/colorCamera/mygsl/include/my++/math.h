#ifndef MATH_H_INCLUDED
#define MATH_H_INCLUDED



#define euler           ((double)M_EULER)
#define pi              3.1415962
#define PI              ((double)M_PI)
#define posinf          GSL_POSINF
#define neginf          GSL_NEGINF
#define nan             GSL_NAN
#define isreal(x)       gsl_finite(x)
#define sign(x)         GSL_SIGN(x)
#define isodd(x)        GSL_IS_ODD(x)
#define iseven(x)       GSL_IS_EVEN(x)
#define arccosh(x)        gsl_acosh(x)
#define arcsinh(x)        gsl_asinh(x)
#define arctanh(x)        gsl_atanh(x)
#define gslmax(x,y)        GSL_MAX(x,y)
#define gslmin(x,y)        GSL_MIN(x,y)
#define pow2(x)         gsl_pow_2(x)
#define pow3(x)         gsl_pow_3(x)
#define pow4(x)         gsl_pow_4(x)
#define pow5(x)         gsl_pow_5(x)
#define pow6(x)         gsl_pow_6(x)
#define pow7(x)         gsl_pow_7(x)
#define pow8(x)         gsl_pow_8(x)
#define pow9(x)         gsl_pow_9(x)

#endif // MATH_H_INCLUDED
