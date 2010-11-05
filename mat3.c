

#include "mat3.h"

#include <stdio.h>
#include <math.h>

double vec3_dot( const double u[3], const double v[3] )
{
   return u[0]*v[0] + u[1]*v[1] + u[2]*v[2];
}


void vec3_cross( double o[3], const double u[3], const double v[3] )
{
   o[0] =  u[1]*v[2] - u[2]*v[1];
   o[1] = -u[0]*v[2] + u[2]*v[0];
   o[2] =  u[0]*v[1] - u[1]*v[0];
}


double vec3_norm( const double v[3] )
{
   return sqrt( v[0]*v[0] + v[1]*v[1] + v[2]*v[2] );
}


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
   for (c=0; c<3; c++) {
      for (r=0; r<3; r++) {
         AB[r][c] = A[r][0]*B[0][c] + A[r][1]*B[1][c] + A[r][2]*B[2][c];
      }
   }
}


void mat3_mul_vec( double out[3], double M[3][3], const double v[3] )
{
   out[0] = M[0][0]*v[0] + M[0][1]*v[1] + M[0][2]*v[2];
   out[1] = M[1][0]*v[0] + M[1][1]*v[1] + M[1][2]*v[2];
   out[2] = M[2][0]*v[0] + M[2][1]*v[1] + M[2][2]*v[2];
}


int mat3_comp( double A[3][3], double B[3][3] )
{
   int c,r, ret;
   ret = 0;
   for (c=0; c<3; c++) {
      for (r=0; r<3; r++) {
         if (fabs(A[r][c] - B[r][c]) > 1e-10) {
            ret = ret + 1;
         }
      }
   }
   return ret;
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



