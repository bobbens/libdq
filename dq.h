

#ifndef _DQ_H
#  define _DQ_H


/**
 * Using C^{+}_{0,3,1} notation (McArthy).
 *
 *    0  1  2  3  4  5  6  7
 *    1  i  j  k  e ei ej ek
 */
typedef double dq_t[8];

/* Creation. */
void dq_cr_rotation( dq_t out, double zita, double s[3], double c[3] );
void dq_cr_translation( dq_t out, double t[3] );
void dq_cr_point( dq_t out, double pos[3] );
void dq_cr_copy( dq_t out, dq_t in );

/* Operations. */
void dq_op_add( dq_t out, dq_t in_p, dq_t in_q );
void dq_op_mul( dq_t pq, dq_t p, dq_t q );
void dq_op_f2g( dq_t ABA, dq_t A, dq_t B );
void dq_op_f4g( dq_t ABA, dq_t A, dq_t B );

/* Misc. */
int dq_cmp( dq_t p, dq_t q );
void dq_print( dq_t dq );
void dq_print_vert( dq_t dq );


#endif /* _DQ_H */


