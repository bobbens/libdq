

#include "dq_vec3.h"

#include <stdio.h>
#include <math.h>
#include <string.h>
#ifdef DQ_CHECK
#include <assert.h>
#endif /* DQ_CHECK */

#include "dq.h"


double vec3_dot( const double u[3], const double v[3] )
{
   return u[0]*v[0] + u[1]*v[1] + u[2]*v[2];
}


void vec3_cross( double o[3], const double u[3], const double v[3] )
{
   double t[3];
   t[0] =  u[1]*v[2] - u[2]*v[1];
   t[1] = -u[0]*v[2] + u[2]*v[0];
   t[2] =  u[0]*v[1] - u[1]*v[0];
   memcpy( o, t, sizeof(double)*3 );
}


void vec3_add( double o[3], const double u[3], const double v[3] )
{
   int i;
   for (i=0; i<3; i++)
      o[i] = u[i] + v[i];
}


void vec3_sub( double o[3], const double u[3], const double v[3] )
{
   int i;
   for (i=0; i<3; i++)
      o[i] = u[i] - v[i];
}


void vec3_sign( double v[3] )
{
   int i;
   for (i=0; i<3; i++)
      v[i] = -v[i];
}


double vec3_norm( const double v[3] )
{
   return sqrt( vec3_dot( v, v ) );
}


void vec3_normalize( double v[3] )
{
   double n = vec3_norm( v );

#ifdef DQ_CHECK
   assert( fabs(n) > DQ_PRECISION );
#endif /* DQ_CHECK */

   v[0] /= n;
   v[1] /= n;
   v[2] /= n;
}


double vec3_distance( const double u[3], const double v[3] )
{
   double t[3];
   vec3_sub( t, u, v );
   return vec3_norm( t );
}


int vec3_cmpV( const double u[3], const double v[3], double precision )
{
   int ret, i;
   ret = 0;
   for (i=0; i<3; i++)
      if (fabs(u[i]-v[i]) > precision)
         ret = ret + 1;
   return ret;
}


int vec3_cmp( const double u[3], const double v[3] )
{
   return vec3_cmpV( u, v, DQ_PRECISION );
}


void vec3_print( const double v[3] )
{
   printf( "   %.3f, %.3f, %.3f\n", v[0], v[1], v[2] );
}


