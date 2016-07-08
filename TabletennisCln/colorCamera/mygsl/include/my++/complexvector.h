#ifndef COMPLEXVECTOR_H_INCLUDED
#define COMPLEXVECTOR_H_INCLUDED


typedef gsl_vector_complex ComplexVector;

#define newComplexVector(x)         gsl_vector_complex_calloc(x)
#define freeComplexVector(x)        gsl_vector_complex_free(x)
#define copyComplexVector(x,y)      gsl_vector_complex_memcpy(x,y)
#define getComplexVector(x,y)       gsl_vector_complex_get(x,y)
#define setComplexVector(x,y,z)     gsl_vector_complex_set(x,y,z)
#define setallComplexVector(x,y)    gsl_vector_complex_set_all(x,y)
#define normComplex(x)              gsl_blas_dznrm2(x)

void printComplexVector(ComplexVector *x);

ComplexVector *algComplexVector(ComplexVector *x,ComplexVector *y,unsigned short int z);

ComplexVector *toComplexVector(gsl_vector *x,gsl_vector *y);

Complex dotComplex(ComplexVector *x,ComplexVector *y);

Complex dotconjComplex(ComplexVector *x,ComplexVector *y);

ComplexVector *conjComplexVector(ComplexVector *x);





#endif // COMPLEXVECTOR_H_INCLUDED
