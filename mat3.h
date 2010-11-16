

#ifndef _MAT3_H
#  define _MAT3_H


double vec3_dot( const double u[3], const double v[3] );
void vec3_cross( double o[3], const double u[3], const double v[3] );
void vec3_add( double o[3], const double u[3], const double v[3] );
void vec3_sub( double o[3], const double u[3], const double v[3] );
double vec3_norm( const double v[3] );
void vec3_normalize( double v[3] );
int vec3_cmp( const double u[3], const double v[3] );
void vec3_print( const double v[3] );


void mat3_eye( double M[3][3] );
double mat3_det( double M[3][3] );
void mat3_add( double out[3][3], double A[3][3], double B[3][3] );
void mat3_sub( double out[3][3], double A[3][3], double B[3][3] );
void mat3_inv( double out[3][3], double in[3][3] );
void mat3_mul( double AB[3][3], double A[3][3], double B[3][3] );
void mat3_mul_vec( double out[3], double M[3][3], const double v[3] );
int mat3_cmp( double A[3][3], double B[3][3] );
void mat3_print( double M[3][3] );

void homo_cr_join( double H[3][4], double R[3][3], double d[3] );
void homo_op_mul( double O[3][4], double A[3][4], double B[3][4] );
void homo_op_split( double R[3][3], double d[3], double H[3][4] );
void homo_op_mul_vec( double o[4], double H[3][4], const double v[4] );
void homo_print( double H[3][4] );


#endif /* _MAT3_H */

