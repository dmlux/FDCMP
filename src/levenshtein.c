//
//  levenshtein.c
//  fcmp
//
//  Created by Denis-Michael Lux on 28.11.15.
//  Copyright © 2015 Denis-Michael Lux. All rights reserved.
//

#include "levenshtein.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#define COST_ID  1
#define COST_R   1

#define min(a,b)   ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })

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
void levenshtein_backtrace(double* dist_table, size_t x_len, size_t y_len, edit_dist* d)
{
    size_t i = x_len, j = y_len;
    
    while (i && j) {
        if(i > 0 && dist_table[(i-1) * y_len + j] + COST_ID == dist_table[i * y_len + j])
        {
            i--;
            d->deletes++;
        }
        else if(j > 0 && dist_table[i * y_len + j-1] + COST_ID == dist_table[i * y_len + j])
        {
            j--;
            d->insertations++;
        }
        else if(i > 0 && j > 0 && dist_table[(i-1) * y_len + j-1] + COST_R == dist_table[i * y_len + j])
        {
            i--; j--;
            d->replacements++;
        }
        else if(i > 0 && j > 0 && dist_table[(i-1) * y_len + j-1] == dist_table[i * y_len + j])
        {
            i--; j--;
            d->duplicates++;
        }
    }
}

/*!
 * @brief           Calculates the minimal edit distance between two
 *                  given strings.
 *
 * @param[in]       x The string that should be transformed to y
 * @param[in]       y The string which is the destination string
 *
 * @return          The distance
 */
void levenshtein(file* x, file* y, edit_dist* d)
{
    // first create table with dynamic values
    double* dist = (double*) malloc((x->chars + 1) * (y->chars + 1) * sizeof(double));
    double d1, d2, d3, d4;
    
    // needed indices
    size_t i, j;
    
    // iterate over x and then over y
    for (i = 0; i <= x->chars; ++i)
    {
        for (j = 0; j <= y->chars; ++j)
        {
            if (!i || !j)
            {
                dist[i * y->chars + j] = (i == 0 ? j : i) * COST_ID;
            }
            else
            {
                // get the best operation
                d1 = (x->content[i-1] == y->content[j-1] ? dist[(i-1) * y->chars + (j-1)] : ULONG_MAX);
                d2 = dist[ i    * y->chars + (j-1)] + COST_ID;
                d3 = dist[(i-1) * y->chars +  j   ] + COST_ID;
                d4 = dist[(i-1) * y->chars + (j-1)] + COST_R ;
                
                // store value in matrix
                dist[i * y->chars + j] = min(min(min(d1, d2), d3), d4);
            }
        }
    }
    
    // get value from distance table and reset distance properties
    d->distance     = dist[(x->chars + 1) * y->chars];
    d->replacements = 0;
    d->insertations = 0;
    d->duplicates   = 0;
    d->deletes      = 0;
    
    // now backtrace to figure out the operations
    levenshtein_backtrace(dist, x->chars, y->chars, d);
    
    // free table
    free( dist );
}
