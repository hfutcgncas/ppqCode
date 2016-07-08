#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED



typedef gsl_vector Vector;

#define newVector(x)        gsl_vector_calloc(x)
#define freeVector(x)       gsl_vector_free(x)
#define getVector(x,y)      gsl_vector_get(x,y)
#define setVector(x,y,z)    gsl_vector_set(x,y,z)
#define setallVector(x,y)   gsl_vector_set_all(x,y)
#define norm(x)             gsl_blas_dnrm2(x)
#define minVector(x)        gsl_vector_min(x)
#define maxVector(x)        gsl_vector_max(x)
#define minindexVector(x)   gsl_vector_min_index(x)
#define maxindexVector(x)   gsl_vector_max_index(x)
#define copyVector(x,y)     gsl_vector_memcpy(x,y)
#define scaleVector(x,y)    gsl_vector_scale(x,y)


void printVector(Vector *v);

Vector *toVector(double x[],unsigned long int dim);

double dot(Vector *x,Vector *y);

Vector *algVector(Vector *x,Vector *y,unsigned short int z);

Vector *randomVector(unsigned long int m);

Vector *randomrealVector(double low,double up,unsigned long int m);

double sumVector(Vector *x);

Vector *linspaceVector(double low,double up,unsigned long int y);

void writeVector(Vector *x,FILE *f);

void outputVector(Vector *x,char *fileid);

Vector *mapVector(double (*f)(double X),Vector *x);

Vector *reverseVector(Vector *x);

Vector *seq(double low,double up,double step);

Vector *xrandomVector(unsigned long int m);

Vector *xrandomrealVector(double low,double up,unsigned long int m);

gsl_matrix *VtoM(Vector *x);

Vector *mapatVector(double (*f)(double X),Vector *x,unsigned long int y);




#endif // VECTOR_H_INCLUDED
