//
//  analysis.c
//  fcmp
//
//  Created by Denis-Michael Lux on 29.11.15.
//  Copyright © 2015 Denis-Michael Lux. All rights reserved.
//

#include "analysis.h"

#include <math.h>
#include <stdlib.h>

/*!
 * @brief           Calculates the standard deviation of the given
 *                  vector A with size N.
 *
 * @param[in]       A The vector containing discrete values.
 * @param[in]       N The size of the vector A.
 *
 * @return          The standard deviation
 */
double std(const double* A, size_t N)
{
    return sqrt( var(A, N) );
}

/*!
 * @brief           Calculates the variance of the given vector A
 *                  with size N.
 *
 * @param[in]       A The vector containing discrete values.
 * @param[in]       N The size of the vector A.
 *
 * @return          The variance,
 */
double var(const double* A, size_t N)
{
    if (N == 1)
    {
        return 0;
    }
    else if (N < 1)
    {
        return NAN;
    }
    
    // sum and mu value
    double sum  = 0;
    double mu_s = mu(A, N);
    
    // iterate over vector
    size_t i;
    for (i = 0; i < N; ++i)
    {
        sum += (fabs( A[ i ] - mu_s ) / (double)(N - 1)) * fabs( A[ i ] - mu_s );
    }
    
    // return value
    return sum;
}

/*!
 * @brief           Calculates the weighted sum of the given vector
 *                  A with size N.
 *
 * @param[in]       A The vector containing discrete values.
 * @param[in]       N The size of the vector A.
 *
 * @return          The weighted sum
 */
double mu(const double* A, size_t N)
{
    double sum  = 0;
    
    size_t i;
    for (i = 0; i < N; ++i)
    {
        sum += A[ i ] / (double)N;
    }
    
    return sum;
}
