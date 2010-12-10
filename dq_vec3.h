

#ifndef _DQ_VEC3_H
#  define _DQ_VEC3_H


/**
 * @file dq_vec3.h
 *
 * @brief File containing functions related to 3d vectors.
 */

/**
 * @defgroup vec3 Auxiliary 3d Vector Functions
 * @brief Set of auxiliary functions to manipulate 3d vectors.
 */
/** @{ */
/**
 * @brief Does the dot product of two 3d vectors.
 *
 * \f[
 *    o = u \cdot v
 * \f]
 *
 *    @param[in] u First 3d vector to operate on.
 *    @param[in] v Second 3d vector to operate on.
 *    @return The dot product of u.v.
 */
double vec3_dot( const double u[3], const double v[3] );
/**
 * @brief Does the cross product of two 3d vectors.
 *
 * \f[
 *    o = u \times v
 * \f]
 *
 *    @param[out] o The cross product of u and v.
 *    @param[in] u First 3d vector to operate on.
 *    @param[in] v Second 3d vector to operate on.
 */
void vec3_cross( double o[3], const double u[3], const double v[3] );
/**
 * @brief Adds two 3d vectors.
 *
 * \f[
 *    o = u + v
 * \f]
 *
 *    @param[out] o Result of the addition.
 *    @param[in] u First 3d vector to operate on.
 *    @param[in] v Second 3d vector to operate on.
 */
void vec3_add( double o[3], const double u[3], const double v[3] );
/**
 * @brief Subtracts two 3d vectors.
 *
 * \f[
 *    o = u - v
 * \f]
 *
 *    @param[out] o Result of the subtraction.
 *    @param[in] u First 3d vector to operate on.
 *    @param[in] v Second 3d vector to operate on.
 */
void vec3_sub( double o[3], const double u[3], const double v[3] );
/**
 * @brief Gets the norm of a 3d vector.
 *
 * \f[
 *    o = \| v \|
 * \f]
 *
 *    @param[in] v Vector to get norm of.
 *    @return The norm of the 3d vector.
 */
double vec3_norm( const double v[3] );
/**
 * @brief Normalizes a 3d vector.
 *
 * \f[
 *    v = \frac{v}{\| v \|}
 * \f]
 *
 *    @param v Vector to normalize.
 */
void vec3_normalize( double v[3] );
/**
 * @brief Compares two 3d vectors.
 *
 *    @param[in] u First 3d vector to compare.
 *    @param[in] v Second 3d vector to compare.
 *    @return 0 if they are the same.
 */
int vec3_cmp( const double u[3], const double v[3] );
/**
 * @brief Prints a 3d vector on screen.
 *
 *    @param[in] v Vector to print.
 */
void vec3_print( const double v[3] );
/** @} */


#endif /* _DQ_VEC3_H */

