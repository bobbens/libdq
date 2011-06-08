

#ifndef _DQ_H
#  define _DQ_H


/**
 * @mainpage libdq doxygen documentation
 * @author Edgar Simo-Serra <bobbens@gmail.com>
 * @version 2.0
 * @date February 2011
 *
 * @section License
 *
 @verbatim
    Copyright 2010, 2011 Edgar Simo-Serra

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
 *
 * @section Overview
 *
 * This is a library for using and manipulating unit dual quaternions. Unit
 * dual quaternions are useful for describing rigid body movements using screw
 * theory.
 *
 * If you use this library please reference it.
 *
 *
 * @section Notation
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
 * For notation and quaternion definition please refer to the documentation for
 * the dual quaternion type @ref dq_t .
 *
 *
 * @section Usage
 *
 * To use dual quaternion library you need to include it as <dq.h>. When
 * linking you should pass -ldq. A simple example would be:
 *
 * @code
 * #include <dq/dq.h>
 *
 * int main( int argc, char *argv[] )
 * {
 *    dq_t Q;
 *    double t[3] = { 1.0, 2.0, 3.0 };
 *
 *    dq_cr_point( Q, t );
 *    dq_print_vert( Q );
 *
 *    return 0;
 * }
 * @endcode
 *
 * The example would just create a point dual quaternion and display it. To
 * compile you would have to use:
 *
 * @code
 * $ gcc -ldq dq_test.c -o dq_test
 * @endcode
 *
 * Auxiliary functions are also provided to help manipulate common data
 * structures when working with dual quaternions.
 *
 *
 * @section Changelog
 *
 * - Version 2.0, April 2011
 *    - Lua bindings
 * - Version 1.5, April 2011
 *    - Install headers into /usr/include/dq by default instead of prefixing with dq_
 *    - Updated documentation
 *    - Make docs now places final documentation in /
 * - Version 1.4, February 2011
 *    - Fixed major issue in dual quaternion conjugation
 *    - Added dq_op_extract
 *    - Added mat3_solve
 *    - Added vec3_sign
 *    - Added vec3_distance
 *    - Minor doxygen improvements
 * - Version 1.3, December 2010
 *    - Cleaned up and documented the auxiliary function files so they can be used.
 * - Version 1.2, December 2010
 *    - Added dq_op_sign to change sign of a dual quaternion.
 *    - dq_ch_cmp and dq_ch_cmpV now take into account the fact it could be with a different sign.
 * - Version 1.1, November 2010
 *    - Fixed dual quaternion multiplication
 *    - Implemented more strict unit tests
 *    - Misc fixes
 * - Version 1.0, November 2010
 *    - Initial Revision
 *
 *
 * @section References
 *
 * - E. Simo-Serra, Chapter 4 of Kinematic Model of the Hand using Computer Vision (Degree Thesis). BarcelonaTECH (UPC), April 2011.
 * - J. M. Selig. Geometric Fundamentals of Robotics (Monographs in Computer Science). Springer, 2nd edition, November 2004.
 * - J. M. McCarthy, Introduction to theoretical kinematics, MIT Press, Cambridge, MA, 1990 
 * - A. Perez, Kinematics of Robots (unpublished as of this writing)
 *
 *
 * @section Citation
 *
 * There is no real convention for citing software, the following is a proposal.
 *
 * @code
 * @MISC{esimolibdq,
 *    author = {Edgar Simo-serra},
 *    title = {libdq: {D}ual {Q}uaternion {L}ibrary},
 *    year = {2011},
 *    howpublished = {\url{https://github.com/bobbens/libdq}},
 * }
 * @endcode
 *
 *
 * @section Acknowledgements
 *
 * A big thanks to Alba Perez for having the patience to deal with my repetitive
 *  boring dual quaternion questions and for lending me her notes.
 *
 * @sa dq_t
 * @sa creation
 * @sa operations
 * @sa check
 * @sa misc
 */


/**
 * @file dq.h
 * @brief The include for the libdq dual quaternion library.
 */


#define DQ_PRECISION    1e-10 /**< Precision to use when comparing doubles. */


/**
 * @brief A representation of a dual quaternion.
 * 
 * Dual quaternions are elements of the Clifford even subalgebra
 * \f$C^{+}_{0,3,1}\f$. There are many notations for dual quaternions. This
 * library uses the basis used by McArthy which is the same as Selig with minor
 * rearrangements,
 *
 * \f[
 * \{ 1, e_{23}, e_{31}, e_{12}, e_{41}, e_{42}, e_{43}, e_{1234} \} = \\
 * \{ 1, i, j, k, i\epsilon, j\epsilon, k\epsilon, \epsilon \}
 * \f]
 *
 * This allows us to write a dual quaternion as,
 *
 * \f[
 * \widehat{Q} = (q_0 + q_1 i + q_2 j + q_3 k ) + \epsilon ( q_7 + q_4 i + q_5 j + q_6 k ) = \widehat{q} + \epsilon \widehat{q}^0
 * \f]
 *
 * Using vertical notation we would have the following,
 *
 * \f[
 * \widehat{Q} = \left\{\begin{array}{c}
 *   e_{23} \\
 *   e_{31} \\
 *   e_{12} \\
 *   1
 * \end{array}\right\} + \left\{\begin{array}{c}
 *   e_{41} \\
 *   e_{42} \\
 *   e_{43} \\
 *   e_{1234}
 * \end{array}\right\}
 * = \left\{\begin{array}{c}
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
 *
 * In order for the dual quaternion to be able to represent spatial
 * displacements it must be a unit dual quaternion and thus comply with the
 * following restrictions,
 *
 * \f{align}{
 * \widehat{q}\widehat{q}^0 = 1 \nonumber \\
 * \widehat{q}\cdot\widehat{q}^0 = 0 \nonumber
 * \f}
 *
 * It is important to note that unit dual quaternions double cover the special
 * euclidean group \f$SE(3)\f$. This means that \f$\widehat{Q}\f$ and
 * \f$-\widehat{Q}\f$ represent the same spatial displacement.
 */
typedef double dq_t[8];


/**
 * @defgroup creation Dual Quaternion Creation Functions
 * @brief Set of functions to create dual quaternions.
 */
/** @{ */
/**
 * @brief Creates a pure rotation dual quaternion.
 *
 *    @param[out] O Dual quaternion created.
 *    @param[in] theta Angle to rotate.
 *    @param[in] s Vector to rotate around (normalized).
 *    @param[in] c Any point of the vector (to create plucker coordinates).
 * @sa dq_cr_rotation_plucker
 * @sa dq_cr_rotation_matrix
 */
void dq_cr_rotation( dq_t O, double theta, const double s[3], const double c[3] );
/**
 * @brief Creates a pure rotation dual quaternion using plucker coordinates.
 *
 *    @param[out] O Dual quaternion created.
 *    @param[in] theta Angle to rotate.
 *    @param[in] s Vector to rotate around (normalized).
 *    @param[in] s0 Moment of the vector.
 * @sa dq_cr_rotation
 * @sa dq_cr_rotation_matrix
 */
void dq_cr_rotation_plucker( dq_t O, double theta, const double s[3], const double s0[3] );
/**
 * @brief Creates a pure rotation dual quaternion from a rotation matrix.
 *
 *    @param[out] O Dual quaternion created.
 *    @param[in] R 3x3 Rotation matrix.
 * @sa dq_cr_rotation
 * @sa dq_cr_rotation_plucker
 */
void dq_cr_rotation_matrix( dq_t O, double R[3][3] );
/**
 * @brief Creates a pure translation dual quaternion.
 *
 *    @param[out] O Dual quaternion created.
 *    @param[in] t Translation amount.
 *    @param[in] s Translation vector (normalized).
 * @sa dq_cr_translation_vector
 */
void dq_cr_translation( dq_t O, double t, const double s[3] );
/**
 * @brief Creates a pure translation dual quaternion from a traslation vector.
 *
 *    @param[out] O Dual quaternion created.
 *    @param[in] t Traslation vector.
 * @sa dq_cr_translation
 */
void dq_cr_translation_vector( dq_t O, const double t[3] );
/**
 * @brief Creates a dual quaternion representing a point.
 *
 *    @param[out] O Dual quaternion created.
 *    @param[in] pos Position of the point.
 * @sa dq_op_f4g
 */
void dq_cr_point( dq_t O, const double pos[3] );
/**
 * @brief Creates a dual quaternion representing a line.
 *
 *    @param[out] O Dual quaternion created.
 *    @param[in] s Direction vector of the line.
 *    @param[in] c A point of the line.
 * @sa dq_cr_line_plucker
 * @sa dq_op_f2g
 */
void dq_cr_line( dq_t O, const double s[3], const double c[3] );
/**
 * @brief Creates a dual quaternion representing a line from plucker coordinates.
 *
 *    @param[out] O Dual quaternion created.
 *    @param[in] s Direction vector of the line.
 *    @param[in] s0 The momento f the line.
 * @sa dq_cr_line
 * @sa dq_op_f2g
 */
void dq_cr_line_plucker( dq_t O, const double s[3], const double s0[3] );
/**
 * @brief Creates a unit dual quaternion representing a plane.
 *
 *    @param[out] O Dual quaternion created.
 *    @param[in] n Normal of the plane.
 *    @param[in] d Distancefrom the origin to the plane.
 */
void dq_cr_plane( dq_t O, const double n[3], const double d );
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
void dq_cr_copy( dq_t O, const dq_t Q );
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
void dq_cr_conj( dq_t O, const dq_t Q );
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
 * \f{align}{
 * ((q_0 q_0 + q_1 q_1 + q_2 q_2 + q_3 q_3) + \epsilon 2 (q_0 q_7 + q_1 q_4 + q_2 q_5 + q_3 q_6)) \nonumber \\
 *    ((q_0 q_0 + q_1 q_1 + q_2 q_2 + q_3 q_3) - \epsilon 2 (q_0 q_7 + q_1 q_4 + q_2 q_5 + q_3 q_6)) \nonumber \\
 *  = (q_0 q_0 + q_1 q_1 + q_2 q_2 + q_3 q_3)^2 \nonumber
 * \f}
 *
 * Due to the fact that the first quaternion represents the rotation, it can be proven that it's equal to the identity:
 *
 * \f{align}{
 * \| \widehat{q} \| &= \| ( \cos \left ( \frac{\theta}{2} \right ) + \sin \left ( \frac{\theta}{2} \right ) s) \| \nonumber \\
 *    &= ( \sin^2 \left ( \frac{\theta}{2} \right ) + \cos^2 \left ( \frac{\theta}{2} \right ) ) = 1 = q_0 q_0 + q_1 q_1 + q_2 q_2 + q_3 q_3 \nonumber
 * \f}
 *
 * Therefore with the multiplications we've done we have gotten what we wanted. If we analyze the multiplication we did we see it was:
 *
 * \f{align}{
 * & \widehat{Q}^* ((q_0 q_0 + q_1 q_1 + q_2 q_2 + q_3 q_3) - \epsilon 2 (q_0 q_7 + q_1 q_4 + q_2 q_5 + q_3 q_6)) = \nonumber \\
 * &   (q_0 q_0 + q_1 q_1 + q_2 q_2 + q_3 q_3 )((q_0 - q_1 i - q_2 j - q_3 k) + \epsilon( q_7 - q_4 i - q_5 j - q_7 k)) \nonumber \\
 * & - 2 (q_0 q_7 + q_1 q_4 + q_2 q_5 + q_3 q_6)(q_7 + q_4 i - q_5 j - q_6 k) \nonumber
 * \f}
 *
 *    @param[out] O Dual quaternion created (inverted).
 *    @param[in] Q Dual quaternion to invert.
 */
void dq_cr_inv( dq_t O, const dq_t Q );
/** @} */


/**
 * @defgroup operations Dual Quaternion Operations
 * @brief Functions for operation on dual quaternions.
 */
/** @{ */
/**
 * @brief Gets the square of the norm of a dual quaternion.
 *
 * \f[
 * \| \widehat{Q} \|^2 = \widehat{Q} \widehat{Q}^*
 * \f]
 *
 * The square of the norm is a dual number. If we denote \f$ Q \f$ as the vector part of the dual quaternion (all except \f$ q_0 \f$ and \f$ q_7 \f$):
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
void dq_op_norm2( double *real, double *dual, const dq_t Q );
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
 * @sa dq_op_sub
 */
void dq_op_add( dq_t O, const dq_t P, const dq_t Q );
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
 * @sa dq_op_add
 */
void dq_op_sub( dq_t O, const dq_t P, const dq_t Q );
/**
 * @brief Multiplies to dual quaternions.
 *
 * \f[
 * \widehat{PQ} = \widehat{P} \widehat{Q}
 * \f]
 *
 *  The multiplication table used is:
 *
 @verbatim
   Q1*Q2 | Q2.1  Q2.i  Q2.j  Q2.k  Q2.ei  Q2.ej  Q2.ek  Q2.e
   ------+---------------------------------------------------
   Q1.1  |   1     i     j     k    ei     ej     ek      e
   Q1.i  |   i    -1     k    -j    -e     ek     -ej    ei
   Q1.j  |   j    -k    -1     i    -ek    -e     ei     ej
   Q1.k  |   k     j    -i    -1    ej     -ei    -e     ek
   Q1.ei |  ei    -e    ek    -ej    0      0      0      0
   Q1.ej |  ej    -ek   -e    ei     0      0      0      0
   Q1.ek |  ek    ej    -ei   -e     0      0      0      0
   Q1.e  |   e    ei     ej   ek     0      0      0      0
 @endverbatim
 *
 *    @param[out] PQ Result of the multiplication.
 *    @param[in] P First dual quaternion to multiply.
 *    @param[in] Q Second dual quaternion to multiply.
 */
void dq_op_mul( dq_t PQ, const dq_t P, const dq_t Q );
/**
 * @brief Swaps the sign of all the elements in a dual quaternion.
 *
 *    @param[out] P Result of swapping all values of the elements.
 *    @param[in] Q Dual quaternion to swap sign of all elements.
 */
void dq_op_sign( dq_t P, const dq_t Q );
/**
 * @brief Clifford conjugation transformation of type \f$f_{1g}\f$ (Alba Perez notation).
 *
 * \f{eqnarray*}{
 *    f_{1G} : C(V,<,>) &\longrightarrow& C(V,<,>) \\
 *    A : B &\longmapsto& ABA
 * \f}
 *
 *    @param[out] ABA Result of the transformation.
 *    @param[in] A Dual quaternion representing the transformation.
 *    @param[in] B Dual quaternion being transformated.
 * @sa dq_op_f2g
 * @sa dq_op_f3g
 * @sa dq_op_f4g
 */
void dq_op_f1g( dq_t ABA, const dq_t A, const dq_t B );
/**
 * @brief Clifford conjugation transformation of type \f$f_{2g}\f$ (Alba Perez notation).
 *
 * \f{eqnarray*}{
 *    f_{2G} : C(V,<,>) &\longrightarrow& C(V,<,>) \\
 *    A : B &\longmapsto& ABA^*
 * \f}
 *
 * This transformation is useful for lines.
 *
 *    @param[out] ABA Result of the transformation.
 *    @param[in] A Dual quaternion representing the transformation.
 *    @param[in] B Dual quaternion being transformated.
 * @sa dq_op_f1g
 * @sa dq_op_f3g
 * @sa dq_op_f4g
 */
void dq_op_f2g( dq_t ABA, const dq_t A, const dq_t B );
/**
 * @brief Clifford conjugation transformation of type \f$f_{3g}\f$ (Alba Perez notation).
 *
 * \f{eqnarray*}{
 *    f_{3G} : C(V,<,>) &\longrightarrow& C(V,<,>) \\
 *    A : B &\longmapsto& AB(a_0 + a - \epsilon ( a^0 + a_7 ))
 * \f}
 *
 *    @param[out] ABA Result of the transformation.
 *    @param[in] A Dual quaternion representing the transformation.
 *    @param[in] B Dual quaternion being transformated.
 * @sa dq_op_f1g
 * @sa dq_op_f2g
 * @sa dq_op_f4g
 */
void dq_op_f3g( dq_t ABA, const dq_t A, const dq_t B );
/**
 * @brief Clifford conjugation transformation of type \f$f_{4g}\f$ (Alba Perez notation).
 *
 * \f{eqnarray*}{
 *    f_{4G} : C(V,<,>) &\longrightarrow& C(V,<,>) \\
 *    A : B &\longmapsto& AB(a_0 - a + \epsilon ( a^0 - a_7 ))
 * \f}
 *
 * This transformation is useful for points.
 *
 *    @param[out] ABA Result of the transformation.
 *    @param[in] A Dual quaternion representing the transformation.
 *    @param[in] B Dual quaternion being transformated.
 * @sa dq_op_f1g
 * @sa dq_op_f2g
 * @sa dq_op_f3g
 */
void dq_op_f4g( dq_t ABA, const dq_t A, const dq_t B );
/**
 * @brief Extracts the rotation matrix and translation vector assosciated to a dual quaternion.
 *
 *    @param[out] R Rotation matrix.
 *    @param[out] d Translation vector.
 *    @param[in] Q Dual quaternion to extract R and d from.
 */
void dq_op_extract( double R[3][3], double d[3], const dq_t Q );
/** @} */


/**
 * @defgroup check Dual Quaternion Check Functions
 * @brief Assorted functions related to dual quaternions properties that can be checked.
 */
/** @{ */
/**
 * @brief Checks to see if a dual quaternion is a unit quaternion.
 *
 *    @param[in] Q Dual quaternion to check if is a unit quaternion.
 *    @return 1 if is a unit dual quaternion or 0 otherwise.
 */
int dq_ch_unit( const dq_t Q );
/**
 * @brief Checks to see if a point Q is on the plane P.
 *
 *    @param[in] P Plane to check if point is on it.
 *    @param[in] Q Point to check if is on plane P.
 *    @return 1 if point Q is on plane P.
 */
int dq_ch_point_plane( const dq_t P, const dq_t Q );
/**
 * @brief Compares two dual quaternions.
 *
 *    @param[in] P First dual quaternion to compare.
 *    @param[in] Q Second dual quaternion to compare.
 *    @return 0 if they are equal.
 * @sa dq_ch_cmpV
 */
int dq_ch_cmp( const dq_t P, const dq_t Q );
/**
 * @brief Compares two dual quaternions with variable precision.
 *
 *    @param[in] P First dual quaternion to compare.
 *    @param[in] Q Second dual quaternion to compare.
 *    @param[in] precision Precision to use when comparing members of each dual quaternion.
 *    @return 0 if they are equal.
 * @sa dq_ch_cmp
 */
int dq_ch_cmpV( const dq_t P, const dq_t Q, double precision );
/** @} */


/**
 * @defgroup misc Dual Quaternion Miscellaneous Functions
 * @brief Assorted functions related to dual quaternions that don't fit elsewhere.
 */
/** @{ */
/**
 * @brief Prints a quaternion on a single line.
 * 
 *    @param[in] Q Dual quaternion to print.
 * @sa dq_printVert
 */
void dq_print( const dq_t Q );
/**
 * @brief Prints a dual quaternion vertically.
 * 
 *    @param[in] Q Dual quaternion to print.
 * @sa dq_print
 */
void dq_print_vert( const dq_t Q );
/** @} */


#endif /* _DQ_H */


