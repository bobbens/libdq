
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dq.h>
#include "../mat3.h"

/**
 * Small example of using dual quaternions to solve the hand reconstruction problem.
 *
 * The approach is specifically handled in the paper:
 *
 * @INPROCEEDINGS{smarthandprosthesis,
 *    author={Duraisamy, K. and Isebor, O. and Perez, A. and Schoen, M.P. and Naidu, D.S.},
 *    booktitle={Biomedical Robotics and Biomechatronics, 2006. BioRob 2006. The First IEEE/RAS-EMBS International Conference on}, 
 *    title={Kinematic Synthesis for Smart Hand Prosthesis},
 *    year={2006},
 *    month=feb.,
 *    volume={},
 *    number={},
 *    pages={1135 -1140},
 *    keywords={Clifford algebra exponential;EMG-based interface;artificial hands;biomechanical modeling;bionic replacement appendage;human hand joint motion;kinematic synthesis;mechatronic prostheses;motion control;motion tracking system;myoelectric signal;prostheses control system;robotics;serial robot;smart hand prosthesis;biocybernetics;biomechanics;electromyography;mechatronics;medical robotics;motion control;prosthetics;},
 *    doi={10.1109/BIOROB.2006.1639245},
 *    ISSN={}
 * }
 *
 */


static int cur_seed     = 0; /**< Current seed to keep things sane. */


/**
 * @brief Generates a random value from [0:1].
 */
static double rnd (void)
{
   return (double)rand() / (double)RAND_MAX;
}


/**
 * @brief Creates your standard rotation matrix.
 *
 *    @param[out] Rotation matrix output.
 *    @param thetax Rotation around X axis.
 *    @param thetay Rotation around Y axis.
 *    @param thetaz Rotation around Z axis.
 */
static void make_rot( double R[3][3], double thetax, double thetay, double thetaz )
{
   double Rx[3][3], Ry[3][3], Rz[3][3];
   double c, s;

   memset( Rx, 0, sizeof(double)*9 );
   memset( Ry, 0, sizeof(double)*9 );
   memset( Rz, 0, sizeof(double)*9 );

   c = cos(thetax); s = sin(thetax);
   Rx[0][0] =  1.;
   Rx[1][1] =  c;
   Rx[1][2] = -s;
   Rx[2][1] =  s;
   Rx[2][2] =  c;

   c = cos(thetay); s = sin(thetay);
   Ry[0][0] =  c;
   Ry[0][2] =  s;
   Ry[1][1] =  1.;
   Ry[2][0] = -s;
   Ry[2][2] =  c;

   c = cos(thetaz); s = sin(thetaz);
   Rz[0][0] =  c;
   Rz[0][1] = -s;
   Rz[1][0] =  s;
   Rz[1][1] =  c;
   Rz[2][2] =  1.;

   mat3_mul( R, Rx, Ry );
   mat3_mul( R, R,  Rz );
}


/**
 * @brief Extracts the rotation and position from a homogeneous matrix.
 */
static void extract_rot( double dir[3], double pos[3], double H[3][4] )
{
   pos[0] = H[0][3];
   pos[1] = H[1][3];
   pos[2] = H[2][3];

   dir[0] = H[0][2];
   dir[1] = H[1][2];
   dir[2] = H[2][2];
}


/**
 * @brief Calculates all the robot matrixs for the movement for given angles.
 *
 * S Z1 X1 Z2 X2 Z3 X3 E
 */
static void calc_M( double Z1[3][4], double Z2[3][4], double Z3[3][4],
      double S[3][4], double X1[3][4], double X2[3][4], double E[3][4],
      const double a[3] )
{
   double R[3][3], d[3];

   srand( cur_seed );

   /* S */
   make_rot( R, rnd()*2.*M_PI, rnd()*2.*M_PI, rnd()*2.*M_PI );
   d[0] = rnd(); d[1] = rnd(); d[2] = rnd();
   homo_cr_join( S, R, d );

   /* Z1 */
   make_rot( R, 0., 0., a[0] );
   d[0] = d[1] = 0.; d[2] = rnd();
   homo_cr_join( Z1, R, d );

   /* X1 */
   make_rot( R, rnd()*2.*M_PI, 0., 0. );
   d[2] = d[1] = 0.; d[0] = rnd();
   homo_cr_join( X1, R, d );

   /* Z2 */
   make_rot( R, 0., 0., a[1] );
   d[0] = d[1] = 0.; d[2] = rnd();
   homo_cr_join( Z2, R, d );

   /* X2 */
   make_rot( R, rnd()*2.*M_PI, 0., 0. );
   d[2] = d[1] = 0.; d[0] = rnd();
   homo_cr_join( X2, R, d );

   /* Z3 */
   make_rot( R, 0., 0., a[2] );
   d[0] = d[1] = 0.; d[2] = rnd();
   homo_cr_join( Z3, R, d );

   /* E */
   make_rot( R, rnd()*2.*M_PI, rnd()*2.*M_PI, rnd()*2.*M_PI );
   d[0] = rnd(); d[1] = rnd(); d[2] = rnd();
   homo_cr_join( E, R, d );
}


static void calc_R( dq_t R, const double a0[3], const double a[3] )
{
   double H[3][4], Z1[3][4], Z2[3][4], Z3[3][4], S[3][4], X1[3][4], X2[3][4], E[3][4];
   double s[3], s0[3];
   dq_t R1, R2, R3;

   /* Calculate the matrix. */
   calc_M( Z1, Z2, Z3, S, X1, X2, E, a0 );

   homo_op_mul( H, S, Z1 );
   extract_rot( s, s0, H );
   dq_cr_rotation( R1, a[0], s, s0 );

   homo_op_mul( H, H, X1 );
   homo_op_mul( H, H, Z2 );
   extract_rot( s, s0, H );
   dq_cr_rotation( R2, a[1], s, s0 );

   homo_op_mul( H, H, X2 );
   homo_op_mul( H, H, Z3 );
   extract_rot( s, s0, H );
   dq_cr_rotation( R3, a[2], s, s0 );

   homo_op_mul( H, H, E  );

   /* Multiply. */
   dq_op_mul( R, R1, R2 );
   dq_op_mul( R, R,  R3 );
}


/**
 * @brief Calculates the resulting dual quaternion for given angles.
 */
static void calc_T( dq_t T, const double a[3] )
{
   double H[3][4], Z1[3][4], Z2[3][4], Z3[3][4], S[3][4], X1[3][4], X2[3][4], E[3][4];
   double R[3][3], d[3];

   /* Calculate the matrix. */
   calc_M( Z1, Z2, Z3, S, X1, X2, E, a );

   /* Multiply S Z1 X1 Z2 X2 Z3 E */
   homo_op_mul( H, S, Z1 );
   homo_op_mul( H, H, X1 );
   homo_op_mul( H, H, Z2 );
   homo_op_mul( H, H, X2 );
   homo_op_mul( H, H, Z3 );
   homo_op_mul( H, H, E  );

   /* Now we create a dual quaternion. */
   homo_op_split( R, d, H );
   dq_cr_homo( T, R, d );
}


/**
 * @brief Entry point.
 */
int main( int argc, char *argv[] )
{
   int i, j;
   double a0[3], a1[3], a01[3];
   dq_t T0, T1, R01, T01, Tinv;
   (void) argc;
   (void) argv;

   srand(0);

   for (j=0; j<5000; j++) {

      cur_seed = rand();

      /* Create rotations and static stuff. */
      for (i=0; i<3; i++) {
         a0[i]  = rnd();
         a1[i]  = rnd();
         a01[i] = a1[i] - a0[i];
      }

      /* Calculate T0 and T1. */
      calc_T( T0, a0 );
      calc_T( T1, a1 );

      /* Calculate R. */
      calc_R( R01, a0, a01 );

      /* Calculate T01. */
      dq_cr_inv( Tinv, T0 );
      dq_op_mul( T01, T1, Tinv );

      if (dq_ch_cmpV( T01, R01, 1e-10 ) != 0) {
         fprintf( stderr, "T01 != R01! (iteration %d)\n", j );

         /* Spam. */
         printf( "T1:\n" );
         dq_print_vert( T1 );
         printf( "T0:\n" );
         dq_print_vert( T0 );
         printf( "Tinv:\n" );
         dq_print_vert( Tinv );

         /* Now we compare. */
         printf( "T01:\n" );
         dq_print_vert( T01 );
         printf( "R01:\n" );
         dq_print_vert( R01 );
         return EXIT_FAILURE;
      }
   }

   printf( "Success!\n" );
   return EXIT_SUCCESS;
}




