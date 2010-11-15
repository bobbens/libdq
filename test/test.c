

#include "dq.h"
#include "mat3.h"

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include <sys/time.h>


void rnd_init (void)
{
   srand( 0 );
}

double rnd_double (void)
{
   return ((double)rand() / (double)RAND_MAX);
}


static int test_vector (void)
{
   double v0[3] = { 3., -3., 1. };
   double v1[3] = { 4., 9., 2. };
   double v2[3] = { -12., 12., -4. };
   double u[3];
   double u01[3] = { -15., -2., 39. };
   double u02[3] = { 0., 0., 0. };

   /* First test. */
   vec3_cross( u, v0, v1 );
   if (vec3_cmp( u, u01 )) {
      fprintf( stderr, "Error with cross product!\n" );
      printf("Got: %0.3f, %0.3f, %0.3f", u[0], u[1], u[2]);
      printf("Expected: %0.3f, %0.3f, %0.3f", u01[0], u01[1], u01[2]);
      return -1;
   }

   /* Second test. */
   vec3_cross( u, v0, v2 );
   if (vec3_cmp( u, u02 )) {
      fprintf( stderr, "Error with cross product!\n" );
      printf("Got: %0.3f, %0.3f, %0.3f", u[0], u[1], u[2]);
      printf("Expected: %0.3f, %0.3f, %0.3f", u02[0], u02[1], u02[2]);
      return -1;
   }

   return 0;
}


static int test_matrix (void)
{
   double M[3][3] = { { 1., 3., 1. },
                      { 1., 1., 2. },
                      { 2., 3., 4. } };
   double comp[3][3] = { { 2., 9., -5. },
                         { 0., -2., 1. },
                         { -1., -3., 2. } };
   double eye[3][3] = { { 1., 0., 0. },
                        { 0., 1., 0. },
                        { 0., 0., 1. } };
   double det, inv[3][3], mmul[3][3];

   det = mat3_det( M );
   if (fabs(det+1.) > 1e-10) {
      fprintf( stderr, "Error with determinant, got %.3e expected -1.\n", det );
      return -1;
   }

   mat3_inv( inv, M );
   if (mat3_cmp( inv, comp ) != 0) {
      fprintf( stderr, "Error with matrix inversion!\n" );
      printf( "Got:\n" );
      mat3_print( inv );
      printf( "Expected:\n" );
      mat3_print( comp );
      return -1;
   }

   mat3_mul( mmul, M, inv );
   if (mat3_cmp( mmul, eye ) != 0) {
      fprintf( stderr, "Error with matrix multiplication!\n" );
      printf( "Got:\n" );
      mat3_print( mmul );
      printf( "Expected:\n" );
      mat3_print( eye );
      return -1;
   }

   return 0;
}


static int test_translation (void)
{
   int i, j;
   double p[3], t[3], pf[3];
   dq_t P, T, TP, PF;

   rnd_init();

   for (j=0; j<10000; j++) {
      /* Create positions. */
      p[0] = rnd_double() * 10;
      p[1] = rnd_double() * 10;
      p[2] = rnd_double() * 10;
      t[0] = rnd_double() * 10;
      t[1] = rnd_double() * 10;
      t[2] = rnd_double() * 10;
      for (i=0; i<3; i++)
         pf[i] = p[i] + t[i];

      /* Calculate. */
      dq_cr_point( P, p );
      dq_cr_translation_vector( T, t ); 
      dq_op_f4g( TP, T, P );
      dq_cr_point( PF, pf );

      /* Results. */
      if (dq_ch_cmp( PF, TP ) != 0) {
         fprintf( stderr, "Translation failed!\n" );
         printf( "Point:\n" );
         dq_print_vert( P );
         printf( "Translation:\n" );
         dq_print_vert( T );
         printf( "Got:\n" );
         dq_print_vert( TP );
         printf( "Expected:\n" );
         dq_print_vert( PF );
         return -1;
      }
   }

   return 0;
}


static int test_rotation (void)
{
   int i;
   double a, p[3], s[3], c[3], pf[3], t[3] = { 0., 0., 0. };
   dq_t P, R, RP, PF, RR, Rinv;

   /* Create positions. */
   a    = M_PI/2.;
   p[0] = 1.;
   p[1] = 1.;
   p[2] = 1.;
   s[0] = 0.;
   s[1] = 0.;
   s[2] = 1.;
   c[0] = 0.;
   c[1] = 0.;
   c[2] = 0.;
   pf[0] = -1.;
   pf[1] = 1.;
   pf[2] = 1.;

   /* Calculate. */
   dq_cr_point( P, p ); /* B */
   dq_cr_rotation( R, a, s, c ); /* A */
   dq_op_f4g( RP, R, P ); /* ABA* */
   dq_cr_point( PF, pf );

   /* Check norm. */
   if (!dq_ch_unit( R )) {
      fprintf( stderr, "Rotation dual quaternion R is not a unit dual quaternion!\n" );
      dq_print_vert( R );
      return 1;
   }

   /* Results. */
   if (dq_ch_cmp( PF, RP ) != 0) {
      fprintf( stderr, "Rotation failed!\n" );
      printf( "Point:\n" );
      dq_print_vert( P );
      printf( "Rotation:\n" );
      dq_print_vert( R );
      printf( "Got:\n" );
      dq_print_vert( RP );
      printf( "Expected:\n" );
      dq_print_vert( PF );
      return -1;
   }

   /* Some number crunching. */
   for (i=0; i<10000; i++) {
      a    = rnd_double() * 2. * M_PI;
      s[0] = rnd_double();
      s[1] = rnd_double();
      s[2] = rnd_double();
      vec3_normalize( s );
      c[0] = rnd_double() * 10.;
      c[1] = rnd_double() * 10.;
      c[2] = rnd_double() * 10.;
      dq_cr_rotation( RR, a, s, c );

      a    = rnd_double() * 2. * M_PI;
      s[0] = rnd_double();
      s[1] = rnd_double();
      s[2] = rnd_double();
      vec3_normalize( s );
      c[0] = rnd_double() * 10.;
      c[1] = rnd_double() * 10.;
      c[2] = rnd_double() * 10.;
      dq_cr_rotation( R, a, s, c );

      /* Multiply. */
      dq_op_mul( RR, RR, R );

      /* Check norm. */
      if (!dq_ch_unit( RR )) {
         fprintf( stderr, "Rotation dual quaternion RR is not a unit dual quaternion!\n" );
         dq_print_vert( RR );
         return 1;
      }

      /* Now we'll test inversion. */
      dq_cr_inv( Rinv, RR );
      dq_op_mul( R, RR, Rinv );
      dq_cr_point( P, t );
      if (dq_ch_cmp( P, R ) != 0) {
         fprintf( stderr, "Rotation inversion failed (RR = R1*R2 ; I = RR RR^-1)!\n" );
         printf( "RR:\n" );
         dq_print_vert( RR );
         printf( "Rinvt:\n" );
         dq_print_vert( Rinv );
         printf( "Got:\n" );
         dq_print_vert( R );
         printf( "Expected:\n" );
         dq_print_vert( P );
         return -1;
      }
   }

   return 0;
}


static int test_movement (void)
{
   int i;
   double a, p[3], t[3], s[3], c[3], pf[3];
   dq_t P, R, T, RT, RTP, PF;

   /* Create positions. */
   a    = M_PI/2.;
   p[0] = 0.;
   p[1] = 0.;
   p[2] = 0.;
   s[0] = 1./M_SQRT2;
   s[1] = -1./M_SQRT2;
   s[2] = 0.;
   c[0] = 1.;
   c[1] = 0.;
   c[2] = 0.;
   t[0] = 1.;
   t[1] = 2.;
   t[2] = 3.;
   pf[0] = 0.5;
   pf[1] = 0.5;
   pf[2] = -1./M_SQRT2;
   for (i=0; i<3; i++)
      pf[i] = pf[i] + t[i];

   /* Calculate. */
   dq_cr_point( P, p ); /* B */
   dq_cr_rotation( R, a, s, c );
   dq_cr_translation_vector( T, t );
   dq_op_mul( RT, T, R ); /* A */
   dq_op_f4g( RTP, R, P );
   dq_op_f4g( RTP, RT, P ); /* ABA* */
   dq_cr_point( PF, pf );

   /* Check norm. */
   if (!dq_ch_unit( R )) {
      fprintf( stderr, "Rotation dual quaternion R is not a unit dual quaternion!\n");
      dq_print_vert( R );
      return 1;
   }

   /* Results. */
   if (dq_ch_cmp( PF, RTP ) != 0) {
      fprintf( stderr, "Rotation+Translation failed!\n" );
      printf( "Point:\n" );
      dq_print_vert( P );
      printf( "Rotation:\n" );
      dq_print_vert( R );
      printf( "Translation:\n" );
      dq_print_vert( T );
      printf( "Rotation+Translation:\n" );
      dq_print_vert( RT );
      printf( "Got:\n" );
      dq_print_vert( RTP );
      printf( "Expected:\n" );
      dq_print_vert( PF );
      return -1;
   }

   return 0;
}


static void test_mat_rot( double R[3][3], double a1, double a2, double a3 )
{
   double R1[3][3], R2[3][3], R3[3][3];
   double c1, s1, c2, s2, c3, s3;

   s1 = sin(a1);
   s2 = sin(a2);
   s3 = sin(a3);
   c1 = cos(a1);
   c2 = cos(a2);
   c3 = cos(a3);

   memset( R1, 0, sizeof(double)*3*3 );
   memset( R2, 0, sizeof(double)*3*3 );
   memset( R3, 0, sizeof(double)*3*3 );

   R1[0][0] =  1.;
   R1[1][1] =  c1;
   R1[1][2] = -s1;
   R1[2][1] =  s1;
   R1[2][2] =  c1;

   R2[0][0] =  c2;
   R2[0][2] =  s2;
   R2[1][1] =  1.;
   R2[2][0] = -s2;
   R2[2][2] =  c2;

   R3[0][0] =  c3;
   R3[0][1] = -s3;
   R3[1][0] =  s3;
   R3[1][1] =  c3;
   R3[2][2] =  1.;

   mat3_mul( R, R1, R2 );
   mat3_mul( R, R,  R3 );
}


static int test_homo (void)
{
   dq_t E, P, Q, PF;
   double R[3][3];
   double d[3] = { 1., 5., 3. };
   double p[3] = { 7., 5., 6. };
   double pf[3];
   double det;
   double a1, a2, a3;
   int i;

   /* Make function deterministic. */
   rnd_init();

   for (i=0; i<10000; i++) {
      /* Parameters. */
      a1 = rnd_double() * 2. * M_PI;
      a2 = rnd_double() * 2. * M_PI;
      a3 = rnd_double() * 2. * M_PI;

      /* Create rotation matrix. */
      test_mat_rot( R, a1, a2, a3 );

      /* Test determinant. */
      det = mat3_det( R );
      if (fabs(det-1.) > DQ_PRECISION) {
         fprintf( stderr, "Rotation matrix determinant test failed (got %.3e, expected 1.)!\n", det );
         return -1;
      }

      /* Calculations with vector math. */
      mat3_mul_vec( pf, R, p );
      pf[0] += d[0];
      pf[1] += d[1];
      pf[2] += d[2];
      dq_cr_point( PF, pf );

      /* Calculations with quaternions. */
      dq_cr_homo( Q, R, d );
      dq_cr_point( P, p );
      dq_op_f4g( E, Q, P );

      /* Comparison. */
      if (dq_ch_cmp( PF, E ) != 0) {
         fprintf( stderr, "Homogeneous matrix test failed!\n" );
         printf( "Got:\n" );
         dq_print_vert( E );
         printf( "Expected:\n" );
         dq_print_vert( PF );
         return -1;
      }
   }
   return 0;
}


static int test_scara (void)
{
   int i;
   double s1[3] = { 0., 0., 1. };
   double c1[3] = { 0., 0., 0. };
   double s2[3] = { 0., 0., 1. };
   double c2[3] = { 300., 0., 0. };
   double s3[3] = { 0., 0., 1. };
   double c3[3] = { 0., -650., 0. };
   double s4[3] = { 0., 0., -1. };
   double a1, a2, a3, t4;
   dq_t S1, S2, S3, S4;
   dq_t S1234;
   dq_t St;

   /* Make function deterministic. */
   rnd_init();

   /* Example taken form Alba Perez.
    *
    * SCARA robot Epson E2L65
    */
   for (i=0; i<10000; i++) {
      /* Parameters. */
      a1 = rnd_double() * 2. * M_PI;
      a2 = rnd_double() * 2. * M_PI;
      a3 = rnd_double() * 2. * M_PI;
      t4 = rnd_double() * 10.;
      /* Create individual rotations. */
      dq_cr_rotation_plucker( S1, a1, s1, c1 );
      dq_cr_rotation( S2, a2, s2, c2 ); /* We spice it up by using non-plucker here. */
      dq_cr_rotation_plucker( S3, a3, s3, c3 );
      dq_cr_translation( S4, t4, s4 );
      /* Create movement. */
      dq_op_mul( S1234, S1, S2 );
      dq_op_mul( S1234, S1234, S3 );
      dq_op_mul( S1234, S1234, S4 );
      /* Calculate movement as per the document. */
      St[0] = cos( (a1+a2+a3)/2. );
      St[1] = 0.;
      St[2] = 0.;
      St[3] = sin( (a1+a2+a3)/2. );
      St[4] = 25.*( 6.*cos((a1-a2-a3)/2.) + 7.*cos((a1+a2-a3)/2.) - 13.*cos((a1+a2+a3)/2.));
      St[5] = 25.*( 6.*sin((a1-a2-a3)/2.) + 7.*sin((a1+a2-a3)/2.) - 13.*sin((a1+a2+a3)/2.));
      St[6] = -t4/2.*cos((a1+a2+a3)/2.);
      St[7] =  t4/2.*sin((a1+a2+a3)/2.);
      /* Compare with real movement. */
      if (dq_ch_cmp( S1234, St ) != 0) {
         fprintf( stderr, "Scara test failed!\n" );
         printf( "Got:\n" );
         dq_print_vert( S1234 );
         printf( "Expected:\n" );
         dq_print_vert( St );
         return -1;
      }
   }
   return 0;
}


static int test_benchmark (void)
{
   int i, N;
   double a, p[3], s[3], c[3], pf[3];
   dq_t P, R, RR, RP, PF, PS, PA;
   struct timeval tstart, tend;
   unsigned long elapsed;
   double dt;
   double nra, nda, nrs, nds;

   N     = 1000000;

   /* Create positions. */
   a     = M_PI/2./(double)N;
   p[0]  = 1.;
   p[1]  = 1.;
   p[2]  = 1.;
   s[0]  = 0.;
   s[1]  = 0.;
   s[2]  = 1.;
   c[0]  = 0.;
   c[1]  = 0.;
   c[2]  = 0.;
   pf[0] = -1.;
   pf[1] = 1.;
   pf[2] = 1.;

   /* Calculate. */
   dq_cr_point( P, p ); /* B */
   dq_cr_rotation( R, a, s, c ); /* A */
   dq_cr_copy( RR, R );
   gettimeofday( &tstart, NULL );
   for (i=0; i<N; i++)
      dq_op_mul( RR, RR, R );
   gettimeofday( &tend, NULL );
   dq_cr_copy( R, RR );
   dq_op_f4g( RP, R, P ); /* ABA* */
   dq_cr_point( PF, pf );

   /* Check norm. */
   if (!dq_ch_unit( R )) {
      fprintf( stderr, "Rotation dual quaternion R is not a unit dual quaternion!\n" );
      dq_print_vert( R );
      return 1;
   }

   /* Display elapsed time. */
   elapsed = ((tend.tv_sec - tstart.tv_sec) * 1000000 + (tend.tv_usec - tstart.tv_usec));
   dt      = ((double)elapsed) / 1e6;
   fprintf( stdout, "Benchmarked %d multiplications in %.3f seconds (%.3e seconds/multiplication).\n", N, dt, dt/(double)N );

   /* Compare result. */
   dq_op_add( PA, PF, P );
   dq_op_sub( PS, PF, P );
   dq_op_norm2( &nra, &nda, PA );
   dq_op_norm2( &nrs, &nds, PA );
   if ((fabs(nra-nrs) > DQ_PRECISION) || (fabs(nda-nds) > DQ_PRECISION)) {
      fprintf( stderr, "Error with %d multiplications is above 1e-10!\n", N );
      return -1;
   }

   return 0;
}


static int test_inversion (void)
{
   int i, r1, r2;
   dq_t Q, Qinv, M, I, M2;
   dq_t R, RIQ, RQI;
   double a, s[3], c[3], z[3] = { 0., 0., 0. };

   rnd_init();

   /* Normalize vector and set up coordinates. */
   for (i=0; i<10000; i++) {
      a    = rnd_double() * 2. * M_PI;
      s[0] = rnd_double();
      s[1] = rnd_double();
      s[2] = rnd_double();
      vec3_normalize( s );
      c[0] = rnd_double() * 10.;
      c[1] = rnd_double() * 10.;
      c[2] = rnd_double() * 10.;

      /* Create the dual quaternions. */
      dq_cr_rotation( Q, a, s, c );
      dq_cr_inv( Qinv, Q );
      dq_op_mul( M, Q, Qinv );
      dq_op_mul( M2, Qinv, Q );
      dq_cr_translation_vector( I, z );

      /* Make sure the result is the identity. */
      r1 = dq_ch_cmp( M, I );
      r2 = dq_ch_cmp( M2, I );
      if ((r1 != 0) || (r2 != 0)) {
         fprintf( stderr, "Failed dual quaternion inversion test!\n" );
         printf( "Got:\n" );
         if (r1 != 0)
            dq_print_vert( M );
         else
            dq_print_vert( M2 );
         printf( "Expected:\n" );
         dq_print_vert( I );
         return -1;
      }

      /* Now test commutative property. */
      a    = rnd_double() * 2. * M_PI;
      s[0] = rnd_double();
      s[1] = rnd_double();
      s[2] = rnd_double();
      vec3_normalize( s );
      c[0] = rnd_double() * 10.;
      c[1] = rnd_double() * 10.;
      c[2] = rnd_double() * 10.;

      /* Create the dual quaternions. */
      dq_cr_rotation( R, a, s, c );
      dq_op_mul( RIQ, R, Qinv );
      dq_op_mul( RIQ, RIQ, Q );
      dq_op_mul( RQI, R, Q );
      dq_op_mul( RQI, RQI, Qinv );

      /* M and M2 must be equal. */
      if (dq_ch_cmp( RQI, RIQ ) != 0) {
         fprintf( stderr, "Failed dual quaternion inversion test!\n" );
         printf( "R Qinv Q:\n" );
         dq_print_vert( RIQ );
         printf( "R Q Qinv:\n" );
         dq_print_vert( RQI );
         return -1;
      }
   }

   return 0;
}


static int test_stress( int stress )
{
   int i;
   dq_t Minv, M, I, Hinv, H, P, PF;
   double pf[3];
   double R[3][3], d[3];
   double a1, a2, a3;
   double det;
   double z[3] = { 0., 0., 0. };

   /* Make function deterministic. */
   rnd_init();

   /* Initialize. */
   pf[0] = 1.;
   pf[1] = 2.;
   pf[2] = 3.;
   dq_cr_point( P, pf );
   dq_cr_translation_vector( I, z );

   /* Stress the bugger. */
   for (i=0; i<stress; i++) {
      a1 = rnd_double() * 2. * M_PI;
      a2 = rnd_double() * 2. * M_PI;
      a3 = rnd_double() * 2. * M_PI;

      /* Create rotation matrix. */
      test_mat_rot( R, a1, a2, a3 );
      det = mat3_det( R );
      if (fabs(det-1.) > 1e-10) {
         fprintf( stderr, "Error with determinant, got %.3e expected 1.\n", det );
         return -1;
      }

      /* Create translation. */
      d[0] = rnd_double() * 10.;
      d[1] = rnd_double() * 10.;
      d[2] = rnd_double() * 10.;

      /* Rotate and translate with homogeneous matrix. */
      mat3_mul_vec( pf, R, pf );
      vec3_add( pf, pf, d );

      /* Create the dual quaternion rotation and run. */
      dq_cr_homo( H, R, d );
      dq_op_f4g( P, H, P );

      /* Compare results. */
      dq_cr_point( PF, pf );
      if (dq_ch_cmpV( PF, P, 1e-6 ) != 0) {
         fprintf( stderr, "Stress test failed at %d/%d!\n", i, stress );
         printf( "Got:\n" );
         dq_print_vert( P );
         printf( "Expected:\n" );
         dq_print_vert( PF );
         printf( "R:\n" );
         mat3_print( R );
         printf( "d:\n" );
         vec3_print( d );
         return -1;
      }

      /* Test inversion. */
      dq_cr_inv( Hinv, H );
      dq_op_mul( M, H, Hinv );
      dq_op_mul( Minv, Hinv, H );
      /* Make sure the result is the identity. */
      if (dq_ch_cmp( M, I ) != 0) {
         fprintf( stderr, "Failed dual quaternion inversion test!\n" );
         printf( "Got:\n" );
         dq_print_vert( M );
         printf( "Expected:\n" );
         dq_print_vert( I );
         return -1;
      }
      if (dq_ch_cmp( Minv, I ) != 0) {
         fprintf( stderr, "Failed dual quaternion inversion test!\n" );
         printf( "Got:\n" );
         dq_print_vert( Minv );
         printf( "Expected:\n" );
         dq_print_vert( I );
         return -1;
      }

      /* We must copy over to avoid problems with double accuracy deviation. */
      dq_cr_point( P, pf );
   }

   return 0;
}


int main( int argc, char *argv[] )
{
   int ret;

   /* Warnings. */
   (void) argc;
   (void) argv;

   ret  = 0;
   ret += !!test_vector();
   ret += !!test_matrix();
   ret += !!test_translation();
   ret += !!test_rotation();
   ret += !!test_movement();
   ret += !!test_homo();
   ret += !!test_scara();
   ret += !!test_inversion();
   ret += !!test_benchmark();
   ret += !!test_stress( 500000 );

   if (ret == 0) {
      fprintf( stdout, "All tests passed.\n" );
   }

   return ret;
}


