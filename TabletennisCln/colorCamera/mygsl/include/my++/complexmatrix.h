#ifndef COMPLEXMATRIX_H_INCLUDED
#define COMPLEXMATRIX_H_INCLUDED


typedef gsl_matrix_complex ComplexMatrix;

#define newComplexMatrix(x,y)       gsl_matrix_complex_calloc(x,y)
#define freeComplexMatrix(x)        gsl_matrix_complex_free(x)
#define copyComplexMatrix(x,y)      gsl_matrix_complex_memcpy(x,y)
#define getComplexMatrix(x,y,z)     gsl_matrix_complex_get(x,y,z)
#define setComplexMatrix(x,y,z,w)   gsl_matrix_complex_set(x,y,z,w)
#define setallComplexMatrix(x,y)    gsl_matrix_complex_set_all(x,y)
#define setrowComplexMatrix(x,y,z)  gsl_matrix_complex_set_row(x,y,z)
#define setcolComplexMatrix(x,y,z)  gsl_matrix_complex_set_col(x,y,z)

ComplexMatrix *transposeComplexMatrix(ComplexMatrix *x);

void printComplexMatrix(ComplexMatrix *x);

ComplexVector *getrowComplexMatrix(ComplexMatrix *x,unsigned long int y);

ComplexVector *getcolComplexMatrix(ComplexMatrix *x,unsigned long int y);

ComplexMatrix *conjComplexMatrix(ComplexMatrix *x);

ComplexMatrix *transposeconjComplexMatrix(ComplexMatrix *x);

ComplexMatrix *algComplexMatrix(ComplexMatrix *x,ComplexMatrix *y,unsigned short int z);

ComplexMatrix *toComplexMatrix(gsl_matrix *x,gsl_matrix *y);

ComplexMatrix *invComplexMatrix(ComplexMatrix *x);

ComplexVector *MdotVComplex(ComplexMatrix *x,ComplexVector *y);

ComplexMatrix *MdotMComplex(ComplexMatrix *x,ComplexMatrix *y);

ComplexVector *linsolveComplexVector(ComplexMatrix *x,ComplexVector *y);

ComplexMatrix *pinvComplexMatrix(ComplexMatrix *x);

Complex detComplex(ComplexMatrix *x);

ComplexVector *lspinvlinsolveComplexVector(ComplexMatrix *x,ComplexVector *y);

ComplexMatrix *lspinvlinsolveComplexMatrix(ComplexMatrix *x,ComplexMatrix *y);

ComplexMatrix *iComplexMatrix(unsigned long int x,unsigned long int y);

ComplexMatrix *luComplex(ComplexMatrix *x);

ComplexMatrix *cholComplex(ComplexMatrix *x);




#endif // COMPLEXMATRIX_H_INCLUDED
