

#ifndef _MAT3_H
#  define _MAT3_H


void mat3_eye( double M[3][3] );
double mat3_det( double M[3][3] );
void mat3_add( double out[3][3], double A[3][3], double B[3][3] );
void mat3_sub( double out[3][3], double A[3][3], double B[3][3] );
void mat3_inv( double out[3][3], double in[3][3] );
void mat3_mul( double AB[3][3], double A[3][3], double B[3][3] );
void mat3_mul_vec( double out[3], double M[3][3], const double v[3] );
int mat3_comp( double A[3][3], double B[3][3] );
void mat3_print( double M[3][3] );


#endif /* _MAT3_H */

