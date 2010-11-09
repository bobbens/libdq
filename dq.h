

#ifndef _DQ_H
#  define _DQ_H


/**
 * @mainpage libdq doxygen documentation
 * @author Edgar Simo <bobbens@gmail.com>
 * @version 0.1
 * @date November 2010
 *
 @verbatim
    Copyright 2010 Edgar Simo

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 @endverbatim
 *
 * This is a library for using and manipulating dual quaternions. Dual quaternions are useful for describing rigid body movements in screw theory.
 *
 * The naming scheme used is more or less consistant and the following:
 *  - Capital letters are for dual quaternions, they shall be denoted with a "hat" in the documentation as such: \f$\widehat{Q}\f$.
 *  - Lowercase will be used for vectors or simple quaternions. In the case of simple quaternions they shall have a "hat" as such: \f$\widehat{q}\f$.
 *  - Individual members of quaternions or dual quaternions are lowercase and will be indicated with subscripted index as such: \f$a_0\f$.
 *
 * For functions:
 *  - Input dual quaternions should be called Q if there is one input or P and Q if there are two.
 *  - Output dual quaternions should be called O or a combination of the input quaternions in the case it is multiplying or transforming like for example PQ.
 *
 * For notation and quaternion definition please refer to the documentation for the dual quaternion type @ref dq_t .
 *
 * @sa dq_t
 * @sa creation
 * @sa operations
 * @sa misc
 */


/**
 * @file dq.h
 * @brief The include for the libdq dual quaternion library.
 */


/**
 * @brief A representation of a dual quaternion.
 * 
 * Uses \f$C^{+}_{0,3,1}\f$ notation (McArthy).
 *
 * \f[
 * \{ 1, e_{23}, e_{31}, e_{12}, e_{41}, e_{42}, e_{43}, e_{1234} \} = \{ 1, i, j, k, i\epsilon, j\epsilon, k\epsilon, \epsilon \}
 * \f]
 *
 * \f[
 * Q = (q_0 + q_1 i + q_2 j + q_3 k ) + \epsilon ( q_7 + q_4 i + q_5 j + q_6 k ) = \widehat{q} + \epsilon \widehat{q}^0
 * \f]
 *
 * In vertical notation we have the following:
 *
 * \f[
 * Q = \left\{\begin{array}{c}
 *   e_{23} \\
 *   e_{31} \\
 *   e_{12} \\
 *   1
 * \end{array}\right\} + \left\{\begin{array}{c}
 *   e_{41} \\
 *   e_{42} \\
 *   e_{43} \\
 *   e_{1234}
 * \end{array}\right\} = \left\{\begin{array}{c}
 *   i \\
 *   j \\
 *   k \\
 *   1
 * \end{array}\right\} + \epsilon \left\{\begin{array}{c}
 *   i \\
 *   j \\
 *   k \\
 *   1
 * \end{array}\right\}
 * \f]
 */
typedef double dq_t[8];

/**
 * @defgroup creation Dual Quaternion Creation Functions
 * @brief Set of functions to create dual quaternions.
 */
/* @{ */
/**
 * @brief Creates a pure rotation dual quaternion.
 *
 *    @param[out] O Dual quaternion created.
 *    @param[in] zita Angle to rotate.
 *    @param[in] s Vector to rotate around (normalized).
 *    @param[in] c Any point of the vector (to create plucker coordinates).
 */
void dq_cr_rotation( dq_t O, double zita, const double s[3], const double c[3] );
/**
 * @brief Creates a pure rotation dual quaternion using plucker coordinates.
 *
 *    @param[out] O Dual quaternion created.
 *    @param[in] zita Angle to rotate.
 *    @param[in] s Vector to rotate around (normalized).
 *    @param[in] s0 Moment of the vector.
 */
void dq_cr_rotation_plucker( dq_t O, double zita, const double s[3], const double s0[3] );
/**
 * @brief Creates a pure rotation dual quaternion from a rotation matrix.
 *
 *    @param[out] O Dual quaternion created.
 *    @param[in] R 3x3 Rotation matrix.
 */
void dq_cr_rotation_matrix( dq_t O, double R[3][3] );
/**
 * @brief Creates a pure translation dual quaternion.
 *
 *    @param[out] O Dual quaternion created.
 *    @param[in] t Translation amount.
 *    @param[in] s Translation vector (normalized).
 */
void dq_cr_translation( dq_t O, double t, const double s[3] );
/**
 * @brief Creates a pure translation dual quaternion from a traslation vector.
 *
 *    @param[out] O Dual quaternion created.
 *    @param[in] t Traslation vector.
 */
 void dq_cr_translation_vector( dq_t O, const double t[3] );
/**
 * @brief Creates a point dual quaternion.
 *
 *    @param[out] O Dual quaternion created.
 *    @param[in] pos Position of the point.
 * @sa dq_op_f4g
 */
void dq_cr_point( dq_t O, const double pos[3] );
/**
 * @brief Creates a line dual quaternion.
 *
 *    @param[out] O Dual quaternion created.
 *    @param[in] s Direction vector of the line.
 *    @param[in] c A point of the line.
 * @sa dq_op_f2g
 */
void dq_cr_line( dq_t O, const double s[3], const double c[3] );
/**
 * @brief Creates a line dual quaternion from plucker coordinates.
 *
 *    @param[out] O Dual quaternion created.
 *    @param[in] s Direction vector of the line.
 *    @param[in] c The momento f the line.
 * @sa dq_op_f2g
 */
void dq_cr_line_plucker( dq_t O, const double s[3], const double c[3] );
/**
 * @brief Creates a dual quaternion from a homogeneous transformation matrix.
 *
 *    @param[out] O Dual quaternion created.
 *    @param[in] R Rotation matrix.
 *    @param[in] d Translation vector.
 */
void dq_cr_homo( dq_t O, double R[3][3], const double d[3] );
/**
 * @brief Copies a dual quaternion.
 *
 *    @param[out] O Dual quaternion created.
 *    @param[in] Q Dual quaternion to copy.
 */
void dq_cr_copy( dq_t O, dq_t Q );
/**
 * @brief Conjugates a dual quaternion.
 *
 * \f[
 * \widehat{O} = \widehat{Q}^* = \widehat{q}^* + \epsilon \widehat{q}^*
 * \f]
 *
 *    @param[out] O Dual quaternion created (conjugated).
 *    @param[in] Q Dual quaternion to conjugate.
 */
void dq_cr_conj( dq_t O, dq_t Q );
/**
 * @brief Inverts a dual quaternion.
 *
 * \f[
 * \widehat{O} = \widehat{Q}^{-1} = \frac{ \widehat{Q}^* }{ \| \widehat{Q} \|^2 }
 * \f]
 *
 * First we multiply the dual quaternion by it's conjugate:
 *
 * \f[
 * \widehat{Q} \widehat{Q}^* = \| \widehat{Q} \| = \\
 *    (q_0 q_0 + q_1 q_1 + q_2 q_2 + q_3 q_3) + \epsilon 2 (q_0 q_7 + q_1 q_4 + q_2 q_5 + q_3 q_6)
 * \f]
 *
 * To get rid of the \f$ \epsilon \f$ term we multiply by \f$ (q_0 q_0 + q_1 q_1 + q_2 q_2 + q_3 q_3) - \epsilon 2 (q_0 q_7 + q_1 q_4 + q_2 q_5 + q_3 q_6) \f$.
 *
 * \f[
 * ((q_0 q_0 + q_1 q_1 + q_2 q_2 + q_3 q_3) + \epsilon 2 (q_0 q_7 + q_1 q_4 + q_2 q_5 + q_3 q_6)) * \\
 *    ((q_0 q_0 + q_1 q_1 + q_2 q_2 + q_3 q_3) - \epsilon 2 (q_0 q_7 + q_1 q_4 + q_2 q_5 + q_3 q_6)) =
 *    (q_0 q_0 + q_1 q_1 + q_2 q_2 + q_3 q_3)^2
 * \f]
 *
 * Due to the fact that the first quaternion represents the rotation, it can be proven that it's equal to the identity:
 *
 * \f[
 * \| \widehat{q} \| = \| ( \cos \left ( \frac{\theta}{2} \right ) + \sin \left ( \frac{\theta}{2} \right ) s) \| =
 *    ( \sin^2 \left ( \frac{\theta}{2} \right ) + \cos^2 \left ( \frac{\theta}{2} \right ) ) = 1 = q_0 q_0 + q_1 q_1 + q_2 q_2 + q_3 q_3
 * \f]
 *
 * Therefore with the multiplications we've done we have gotten what we wanted. If we analyze the multiplication we did we see it was:
 *
 * \f[
 * \widehat{Q}^* ((q_0 q_0 + q_1 q_1 + q_2 q_2 + q_3 q_3) - \epsilon 2 (q_0 q_7 + q_1 q_4 + q_2 q_5 + q_3 q_6)) = \
 *    (q_0 q_0 + q_1 q_1 + q_2 q_2 + q_3 q_3 )((q_0 - q_1 i - q_2 j - q_3 k) + \epsilon( q_7 - q_4 i - q_5 j - q_7 k)) - 2 (q_0 q_7 + q_1 q_4 + q_2 q_5 + q_3 q_6)(q_7 + q_4 i - q_5 j - q_6 k)
 * \f]
 *
 *    @param[out] O Dual quaternion created (inverted).
 *    @param[in] Q Dual quaternion to invert.
 */
void dq_cr_inv( dq_t O, dq_t Q );
/* @} */

/**
 * @defgroup operations Dual Quaternion Operations
 * @brief Functions for operation on dual quaternions.
 */
/* @{ */
/**
 * @brief Gets the norm of a dual quaternion.
 *
 * \f[
 * \| \widehat{Q} \| = \sqrt{\widehat{Q} \widehat{Q}^*}
 * \f]
 *
 *    @param[in] Q Dual quaternion to get norm of.
 *    @return The norm of the dual quaternion.
 */
double dq_op_norm( dq_t Q );
/**
 * @brief Gets the square of the norm of a dual quaternion.
 *
 * \f[
 * \| \widehat{Q} \|^2 = \widehat{Q} \widehat{Q}^*
 * \f]
 *
 * The square of the norm is a dual number.
 *
 * \f[
 * \widehat{Q} \widehat{Q}^* = (\widehat{q_0} + Q)(\widehat{q_0} - Q) = \widehat{q_0}^2 + Q \cdot Q
 * \f]
 *
 * If we denote the dual quaternion as \f$ \widehat{Q} = \widehat{q} + \epsilon \widehat{q}' \f$ with
 *  \f$ \widehat{q} \f$ being the pure real quaternion and \f$ \widehat{q}' \f$ being the pure dual
 *  quaternion we can use the following notation to describe the product:
 *
 * \f[
 * \widehat{Q} \widehat{Q}^* = \widehat{q} \widehat{q}^* + \\
 *    \epsilon (\widehat{q} \widehat{q}'^* + \widehat{q}' \widehat{q}^*) = \\
 *    (q_0 q_0 + q_1 q_1 + q_2 q_2 + q_3 q_3) + \epsilon 2 (q_0 q_7 + q_1 q_4 + q_2 q_5 + q_3 q_6)
 * \f]
 *
 * @note A norm of 1 indicates that the dual quaternion is a unit dual quaternion.
 *
 *    @param[out] real The real part of the norm of the dual quaternion.
 *    @param[out] dual The dual port of the norm of the dual quaternion.
 *    @param[in] Q Dual quaternion to get square of norm of.
 * @sa dq_cr_conj
 */
void dq_op_norm2( double *real, double *dual, dq_t Q );
/**
 * @brief Adds two dual quaternions.
 *
 * \f[
 * \widehat{O} = \widehat{P} - \widehat{Q}
 * \f]
 * 
 *    @param[out] O The result of the addition.
 *    @param[in] P First quaternion to add.
 *    @param[in] Q Second quaternion to add.
 */
void dq_op_add( dq_t O, dq_t P, dq_t Q );
/**
 * @brief Subtracts two dual quaternions.
 *
 * \f[
 * \widehat{O} = \widehat{P} - \widehat{Q}
 * \f]
 * 
 *    @param[out] O The result of the subtraction.
 *    @param[in] P Dual quaternion to subtract from.
 *    @param[in] Q Dual quaternion to subtract.
 */
void dq_op_sub( dq_t O, dq_t P, dq_t Q );
/**
 * @brief Multiplies to dual quaternions.
 *
 * \f[
 * \widehat{PQ} = \widehat{P} \widehat{Q}
 * \f]
 *
 *    @param[out] PQ Result of the multiplication.
 *    @param[in] P First dual quaternion to multiply.
 *    @param[in] Q Second dual quaternion to multiply.
 */
void dq_op_mul( dq_t PQ, dq_t P, dq_t Q );
/**
 * @brief Clifford conjugation transformation of type \f$f_{1g}\f$ (Alba Perez notation).
 *
 * \f[
 * A : B \longmapsto ABA
 * \f]
 *
 *    @param[out] ABA Result of the transformation.
 *    @param[in] A Dual quaternion representing the transformation.
 *    @param[in] B Dual quaternion being transformated.
 */
void dq_op_f1g( dq_t ABA, dq_t A, dq_t B );
/**
 * @brief Clifford conjugation transformation of type \f$f_{2g}\f$ (Alba Perez notation).
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
 * @brief Clifford conjugation transformation of type \f$f_{3g}\f$ (Alba Perez notation).
 *
 * \f[
 * A : B \longmapsto AB(a_0 + a - \epsilon ( a^0 + a_7 ))
 * \f]
 *
 *    @param[out] ABA Result of the transformation.
 *    @param[in] A Dual quaternion representing the transformation.
 *    @param[in] B Dual quaternion being transformated.
 */
void dq_op_f3g( dq_t ABA, dq_t A, dq_t B );
/**
 * @brief Clifford conjugation transformation of type \f$f_{4g}\f$ (Alba Perez notation).
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
/* @} */

/**
 * @defgroup misc Dual Quaternion Miscellaneous Functions
 * @brief Assorted functions related to dual quaternions that don't fit elsewhere.
 */
/* @{ */
/**
 * @brief Compares two dual quaternions.
 *
 *    @param[in] P First dual quaternion to compare.
 *    @param[in] Q Second dual quaternion to compare.
 *    @return 0 if they are equal.
 */
int dq_cmp( dq_t P, dq_t Q );
/**
 * @brief Prints a quaternion on a single line.
 * 
 *    @param[in] Q Dual quaternion to print.
 */
void dq_print( dq_t Q );
/**
 * @brief Prints a dual quaternion vertically.
 * 
 *    @param[in] Q Dual quaternion to print.
 */
void dq_print_vert( dq_t Q );
/* @} */


#endif /* _DQ_H */


