

#include "dq.h"

#include <stdio.h>
#include <math.h>


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

   /* Create positions. */
   a    = M_PI;
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
   dq_cr_point( P, p );
   dq_cr_rotation( R, a, s, c );
   dq_op_f4g( RP, R, P );
   dq_cr_point( PF, pf );

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
   return 0;
}


int main( int argc, char *argv[] )
{
   /* Warnings. */
   (void) argc;
   (void) argv;

   test_translation();
   test_rotation();
   test_full();

#if 0
   /* Create rotation. */
   s[0] = s[2] = 0.;
   s[2] = 1.;
   c[0] = c[1] = c[2] = 0.;
   dq_cr_rotation( R, M_PI_2, s, c );
   printf("R:\n");
   dq_print_vert( R );

   /* Create translation. */
   t[0] = 1.;
   t[1] = -1.;
   t[2] = 0.;
   dq_cr_translation( T, t ); 
   printf("T:\n");
   dq_print_vert( T );

   /* Create transformation. */
   dq_op_mul( Q, R, T );

   /* Create point. */
   d[0] = 1.;
   d[1] = d[2] = 0.;
   dq_cr_point( P, d );
   printf("P:\n");
   dq_print_vert( P );

   /* Rotate. */
   dq_op_f2g( RP, R, P );
   printf("RP:\n");
   dq_print_vert( RP );

   /* Translate. */
   dq_op_f2g( TRP, T, RP );
   printf("TRP:\n");
   dq_print_vert( TRP );

   /* Translate. */
   dq_op_f2g( TP, T, P );
   printf("TP:\n");
   dq_print_vert( TP );

   /* Rotate. */
   dq_op_f2g( RTP, R, TP );
   printf("TRP:\n");
   dq_print_vert( RTP );

   /* Transform. */
   dq_op_f2g( PQ, Q, P );

   /* Display. */
   printf("PQ:\n");
   dq_print_vert( PQ );
#endif

   return 0;
}


