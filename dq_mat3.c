#include "dq_mat3.h"

#include <stdio.h>
#include <math.h>
#include <string.h>
#ifdef DQ_CHECK
#include <assert.h>
#endif /* DQ_CHECK */

#include "dq.h"


void mat3_eye( double M[3][3] )
{
   M[0][0] = 1.;
   M[0][1] = 0.;
   M[0][2] = 0.;
   M[1][0] = 0.;
   M[1][1] = 1.;
   M[1][2] = 0.;
   M[2][0] = 0.;
   M[2][1] = 0.;
   M[2][2] = 1.;
}


double mat3_det( double M[3][3] )
{
   return M[0][0]*M[1][1]*M[2][2] +
          M[1][0]*M[2][1]*M[0][2] +
          M[2][0]*M[0][1]*M[1][2] -
          M[0][2]*M[1][1]*M[2][0] -
          M[0][0]*M[2][1]*M[1][2] -
          M[1][0]*M[0][1]*M[2][2];
}


void mat3_add( double out[3][3], double A[3][3], double B[3][3] )
{
   int c,r;
   for (c=0; c<3; c++) {
      for (r=0; r<3; r++) {
         out[r][c] = A[r][c] + B[r][c];
      }
   }
}


void mat3_sub( double out[3][3], double A[3][3], double B[3][3] )
{
   int c,r;
   for (c=0; c<3; c++) {
      for (r=0; r<3; r++) {
         out[r][c] = A[r][c] - B[r][c];
      }
   }
}


void mat3_inv( double out[3][3], double in[3][3] )
{
   double det;

   det = mat3_det(in);

#ifdef DQ_CHECK
   assert( fabs(det) > DQ_PRECISION );
#endif /* DQ_CHECK */

   out[0][0] = (in[1][1]*in[2][2] - in[2][1]*in[1][2])/det;
   out[0][1] = (in[0][2]*in[2][1] - in[2][2]*in[0][1])/det;
   out[0][2] = (in[0][1]*in[1][2] - in[1][1]*in[0][2])/det;
   out[1][0] = (in[1][2]*in[2][0] - in[2][2]*in[1][0])/det;
   out[1][1] = (in[0][0]*in[2][2] - in[2][0]*in[0][2])/det;
   out[1][2] = (in[0][2]*in[1][0] - in[1][2]*in[0][0])/det;
   out[2][0] = (in[1][0]*in[2][1] - in[2][0]*in[1][1])/det;
   out[2][1] = (in[0][1]*in[2][0] - in[2][1]*in[0][0])/det;
   out[2][2] = (in[0][0]*in[1][1] - in[1][0]*in[0][1])/det;
}


void mat3_mul( double AB[3][3], double A[3][3], double B[3][3] )
{
   int c,r;
   double T[3][3];
   for (c=0; c<3; c++) {
      for (r=0; r<3; r++) {
         T[r][c] = A[r][0]*B[0][c] + A[r][1]*B[1][c] + A[r][2]*B[2][c];
      }
   }
   memcpy( AB, T, sizeof(double)*3*3 );
}


void mat3_mul_vec( double out[3], double M[3][3], const double v[3] )
{
   double t[3];
   t[0] = M[0][0]*v[0] + M[0][1]*v[1] + M[0][2]*v[2];
   t[1] = M[1][0]*v[0] + M[1][1]*v[1] + M[1][2]*v[2];
   t[2] = M[2][0]*v[0] + M[2][1]*v[1] + M[2][2]*v[2];
   memcpy( out, t, sizeof(double)*3 );
}


void mat3_solve( double x[3], double A[3][3], const double b[3] )
{
   int i, j;
   double dA, dT, T[3][3];

   dA = mat3_det( A );
#ifdef DQ_CHECK
   assert( fabs(dA) > DQ_PRECISION );
#endif /* DQ_CHECK */

   for (i=0; i<3; i++) {
      memcpy( T, A, sizeof(double)*9 );
      for (j=0; j<3; j++)
         T[j][i] = b[j];
      dT = mat3_det( T );
      x[i] = dT / dA;
   }
}


int mat3_cmpV( double A[3][3], double B[3][3], double precision )
{
   int c,r, ret;
   ret = 0;
   for (c=0; c<3; c++) {
      for (r=0; r<3; r++) {
         if (fabs(A[r][c] - B[r][c]) > precision) {
            ret = ret + 1;
         }
      }
   }
   return ret;
}


int mat3_cmp( double A[3][3], double B[3][3] )
{
   return mat3_cmpV( A, B, DQ_PRECISION );
}


void mat3_print( double M[3][3] )
{
   printf( "   % 3.3f % 3.3f % 3.3f\n"
           "   % 3.3f % 3.3f % 3.3f\n"
           "   % 3.3f % 3.3f % 3.3f\n",
           M[0][0], M[0][1], M[0][2],
           M[1][0], M[1][1], M[1][2],
           M[2][0], M[2][1], M[2][2] );
}
