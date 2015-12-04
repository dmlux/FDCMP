//
//  analysis.h
//  fcmp
//
//  Created by Denis-Michael Lux on 29.11.15.
//  Copyright © 2015 Denis-Michael Lux. All rights reserved.
//

#ifndef analysis_h
#define analysis_h

#include <stdlib.h>
#include "levenshtein.h"

/*!
 * @brief           Structure that holds two files that are duplicates
 *                  and their matching score
 */
typedef struct duplicate
{
    char* first;    //!< the file that is matched agains second
    char* second;   //!< The file that was matched against
    double score;   //!< The matching score
} duplicate;

/*!
 * @brief           Calculates the standard deviation of the given 
 *                  vector A with size N.
 *
 * @param[in]       A The vector containing discrete values.
 * @param[in]       N The size of the vector A.
 *
 * @return          The standard deviation
 */
double std(const double* A, size_t N);

/*!
 * @brief           Calculates the variance of the given vector A
 *                  with size N.
 *
 * @param[in]       A The vector containing discrete values.
 * @param[in]       N The size of the vector A.
 *
 * @return          The variance,
 */
double var(const double* A, size_t N);

/*!
 * @brief           Calculates the weighted sum of the given vector
 *                  A with size N.
 *
 * @param[in]       A The vector containing discrete values.
 * @param[in]       N The size of the vector A.
 *
 * @return          The weighted sum
 */
double mu(const double* A, size_t N);

#endif /* analysis_h */
