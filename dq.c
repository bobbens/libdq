


#include "dq.h"

#include <stdio.h>
#include <math.h>
#include <string.h>


void dq_cr_rotation( dq_t O, double zita, const double s[3], const double c[3] )
{
   double ss, cs;

   /* Store sin and cos values to speed up calculations. */
   ss = sin( zita/2. );
   cs = cos( zita/2. );

   O[0] = cs;
   O[1] = ss*s[0];
   O[2] = ss*s[1];
   O[3] = ss*s[2];
   /* We do cross product with the line point and line vector to get the plucker coordinates. */
   O[4] = ss*(  s[1]*c[2] - s[2]*c[1] );
   O[5] = ss*( -s[0]*c[2] + s[2]*c[0] );
   O[6] = ss*(  s[0]*c[1] - s[1]*c[0] );
   O[7] = 0.;
}


void dq_cr_translation( dq_t O, const double t[3] )
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


