

#ifndef _DQ_HOMO_H
#  define _DQ_HOMO_H


/**
 * @file dq_homo.h
 *
 * @brief File containing functions related to 4 by 4 homogeneous matrix.
 */

/**
 * @defgroup homo Auxiliary Homogeneous Matrix Functions
 * @brief Set of auxiliary functions to manipulate homogeneous matrix.
 *
 * Due to the fact that the bottom row of each homogeneous matrix is always
 *  the same, we can simplify the amount of data needed to be stored.
 *
 * The full homogeneous matrix is:
 *
 * \f[
 *    H = \left( \begin{array}{cc}
 *       R & d \\
 *       0 & 1
 *    \end{array} \right)
 * \f]
 *
 * Where R is a 3x3 rotation matrix and d is a 3d translation vector. The
 *  bottom row is always \f$ ( 0, 0, 0, 1 ) \f$.
 *
 * We simplify this to a 3x4 matrix by eliminating the bottom row and
 *  thus we end up with:
 *
 * \f[
 *    H = \left( \begin{array}{cc}
 *       R & t
 *    \end{array} \right)
 * \f]
 */
/** @{ */
/**
 * @brief Creates a homogeneous matrix by joining a 3x3 rotation matrix and a 3d translation vector.
 *
 *    @param[out] H Homogeneous matrix formed by R and d.
 *    @param[in] R 3x3 Rotation matrix.
 *    @param[in] d 3d translation vector.
 */
void homo_cr_join( double H[3][4], double R[3][3], double d[3] );
/**
 * @brief Multiplies two homogeneous matrix.
 *
 * \f[
 *    O = A * B
 * \f]
 *
 *    @param[out] O Resulting homogeneous matrix of the multiplication.
 *    @param[in] A First homogeneous matrix to operate on.
 *    @param[in] B Second homogeneous matrix to operate on.
 */
void homo_op_mul( double O[3][4], double A[3][4], double B[3][4] );
/**
 * @brief Splits a homogeneous matrix into a 3x3 rotation matrix and a 3d translation vector.
 *
 *    @param[out] R 3x3 rotation matrix extracted from the homogeneous matrix.
 *    @param[out] d 3d translation vector extracted from the homogeneous matrix.
 *    @param[in] H Homogeneous matrix to split.
 */
void homo_op_split( double R[3][3], double d[3], double H[3][4] );
/**
 * @brief Multiplies a homogeneous matrix by a 4d vector.
 *
 * To multiply a normal 3d vector, add 1 as it's 4th component.
 *
 *    @param[out] o Resulting 4d vector of the multiplication.
 *    @param[in] H Homogeneous matrix to multiply against.
 *    @param[in] v 4d vector to multiply.
 */
void homo_op_mul_vec( double o[4], double H[3][4], const double v[4] );
/**
 * @brief Prints a homogeneous matrix on screen.
 *
 *    @param[in] H Homogeneous matrix to print on screen.
 */
void homo_print( double H[3][4] );
/** @} */


#endif /* _DQ_HOMO_H */

