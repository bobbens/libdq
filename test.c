

#include "dq.h"

#include <stdio.h>
#include <math.h>

#include <sys/time.h>


static int test_translation (void)
{
   int i;
   double p[3], t[3], pf[3];
   dq_t P, T, TP, PF;

   /* Create positions. */
   p[0] = 1.;
   p[1] = 2.;
   p[2] = 3.;
   t[0] = 4.;
   t[1] = 5.;
   t[2] = 6.;
   for (i=0; i<3; i++)
      pf[i] = p[i] + t[i];

   /* Calculate. */
   dq_cr_point( P, p );
   dq_cr_translation( T, t ); 
   dq_op_f4g( TP, T, P );
   dq_cr_point( PF, pf );

   /* Results. */
   if (dq_cmp( PF, TP ) != 0) {
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

   return 0;
}


static int test_rotation (void)
{
   double a, p[3], s[3], c[3], pf[3];
   dq_t P, R, RP, PF;
   double norm;

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
   norm = dq_op_norm( R );
   if (fabs(norm - 1.) > 1e-10) {
      fprintf( stderr, "Rotation dual quaternion R is not a unit dual quaternion (has a norm of %.3f)\n", norm );
      return 1;
   }

   /* Results. */
   if (dq_cmp( PF, RP ) != 0) {
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

   return 0;
}


static int test_full (void)
{
   int i;
   double a, p[3], t[3], s[3], c[3], pf[3];
   dq_t P, R, T, TR, TRP, PF;

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
   pf[0] = -0.5;
   pf[1] = -0.5;
   pf[2] = 1./M_SQRT2;
   for (i=0; i<3; i++)
      pf[i] = pf[i] + t[i];

   /* Calculate. */
   dq_cr_point( P, p ); /* B */
   dq_cr_rotation( R, a, s, c );
   dq_cr_translation( T, t );
   dq_op_mul( TR, T, R ); /* A */
   dq_op_f4g( TRP, R, P );
   dq_op_f4g( TRP, TR, P ); /* ABA* */
   dq_cr_point( PF, pf );

   /* Results. */
   if (dq_cmp( PF, TRP ) != 0) {
      fprintf( stderr, "Rotation+Translation failed!\n" );
      printf( "Point:\n" );
      dq_print_vert( P );
      printf( "Rotation:\n" );
      dq_print_vert( R );
      printf( "Translation:\n" );
      dq_print_vert( T );
      printf( "Rotation+Translation:\n" );
      dq_print_vert( TR );
      printf( "Got:\n" );
      dq_print_vert( TRP );
      printf( "Expected:\n" );
      dq_print_vert( PF );
      return -1;
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
   double n, na, ns;

   N = 1000000;

   /* Create positions. */
   a    = M_PI/2./(double)N;
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
   dq_cr_copy( RR, R );
   gettimeofday( &tstart, NULL );
   for (i=0; i<N; i++)
      dq_op_mul( RR, RR, R );
   gettimeofday( &tend, NULL );
   dq_cr_copy( R, RR );
   dq_op_f4g( RP, R, P ); /* ABA* */
   dq_cr_point( PF, pf );

   /* Display elapsed time. */
   elapsed = ((tend.tv_sec - tstart.tv_sec) * 1000000 + (tend.tv_usec - tstart.tv_usec));
   dt      = ((double)elapsed) / 1e6;
   fprintf( stdout, "Benchmarked %d multiplications in %.3f seconds (%.3e seconds/multiplication).\n", N, dt, dt/(double)N );

   /* Compare result. */
   dq_op_add( PA, PF, P );
   dq_op_sub( PS, PF, P );
   na = dq_op_norm( PA );
   ns = dq_op_norm( PS );
   n = (na > ns) ? ns : na;
   if (n > 1e-10) {
      fprintf( stderr, "Error with %d multiplications is above 1e-10: %.3e\n", N, n );
      return -1;
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
   ret += test_translation();
   ret += test_rotation();
   ret += test_full();
   ret += test_benchmark();

   if (ret == 0) {
      fprintf( stdout, "All tests passed.\n" );
   }

   return ret;
}


