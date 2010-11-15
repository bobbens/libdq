


#include "dq.h"

#include <stdio.h>
#include <math.h>
#include <string.h>
#ifdef DQ_CHECK
#include <assert.h>
#endif /* DQ_CHECK */

#include "mat3.h"


void dq_cr_rotation( dq_t O, double theta, const double s[3], const double c[3] )
{
   double ss, cs, s0[3];

   /* Store sin and cos values to speed up calculations. */
   ss = sin( theta/2. );
   cs = cos( theta/2. );

   O[0] = cs;
   O[1] = ss*s[0];
   O[2] = ss*s[1];
   O[3] = ss*s[2];
   /* We do cross product with the line point and line vector to get the plucker coordinates. */
   vec3_cross( s0, c, s );
   O[4] = ss*s0[0];
   O[5] = ss*s0[1];
   O[6] = ss*s0[2];
   O[7] = 0.;
}


void dq_cr_rotation_plucker( dq_t O, double theta, const double s[3], const double s0[3] )
{
   double ss, cs;

#if DQ_CHECK
   assert( fabs(vec3_dot(s,s0)) < DQ_PRECISION );
#endif /* DQ_CHECK */

   /* Store sin and cos values to speed up calculations. */
   ss = sin( theta/2. );
   cs = cos( theta/2. );

   O[0] = cs;
   O[1] = ss*s[0];
   O[2] = ss*s[1];
   O[3] = ss*s[2];
   O[4] = ss*s0[0];
   O[5] = ss*s0[1];
   O[6] = ss*s0[2];
   O[7] = 0.;
}


void dq_cr_rotation_matrix( dq_t O, double R[3][3] )
{
   double Rminus[3][3], Rplus[3][3], Rinv[3][3], B[3][3], eye[3][3];
   double s[3];
   double z2, tz, sz, cz;

#ifdef DQ_CHECK
   assert( fabs(mat3_det(R) - 1.) < DQ_PRECISION );
#endif /* DQ_CHECK */

   /* B = (R-I)(R+I)^{-1} */
   mat3_eye( eye );
   mat3_sub( Rminus, R, eye );
   mat3_add( Rplus, R, eye );
   mat3_inv( Rinv, Rplus );
   mat3_mul( B, Rminus, Rinv );

   /*
    *      0 -b_z  b_y
    * B = b_z  0  -b_x
    *    -b_y b_x   0
    *
    * b = { b_x b_y b_z }
    *
    * s           = b / ||b||
    * tan(theta/2) = ||b||
    */
   s[0] = B[2][1];
   s[1] = B[0][2];
   s[2] = B[1][0];
   tz   = vec3_norm( s );
   /* Avoid normalizing 0. vectors. */
   if (tz > 0.) {
      s[0] /= tz;
      s[1] /= tz;
      s[2] /= tz;
   }
   z2   = atan(tz);

    /*
     * Build the rotational part.
     */
    sz = sin( z2 );
    cz = cos( z2 );
    O[0] = cz;
    O[1] = sz*s[0];
    O[2] = sz*s[1];
    O[3] = sz*s[2];
    O[4] = 0.;
    O[5] = 0.;
    O[6] = 0.;
    O[7] = 0.;
}


void dq_cr_translation( dq_t O, double t, const double s[3] )
{
   O[0] = 1.;
   O[1] = 0.;
   O[2] = 0.;
   O[3] = 0.;
   O[4] = t*s[0] / 2.;
   O[5] = t*s[1] / 2.;
   O[6] = t*s[2] / 2.;
   O[7] = 0.;
}


void dq_cr_translation_vector( dq_t O, const double t[3] )
{
   O[0] = 1.;
   O[1] = 0.;
   O[2] = 0.;
   O[3] = 0.;
   O[4] = t[0] / 2.;
   O[5] = t[1] / 2.;
   O[6] = t[2] / 2.;
   O[7] = 0.;
}


void dq_cr_point( dq_t O, const double pos[3] )
{
   O[0] = 1.;
   O[1] = 0.;
   O[2] = 0.;
   O[3] = 0.;
   O[4] = pos[0];
   O[5] = pos[1];
   O[6] = pos[2];
   O[7] = 0.;
}


void dq_cr_line( dq_t O, const double s[3], const double c[3] )
{
   double v[3];
   /* We do cross product with the line point and line vector to get the plucker coordinates. */
   vec3_cross( v, c, s );
   dq_cr_line_plucker( O, s, v );
}


void dq_cr_line_plucker( dq_t O, const double s[3], const double c[3] )
{
   O[0] = 0.;
   O[1] = s[0];
   O[2] = s[1];
   O[3] = s[2];
   O[4] = c[0];
   O[5] = c[1];
   O[6] = c[2];
   O[7] = 0.;
}


void dq_cr_homo( dq_t O, double R[3][3], const double d[3] )
{
   dq_t QR, QT;

#ifdef DQ_CHECK
   assert( fabs(mat3_det(R) - 1.) < DQ_PRECISION );
#endif /* DQ_CHECK */

   dq_cr_rotation_matrix( QR, R );
   dq_cr_translation_vector( QT, d );
   dq_op_mul( O, QT, QR );
}


void dq_cr_copy( dq_t O, const dq_t Q )
{
   memcpy( O, Q, sizeof(dq_t) );
}


void dq_cr_conj( dq_t O, const dq_t Q )
{
   O[0] =  Q[0];
   O[1] = -Q[1];
   O[2] = -Q[2];
   O[3] = -Q[3];
   O[4] =  Q[4];
   O[5] = -Q[5];
   O[6] = -Q[6];
   O[7] = -Q[7];
}


void dq_cr_inv( dq_t O, const dq_t Q )
{
   double real, dual;

   /* Get the dual number of t he norm. */
   dq_op_norm2( &real, &dual, Q );
  
   /* Set the values. */
   O[0] =  Q[0] * real;
   O[1] = -Q[1] * real;
   O[2] = -Q[2] * real;
   O[3] = -Q[3] * real;
   O[4] =  Q[4] * (dual-real);
   O[5] =  Q[5] * (dual-real);
   O[6] =  Q[6] * (dual-real);
   O[7] =  Q[7] * (real-dual);
}


void dq_op_norm2( double *real, double *dual, const dq_t Q )
{
   *real =     Q[0]*Q[0] + Q[1]*Q[1] + Q[2]*Q[2] + Q[3]*Q[3];
   *dual = 2.*(Q[0]*Q[7] + Q[1]*Q[4] + Q[2]*Q[5] + Q[3]*Q[6]);
}


void dq_op_add( dq_t O, const dq_t P, const dq_t Q )
{
   int i;
   for (i=0; i<8; i++)
      O[i] = P[i] + Q[i];
}


void dq_op_sub( dq_t O, const dq_t P, const dq_t Q )
{
   int i;
   for (i=0; i<8; i++)
      O[i] = P[i] - Q[i];
}


void dq_op_mul( dq_t PQ, const dq_t P, const dq_t Q )
{
   dq_t T;
   /* Multiplication table:
    *
    *  Q1*Q2 | Q2.1  Q2.i  Q2.j  Q2.k  Q2.ei  Q2.ej  Q2.ek  Q2.e
    *  ------+---------------------------------------------------
    *  Q1.1  |   1     i     j     k    ei     ej     ek      e
    *  Q1.i  |   i    -1     k    -j    -e     ek     -ej    ei
    *  Q1.j  |   j    -k    -1     i    -ek    -e     ei     ej
    *  Q1.k  |   k     j    -i    -1    ej     -ei    -e     ek
    *  Q1.ei |  ei    -e    ek    -ej    0      0      0      0
    *  Q1.ej |  ej    -ek   -e    ei     0      0      0      0
    *  Q1.ek |  ek    ej    -ei   -e     0      0      0      0
    *  Q1.e  |   e    ei     ej   ek     0      0      0      0
    */
   /* Real quaternion. */
   T[0] = P[0]*Q[0] - P[1]*Q[1] - P[2]*Q[2] - P[3]*Q[3];
   T[1] = P[0]*Q[1] + P[1]*Q[0] + P[2]*Q[3] - P[3]*Q[2];
   T[2] = P[0]*Q[2] + P[2]*Q[0] - P[1]*Q[3] + P[3]*Q[1];
   T[3] = P[0]*Q[3] + P[3]*Q[0] + P[1]*Q[2] - P[2]*Q[1];

   /* Dual unit Quaternion. */
   T[4] = P[4]*Q[0] + P[0]*Q[4] + P[7]*Q[1] + P[1]*Q[7] -
          P[6]*Q[2] + P[2]*Q[6] + P[5]*Q[3] - P[3]*Q[5];
   T[5] = P[5]*Q[0] + P[0]*Q[5] + P[6]*Q[1] - P[1]*Q[6] +
          P[7]*Q[2] + P[2]*Q[7] - P[4]*Q[3] + P[3]*Q[4];
   T[6] = P[6]*Q[0] + P[0]*Q[6] - P[5]*Q[1] + P[1]*Q[5] +
          P[4]*Q[2] - P[2]*Q[4] + P[7]*Q[3] + P[3]*Q[7];
   T[7] = P[7]*Q[0] + P[0]*Q[7] - P[1]*Q[4] - P[4]*Q[1] -
          P[2]*Q[5] - P[5]*Q[2] - P[3]*Q[6] - P[6]*Q[3];

   /* Copy over results. */
   memcpy( PQ, T, sizeof(dq_t) );
}


void dq_op_f1g( dq_t ABA, const dq_t A, const dq_t B )
{
   dq_op_mul( ABA, A, B );
   dq_op_mul( ABA, ABA, A );
}


void dq_op_f2g( dq_t ABA, const dq_t A, const dq_t B )
{
   dq_t Astar;

   dq_op_mul( ABA, A, B );
   dq_cr_conj( Astar, A );
   dq_op_mul( ABA, ABA, Astar );
}

void dq_op_f3g( dq_t ABA, const dq_t A, const dq_t B )
{
   dq_t Astar;

   dq_op_mul( ABA, A, B );
   Astar[0] =  A[0];
   Astar[1] =  A[1];
   Astar[2] =  A[2];
   Astar[3] =  A[3];
   Astar[4] = -A[4];
   Astar[5] = -A[5];
   Astar[6] = -A[6];
   Astar[7] = -A[7];
   dq_op_mul( ABA, ABA, Astar );
}


void dq_op_f4g( dq_t ABA, const dq_t A, const dq_t B )
{
   dq_t Astar;

   dq_op_mul( ABA, A, B );
   Astar[0] =  A[0];
   Astar[1] = -A[1];
   Astar[2] = -A[2];
   Astar[3] = -A[3];
   Astar[4] =  A[4];
   Astar[5] =  A[5];
   Astar[6] =  A[6];
   Astar[7] = -A[7];
   dq_op_mul( ABA, ABA, Astar );
}


int dq_ch_unit( const dq_t Q )
{
   double real, dual;
   dq_op_norm2( &real, &dual, Q );
   if ((fabs(real-1.) > DQ_PRECISION) || (fabs(dual-0.) > DQ_PRECISION))
      return 0;
   return 1;
}


int dq_ch_cmp( const dq_t P, const dq_t Q )
{
   return dq_ch_cmpV( P, Q, DQ_PRECISION );
}


int dq_ch_cmpV( const dq_t P, const dq_t Q, double precision )
{
   int i, ret;
   
   ret = 0;
   for (i=0; i<8; i++)
      if (fabs(P[i]-Q[i]) > precision)
         ret++;

   return ret;
}


void dq_print( const dq_t Q )
{
   printf( "%.3f + %.3fi + %.3fj + %.3fk + %.3fie + %.3fje + %.3fke + %.3fe\n",
         Q[0], Q[1], Q[2], Q[3], Q[4], Q[5], Q[6], Q[7] );
}


void dq_print_vert( const dq_t Q )
{
   printf( "   % 3.3fi   % 3.3fi\n", Q[1], Q[4] );
   printf( "   % 3.3fj   % 3.3fj\n", Q[2], Q[5] );
   printf( "   % 3.3fk + % 3.3fk\n", Q[3], Q[6] );
   printf( "   % 3.3f    % 3.3f\n",  Q[0], Q[7] );
}


