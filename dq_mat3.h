

#ifndef _MAT3_H
#  define _MAT3_H


/**
 * @file dq_mat3.h
 *
 * @brief File containing functions related to 3 by 3 matrix.
 */

/**
 * @defgroup mat3 Auxiliary 3x3 Matrix Functions
 * @brief Set of auxiliary functions to manipulate 3x3 matrix.
 */
/** @{ */
/**
 * @brief Creates an identity 3x3 matrix.
 *
 * \f[
 *    M = \left( \begin{array}{ccc}
 *       1 & 0 & 0 \\
 *       0 & 1 & 0 \\
 *       0 & 0 & 1
 *    \end{array} \right)
 * \f]
 *
 *    @param[out] M An identiy matrix.
 */
void mat3_eye( double M[3][3] );
/**
 * @brief Gets the determinant of a 3x3 3x3 3x3 matrix.
 *
 * \f[
 *    o = \| M \|
 * \f]
 *
 *    @param[in] M 3x3 Matrix to get the determinant of.
 *    @return The determinant of the 3x3 matrix.
 */
double mat3_det( double M[3][3] );
/**
 * @brief Adds two 3x3 matrix.
 *
 * \f[
 *    out = A + B
 * \f]
 *
 *    @param[out] out Result of the 3x3 matrix addition.
 *    @param[in] A First matrix to operate on.
 *    @param[in] B Second matrix to operate on.
 */
void mat3_add( double out[3][3], double A[3][3], double B[3][3] );
/**
 * @brief Subtracts two 3x3 matrix.
 *
 * \f[
 *    out = A - B
 * \f]
 *
 *    @param[out] out Result of the 3x3 matrix subtraction.
 *    @param[in] A First matrix to operate on.
 *    @param[in] B Second matrix to operate on.
 */
void mat3_sub( double out[3][3], double A[3][3], double B[3][3] );
/**
 * @brief Inverts a 3x3 matrix.
 *
 * \f[
 *    out = in^{-1}
 * \f]
 *
 *    @param[out] out Inversion of the matrix.
 *    @param[in] in Matrix to invert.
 */
void mat3_inv( double out[3][3], double in[3][3] );
/**
 * @brief Multiplies two 3x3 matrix.
 *
 * \f[
 *    AB = A * B
 * \f]
 *
 *    @param[out] AB Result of the matrix multiplication.
 *    @param[in] A First matrix to operate on.
 *    @param[in] B Second matrix to operate on.
 */
void mat3_mul( double AB[3][3], double A[3][3], double B[3][3] );
/**
 * @brief Multiplies a 3x3 matrix by a vector.
 *
 * \f[
 *    out = M * v
 * \f]
 *
 *    @param[out] out Result of the multiplication.
 *    @param[in] M Matrix to operate on.
 *    @param[in] v Vector to operate on.
 */
void mat3_mul_vec( double out[3], double M[3][3], const double v[3] );
/**
 * @brief Compares two 3x3 matrix.
 *
 *    @param[in] A First matrix to compare.
 *    @param[in] B Second matrix to compare.
 *    @return 0 if they are the same.
 */
int mat3_cmp( double A[3][3], double B[3][3] );
/**
 * @brief Compares two 3x3 matrix with custom precision.
 *
 *    @param[in] A First matrix to compare.
 *    @param[in] B Second matrix to compare.
 *    @param[in] precision Precision to use when comparing.
 *    @return 0 if they are the same.
 */
int mat3_cmpV( double A[3][3], double B[3][3], double precision );
/**
 * @brief Prints the value of a matrix on screen.
 *
 *    @param[in] M Matrix to print.
 */
void mat3_print( double M[3][3] );
/** @} */


#endif /* _MAT3_H */

