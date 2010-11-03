

#ifndef _DQ_H
#  define _DQ_H


/**
 * @mainpage libdq doxygen documentation
 * @author Edgar Simo <bobbens@gmail.com>
 * @version 0.1
 * @date 2010
 *
 * This is a library for using and manipulating dual quaternions. Dual quaternions are useful for describing rigid body movements in screw theory.
 *
 * @sa dq.h
 */


/**
 * @file dq.h
 * @brief The include for the libdq dual quaternion library.
 */


/**
 * @brief Using \f$C^{+}_{0,3,1}\f$ notation (McArthy).
 *
 * \f[
 * \{ 1, e_{23}, e_{31}, e_{12}, e_{41}, e_{42}, e_{43}, e_{1234} \} = \{ 1, i, j, k, i\epsilon, j\epsilon, k\epsilon, \epsilon \}
 * \f]
 *
 * \f[
 * Q = (q_0 + q_1 i + q_2 j + q_3 k ) + \epsilon ( q_7 + q_4 i + q_5 j + q_6 k ) = \widehat{q} + \epsilon \widehat{q}^0
 * \f]
 */
typedef double dq_t[8];

/* Creation. */
/**
 * @brief Creates a pure rotation dual quaternion.
 *
 *    @param[out] out Dual quaternion created.
 *    @param[in] zita Angle to ratet.
 *    @param[in] s Vector to rotate around.
 *    @param[in] c Any point of the vector (to create plucker coordinates).
 */
void dq_cr_rotation( dq_t out, double zita, const double s[3], const double c[3] );
/**
 * @brief Creates a pure translation dual quaternion.
 *
 *    @param[out] out Dual quaternion created.
 *    @param[in] t Vector to translate by.
 */
void dq_cr_translation( dq_t out, const double t[3] );
/**
 * @brief Creates a point dual quaternion.
 *
 *    @param[out] out Dual quaternion created.
 *    @param[in] pos Position of the point.
 * @sa dq_op_f4g
 */
void dq_cr_point( dq_t out, const double pos[3] );
/**
 * @brief Copies a dual quaternion.
 *
 *    @param[out] out Dual quaternion created.
 *    @param[in] in Dual quaternion to copy.
 */
void dq_cr_copy( dq_t out, dq_t in );
/**
 * @brief Conjugates a dual quaternion.
 *
 * \f[
 * \widehat{out} = \widehat{p}^* + \epsilon \widehat{q}^*
 * \f]
 *
 *    @param[out] out Dual quaternion created (conjugated).
 *    @param[in] in Dual quaternion to conjugate.
 */
void dq_cr_conj( dq_t out, dq_t in );

/* Operations. */
/**
 * @brief Gets the norm of a dual quaternion.
 *
 * \f[
 * \| \widehat{q} \| = \sqrt{\widehat{q} \widehat{q}^*}
 * \f]
 *
 *    @param[in] dq Dual quaternion to get norm of.
 *    @return The norm of the dual quaternion.
 */
double dq_op_norm( dq_t dq );
/**
 * @brief Adds two dual quaternions.
 *
 * \f[
 * \widehat{out} = \widehat{p} + \widehat{q}
 * \f]
 * 
 *    @param[out] out The result of the addition.
 *    @param[in] p First quaternion to add.
 *    @param[in] q Second quaternion to add.
 */
void dq_op_add( dq_t out, dq_t p, dq_t q );
/**
 * @brief Subtracts two dual quaternions.
 *
 * \f[
 * \widehat{out} = \widehat{p} - \widehat{q}
 * \f]
 * 
 *    @param[out] out The result of the subtraction.
 *    @param[in] p Dual quaternion to subtract from.
 *    @param[in] q Dual quaternion to subtract.
 */
void dq_op_sub( dq_t out, dq_t p, dq_t q );
/**
 * @brief Multiplies to dual quaternions.
 *
 * \f[
 * \widehat{pq} = \widehat{p} \widehat{q}
 * \f]
 *
 *    @param[out] pq Result of the multiplication.
 *    @param[in] p First dual quaternion to multiply.
 *    @param[in] q Second dual quaternion to multiply.
 */
void dq_op_mul( dq_t pq, dq_t p, dq_t q );
/**
 * @brief Clifford conjugation transformation of type f2g (Alba Perez notation).
 *
 * \f[
 * A : B \longmapsto ABA^*
 * \f]
 *
 * This transformation is useful for lines.
 *
 *    @param[out] ABA Result of the transformation.
 *    @param[in] A Dual quaternion representing the transformation.
 *    @param[in] B Dual quaternion being transformated.
 */
void dq_op_f2g( dq_t ABA, dq_t A, dq_t B );
/**
 * @brief Clifford conjugation transformation of type f4g (Alba Perez notation).
 *
 * \f[
 * A : B \longmapsto AB(a_0 - a + \epsilon ( a^0 - a_7 ))
 * \f]
 *
 * This transformation is useful for points.
 *
 *    @param[out] ABA Result of the transformation.
 *    @param[in] A Dual quaternion representing the transformation.
 *    @param[in] B Dual quaternion being transformated.
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


