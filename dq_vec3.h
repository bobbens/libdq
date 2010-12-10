

#ifndef _DQ_VEC3_H
#  define _DQ_VEC3_H


double vec3_dot( const double u[3], const double v[3] );
void vec3_cross( double o[3], const double u[3], const double v[3] );
void vec3_add( double o[3], const double u[3], const double v[3] );
void vec3_sub( double o[3], const double u[3], const double v[3] );
double vec3_norm( const double v[3] );
void vec3_normalize( double v[3] );
int vec3_cmp( const double u[3], const double v[3] );
void vec3_print( const double v[3] );


#endif /* _DQ_VEC3_H */

