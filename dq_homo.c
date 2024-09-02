#include "dq_homo.h"

#include <stdio.h>
#include <math.h>
#include <string.h>
#ifdef DQ_CHECK
#include <assert.h>
#endif /* DQ_CHECK */

#include "dq.h"


void homo_cr_join( double H[3][4], double R[3][3], double d[3] )
{
   int i, j;

#ifdef DQ_CHECK
   assert( fabs(mat3_det(R)-1.) < DQ_PRECISION);
#endif /* DQ_CHECK */

   for (j=0; j<3; j++)
      for (i=0; i<3; i++)
         H[j][i] = R[j][i];
   for (i=0; i<3; i++)
      H[i][3] = d[i];
}


void homo_op_mul( double O[3][4], double A[3][4], double B[3][4] )
{
   double H[3][4];
   int i, j;

   for (j=0; j<3; j++)
      for (i=0; i<4; i++)
         H[j][i] = A[j][0]*B[0][i] + A[j][1]*B[1][i] + A[j][2]*B[2][i];
   for (i=0; i<3; i++)
      H[i][3] += A[i][3];

   memcpy( O, H, sizeof(double)*3*4 );
}


void homo_op_mul_vec( double o[4], double H[3][4], const double v[4] )
{
   int i;
   for (i=0; i<3; i++)
      o[i] = H[i][0]*v[0] + H[i][1]*v[1] + H[i][2]*v[2] + H[i][3]*v[3];
   o[3] = v[3];
}


void homo_op_split( double R[3][3], double d[3], double H[3][4] )
{
   int i, j;
   for (j=0; j<3; j++)
      for (i=0; i<3; i++)
         R[j][i] = H[j][i];
   for (i=0; i<3; i++)
      d[i] = H[i][3];
}


int homo_ch_cmpV( double A[3][4], double B[3][4], double precision )
{
   int i, j, ret;
   ret = 0;
   for (j=0; j<3; j++)
      for (i=0; i<4; i++)
         if (fabs(A[j][i]-B[j][i]) > precision)
            ret = ret + 1;
   return ret;
}


int homo_ch_cmp( double A[3][4], double B[3][4] )
{
   return homo_ch_cmpV( A, B, DQ_PRECISION );
}


void homo_print( double H[3][4] )
{
   printf( "   % 3.3f % 3.3f % 3.3f % 3.3f\n"
           "   % 3.3f % 3.3f % 3.3f % 3.3f\n"
           "   % 3.3f % 3.3f % 3.3f % 3.3f\n",
           H[0][0], H[0][1], H[0][2], H[0][3],
           H[1][0], H[1][1], H[1][2], H[1][3],
           H[2][0], H[2][1], H[2][2], H[2][3] );
}
