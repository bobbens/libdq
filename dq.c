


#include "dq.h"

#include <stdio.h>
#include <math.h>
#include <string.h>

#include "mat3.h"


void dq_cr_rotation( dq_t O, double zita, const double s[3], const double c[3] )
{
   double ss, cs, o[3];

   /* Store sin and cos values to speed up calculations. */
   ss = sin( zita/2. );
   cs = cos( zita/2. );

   O[0] = cs;
   O[1] = ss*s[0];
   O[2] = ss*s[1];
   O[3] = ss*s[2];
   /* We do cross product with the line point and line vector to get the plucker coordinates. */
   vec3_cross( o, s, c );
   O[4] = ss*o[0];
   O[5] = ss*o[1];
   O[6] = ss*o[2];
   O[7] = 0.;
}


void dq_cr_rotation_plucker( dq_t O, double zita, const double s[3], const double c[3] )
{
   double ss, cs;

   /* Store sin and cos values to speed up calculations. */
   ss = sin( zita/2. );
   cs = cos( zita/2. );

   O[0] = cs;
   O[1] = ss*s[0];
   O[2] = ss*s[1];
   O[3] = ss*s[2];
   O[4] = ss*c[0];
   O[5] = ss*c[1];
   O[6] = ss*c[2];
   O[7] = 0.;
}


void dq_cr_rotation_matrix( dq_t O, double R[3][3] )
{
   double Rminus[3][3], Rplus[3][3], Rinv[3][3], B[3][3], eye[3][3];
   double s[3], zita;
   double z2, tz, sz, cz;

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
    * tan(zita/2) = ||b||
    */
   s[0] = B[2][1];
   s[1] = B[1][0];
   s[2] = B[0][2];
   tz   = vec3_norm( s );
   s[0] /= tz;
   s[1] /= tz;
   s[2] /= tz;
   z2   = atan(tz);
   zita = 2.*z2;

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
   vec3_cross( v, s, c );
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
   dq_cr_rotation_matrix( QR, R );
   dq_cr_translation_vector( QT, d );
   dq_op_mul( O, QT, QR );
#if 0
   double Rminus[3][3], Rplus[3][3], Rinv[3][3], B[3][3], eye[3][3];
   double T[3][3], T2[3][3], T3[3][3];
   double v[3];
   double s[3], zita;
   double n, z2, t2, tz, sz, cz;

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
    * tan(zita/2) = ||b||
    */
   s[0] = B[2][1];
   s[1] = B[1][0];
   s[2] = B[0][2];
   tz   = vec3_norm( s );
   s[0] /= tz;
   s[1] /= tz;
   s[2] /= tz;
   z2   = atan(tz);
   zita = 2.*z2;

   /*
    * v = 1/(2*tan^3(zita/2)) B*B(I-B)d
    */
   mat3_sub( T, eye, B );
   mat3_mul( T2, B, B );
   mat3_mul( T3, T2, T );
   mat3_mul_vec( v, T3, d );
   n = 1. / (2. * pow( tz, 3. ));
   v[0] /= n;
   v[1] /= n;
   v[2] /= n;

   /*
    * Calculate the translation part.
    *
    * d = d* + t s
    * d.s = d*.s + t
    * d*.s = 0
    * t = d.s
    */
   t2 = vec3_dot( d, s ) / 2.;

   /*
    * Build the rotational part.
    */
   sz = sin( z2 );
   cz = cos( z2 );
   O[0] = cz;
   O[1] = sz*s[0];
   O[2] = sz*s[1];
   O[3] = sz*s[2];
   O[4] = sz*v[0] + t2*s[0]*cz;
   O[5] = sz*v[1] + t2*s[1]*cz;
   O[6] = sz*v[2] + t2*s[2]*cz;
   O[7] = -t2*sz;
#endif
}


void dq_cr_copy( dq_t O, dq_t Q )
{
   int i;
   for (i=0; i<8; i++)
      O[i] = Q[i];
}


void dq_cr_conj( dq_t O, dq_t Q )
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


double dq_op_norm( dq_t Q )
{
   int i;
   double acc;
   dq_t T, Qstar;
   dq_cr_conj( Qstar, Q );
   dq_op_mul( T, Q, Qstar );
   acc = 0.;
   for (i=0; i<8; i++)
      acc += T[i];
   return sqrt( acc );
}


void dq_op_add( dq_t O, dq_t P, dq_t Q )
{
   int i;
   for (i=0; i<8; i++)
      O[i] = P[i] + Q[i];
}


void dq_op_sub( dq_t O, dq_t P, dq_t Q )
{
   int i;
   for (i=0; i<8; i++)
      O[i] = P[i] - Q[i];
}


void dq_op_mul( dq_t PQ, dq_t P, dq_t Q )
{
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
   PQ[0] = P[0]*Q[0] - P[1]*Q[1] - P[2]*Q[2] - P[3]*Q[3];
   PQ[1] = P[0]*Q[1] + P[1]*Q[0] + P[2]*Q[3] - P[3]*Q[2];
   PQ[2] = P[0]*Q[2] + P[2]*Q[0] + P[1]*Q[3] - P[3]*Q[1];
   PQ[3] = P[0]*Q[3] + P[3]*Q[0] + P[1]*Q[2] - P[2]*Q[1];

   /* Dual unit Quaternion. */
   PQ[4] = P[4]*Q[0] + P[0]*Q[4] + P[7]*Q[1] + P[1]*Q[7] -
           P[6]*Q[2] + P[2]*Q[6] + P[5]*Q[3] - P[3]*Q[5];
   PQ[5] = P[5]*Q[0] + P[0]*Q[5] + P[6]*Q[1] - P[1]*Q[6] +
           P[7]*Q[2] + P[2]*Q[7] - P[4]*Q[3] + P[3]*Q[4];
   PQ[6] = P[6]*Q[0] + P[0]*Q[6] - P[5]*Q[1] + P[1]*Q[5] +
           P[4]*Q[2] - P[2]*Q[4] + P[7]*Q[3] + P[3]*Q[7];
   PQ[7] = P[7]*Q[0] + P[0]*Q[7] - P[1]*Q[4] - P[4]*Q[1] -
           P[2]*Q[5] - P[5]*Q[2] - P[3]*Q[6] - P[6]*Q[3];
}


void dq_op_f1g( dq_t ABA, dq_t A, dq_t B )
{
   dq_t tmp;

   dq_op_mul( tmp, A, B );
   dq_op_mul( ABA, tmp, A );
}


void dq_op_f2g( dq_t ABA, dq_t A, dq_t B )
{
   dq_t tmp, Astar;

   dq_op_mul( tmp, A, B );
   dq_cr_conj( Astar, A );
   dq_op_mul( ABA, tmp, Astar );
}

void dq_op_f3g( dq_t ABA, dq_t A, dq_t B )
{
   dq_t tmp, Astar;

   dq_op_mul( tmp, A, B );
   Astar[0] =  A[0];
   Astar[1] =  A[1];
   Astar[2] =  A[2];
   Astar[3] =  A[3];
   Astar[4] = -A[4];
   Astar[5] = -A[5];
   Astar[6] = -A[6];
   Astar[7] = -A[7];
   dq_op_mul( ABA, tmp, Astar );
}


void dq_op_f4g( dq_t ABA, dq_t A, dq_t B )
{
   dq_t tmp, Astar;

   dq_op_mul( tmp, A, B );
   Astar[0] =  A[0];
   Astar[1] = -A[1];
   Astar[2] = -A[2];
   Astar[3] = -A[3];
   Astar[4] =  A[4];
   Astar[5] =  A[5];
   Astar[6] =  A[6];
   Astar[7] = -A[7];
   dq_op_mul( ABA, tmp, Astar );
}


int dq_cmp( dq_t P, dq_t Q )
{
   int i, ret;
   
   ret = 0;
   for (i=0; i<8; i++)
      if (fabs(P[i]-Q[i]) > 1e-10)
         ret++;

   return ret;
}


void dq_print( dq_t Q )
{
   printf( "%.3f + %.3fi + %.3fj + %.3fk + %.3fie + %.3fje + %.3fke + %.3fe\n",
         Q[0], Q[1], Q[2], Q[3], Q[4], Q[5], Q[6], Q[7] );
}


void dq_print_vert( dq_t Q )
{
   printf( "   % 3.3fi   % 3.3fi\n", Q[1], Q[4] );
   printf( "   % 3.3fj   % 3.3fj\n", Q[2], Q[5] );
   printf( "   % 3.3fk + % 3.3fk\n", Q[3], Q[6] );
   printf( "   % 3.3f    % 3.3f\n",  Q[0], Q[7] );
}


