//
//  levenshtein.h
//  fcmp
//
//  Created by Denis-Michael Lux on 28.11.15.
//  Copyright © 2015 Denis-Michael Lux. All rights reserved.
//

#ifndef levenshtein_h
#define levenshtein_h

#include <stdlib.h>
#include "file_routines.h"

/*!
 * @brief           An structure for levenshtein distance results
 */
typedef struct edit_dist
{
    double distance;      //!< The distance
    size_t chars;         //!< Number of original chars
    size_t duplicates;    //!< Number of duplicates
    size_t replacements;  //!< Number of replacements
    size_t insertations;  //!< Number of isertations
    size_t deletes;       //!< Number of deletes
} edit_dist;

/*!
 * @brief           Extracts the changes that had been done to 
 *                  transform string x to string y from the table
 *                  created by dynamic programming.
 *
 * @param[in]       dist_table The table with changing values.
 * @param[in]       x_len The length of string x.
 * @param[in]       y_len The length of string y.
 * @param[in]       d The edit_dist object to store information.
 */
void levenshtein_backtrace(double* dist_table, size_t x_len, size_t y_len, edit_dist* d);

/*!
 * @brief           Calculates the minimal edit distance between two
 *                  given strings.
 *
 * @param[in]       x The string that should be transformed to y
 * @param[in]       y The string which is the destination string
 * @param[in]       dist The edit_dist object to store information.
 *
 * @return          The distance
 */
void levenshtein(file* x, file* y, edit_dist* dist);

#endif /* levenshtein_h */
