#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED



typedef gsl_matrix Matrix;

#define newMatrix(x,y)          gsl_matrix_calloc(x,y)
#define freeMatrix(x)           gsl_matrix_free(x)
#define setMatrix(x,y,z,w)      gsl_matrix_set(x,y,z,w)
#define getMatrix(x,y,z)        gsl_matrix_get(x,y,z)
#define setallMatrix(x,y)       gsl_matrix_set_all(x,y)
#define setrowMatrix(x,y,z)     gsl_matrix_set_row(x,y,z)
#define setcolMatrix(x,y,z)     gsl_matrix_set_col(x,y,z)
#define minMatrix(x)            gsl_matrix_min(x)
#define maxMatrix(x)            gsl_matrix_max(x)
#define minindexMatrix(x,y,z)   gsl_matrix_min_index(x,y,z)
#define maxindexMatrix(x,y,z)   gsl_matrix_max_index(x,y,z)
#define copyMatrix(x,y)         gsl_matrix_memcpy(x,y)
#define scaleMatrix(x,y)        gsl_matrix_scale(x,y)

Vector *getrowMatrix(Matrix *x,unsigned long int y);

Vector *getcolMatrix(Matrix *x,unsigned long int y);

void printMatrix(Matrix *m);

Matrix *transposeMatrix(Matrix *x);

Matrix *algMatrix(Matrix *x,Matrix *y,unsigned short int z);

Matrix *MdotM(Matrix *x,Matrix *y);

Vector *MdotV(Matrix *x,Vector *y);

Matrix *iMatrix(unsigned long int x,unsigned long int y);

Matrix *invMatrix(Matrix *x);

double det(Matrix *x);

Matrix *toMatrix(double x[],unsigned long int y,unsigned long int z);

void svd(Matrix *x,Matrix *u,Matrix *s,Matrix *v);

void nonsyeigen(Matrix *x,gsl_vector_complex *y,gsl_matrix_complex *z);

void syeigen(Matrix *x,Vector *y,Matrix *z);

Vector *linsolveVector(Matrix *x,Vector *y);

Matrix *linsolveMatrix(Matrix *x,Matrix *y);

Vector *lsqrlinsolve(Matrix *x,Vector *y);

Vector *lssvlinsolve(Matrix *x,Vector *y);

Vector *lslinsolve(Matrix *x,Vector *y,unsigned short int z);

Vector *hhlinsolveVector(Matrix *x,Vector *y);

Matrix *pinvMatrix(Matrix *x);

Vector *lspinvlinsolveVector(Matrix *x,Vector *y);

Matrix *lspinvlinsolveMatrix(Matrix *x,Matrix *y);

Matrix *arraytoMatrix(double *x,unsigned long int y,unsigned long int z);

Matrix *lu(Matrix *x);

Matrix *chol(Matrix *x);

Matrix *randomMatrix(unsigned long int m,unsigned long int n);

Matrix *randomrealMatrix(double low,double up,unsigned long int m,unsigned long int n);

Matrix *randomrealbdMatrix(Vector *low,Vector *up,unsigned long int m);

double sumMatrix(Matrix *x);

Matrix *linspaceMatrix(Vector *low,Vector *up,unsigned long int y);

void writeMatrix(Matrix *x,FILE *f);

void outputMatrix(Matrix *x,char *fileid);

Matrix *mapMatrix(double (*f)(double X),Matrix *x);

Matrix *xrandomMatrix(unsigned long int m,unsigned long int n);

Matrix *xrandomrealMatrix(double low,double up,unsigned long int m,unsigned long int n);

Matrix *xrandomrealbdMatrix(Vector *low,Vector *up,unsigned long int m);

Matrix *mapatMatrix(double (*f)(double X),Matrix *x,unsigned long int y,unsigned long int z);

Matrix *zipVector(Vector *x,Vector *y);

int rank(Matrix *M);



#endif // MATRIX_H_INCLUDED
