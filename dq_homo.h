

#ifndef _DQ_HOMO_H
#  define _DQ_HOMO_H


void homo_cr_join( double H[3][4], double R[3][3], double d[3] );
void homo_op_mul( double O[3][4], double A[3][4], double B[3][4] );
void homo_op_split( double R[3][3], double d[3], double H[3][4] );
void homo_op_mul_vec( double o[4], double H[3][4], const double v[4] );
void homo_print( double H[3][4] );


#endif /* _DQ_HOMO_H */

