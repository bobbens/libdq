

#ifndef _DQ_H
#  define _DQ_H


/**
 * @file dq.h
 */


/**
 * @brief Using \f$C^{+}_{0,3,1}\f$ notation (McArthy).
 *
 *    0  1  2  3  4  5  6  7
 *    1  i  j  k ei ej ek  e
 */
typedef double dq_t[8];

/* Creation. */
/**
 * @brief Creates a pure rotation dual quaternion.
 *
 *    @param[out] out Dual quaternion created.
 *    @param zite Angle to ratet.
 *    @param s Vector to rotate around.
 *    @param c Any point of the vector (to create plucker coordinates).
 */
void dq_cr_rotation( dq_t out, double zita, double s[3], double c[3] );
/**
 * @brief Creates a pure translation dual quaternion.
 *
 *    @param[out] out Dual quaternion created.
 *    @param t Vector to translate by.
 */
void dq_cr_translation( dq_t out, double t[3] );
/**
 * @brief Creates a point dual quaternion.
 *
 *    @param[out] out Dual quaternion created.
 *    @param pos Position of the point.
 * @sa dq_op_f4g
 */
void dq_cr_point( dq_t out, double pos[3] );
/**
 * @brief Copies a dual quaternion.
 *
 *    @param[out] out Dual quaternion created.
 *    @param in Dual quaternion to copy.
 */
void dq_cr_copy( dq_t out, dq_t in );
/**
 * @brief Conjugates a dual quaternion.
 *
 * p+eq = p*+eq*
 *
 *    @param[out] out Dual quaternion created (conjugated).
 *    @param in Dual quaternion to conjugate.
 */
void dq_cr_conj( dq_t out, dq_t in );

/* Operations. */
/**
 * @brief Adds two dual quaternions.
 * 
 *    @param[out] The result of the addition.
 *    @param p First quaternion to add.
 *    @param q Second quaternion to add.
 */
void dq_op_add( dq_t out, dq_t in_p, dq_t in_q );
/**
 * @brief Multiplies to dual quaternions.
 *
 * pq = p * q
 *
 *    @param pq Result of the multiplication.
 *    @param p First quaternion to multiply.
 *    @param q Second quaternion to multiply.
 */
void dq_op_mul( dq_t pq, dq_t p, dq_t q );
/**
 * @brief Clifford conjugation transformation of type f2g (Alba Perez notation).
 *
 * \f[
 * A |----> ABA*
 * \f]
 *
 * This transformation is useful for lines.
 *
 *    @param ABA Result of the transformation.
 *    @param A Dual quaternion representing the transformation.
 *    @param B Dual quaternion being transformated.
 */
void dq_op_f2g( dq_t ABA, dq_t A, dq_t B );
/**
 * @brief Clifford conjugation transformation of type f4g (Alba Perez notation).
 *
 * \f[
 * A |----> AB(a0 - a1 - a2 - a3 + e( a4 + a5 + a6 - a7 )
 * \f]
 *
 * This transformation is useful for points.
 *
 *    @param ABA Result of the transformation.
 *    @param A Dual quaternion representing the transformation.
 *    @param B Dual quaternion being transformated.
 */
void dq_op_f4g( dq_t ABA, dq_t A, dq_t B );

/* Misc. */
/**
 * @brief Compares two dual quaternions.
 *
 *    @param p First dual quaternion to compare.
 *    @param q Second dual quaternion to compare.
 *    @return 0 if they are equal.
 */
int dq_cmp( dq_t p, dq_t q );
/**
 * @brief Prints a quaternion on a single line.
 * 
 *    @param dq Dual quaternion to print.
 */
void dq_print( dq_t dq );
/**
 * @brief Prints a dual quaternion vertically.
 * 
 *    @param dq Dual quaternion to print.
 */
void dq_print_vert( dq_t dq );


#endif /* _DQ_H */


