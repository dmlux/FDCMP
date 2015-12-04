//
//  main.c
//  fcmp
//
//  Created by Denis-Michael Lux on 28.11.15.
//  Copyright © 2015 Denis-Michael Lux. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>

#include "file_routines.h"
#include "levenshtein.h"
#include "analysis.h"

#if _OPENMP
    #include <omp.h>
#endif


#define min(a,b)   ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })
#define max(a,b)   ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })

int main(int argc, const char** argv)
{
    if (argc < 3)
    {
        printf("[fdcmp] Wrong input! usage: ./fdcmp <directory> <filename> [<limit>]\n");
        return EXIT_SUCCESS;
    }
    
    /*** PREPARATION ***/
    // inidicator containing number of files with given name
    size_t nei = 0;
    
    // the directory and file name
    char* directory = (char*) malloc((strlen(argv[1]) + 1) * sizeof(char));
    char* file_name = (char*) malloc((strlen(argv[2]) + 1) * sizeof(char));
    
    // copy both strings to variables
    strcpy(directory, argv[1]);
    strcpy(file_name, argv[2]);
    
    // get limit from input
    double limit = 0;
    if (argc == 4)
    {
        limit = atof(argv[3]);
    }
    
    // get number of files
    size_t n_files = count_files(directory, file_name);
    
    // abort if too less files should be compared
    if (n_files < 2)
    {
        printf("[fdcmp] Not enough files to compare. No duplicate analysis was done!\n");
        return EXIT_SUCCESS;
    }
    
    // get memory for files and its contents
    file* list = (file*) malloc(n_files * sizeof(file));
    
    // stop time of the tool
    struct timeval  start, end;
    long time_spent_files, time_spent_leven;
    size_t file_cmps = 0;
    
    // get start time
    gettimeofday(&start, NULL);
    
    // fill list with file paths
    fetch_files(list, &nei, directory, file_name);
    
    // get stop time
    gettimeofday(&end, NULL);
    
    // store time
    time_spent_files = (((end.tv_sec - start.tv_sec) * 1000000L + end.tv_usec) - start.tv_usec) / 1e+6;
    
    /*** DUPLICATE ANALYSIS ***/
    printf("\n******************************\n");
    printf("* FDCMP : DUPLICATE ANALYSIS *\n");
    printf("******************************\n\n");
    printf("> PREFIX = %s/\n", directory);
    printf("> FILE   = %s\n", file_name);
    
    if (argc == 4)
    {
        printf("> LIMIT  = %3.2f\n", limit);
    }
    printf("\n");
    
    // create distance object
    edit_dist* levs = (edit_dist*) malloc(n_files * n_files * sizeof(edit_dist));
    
    // create indices
    size_t i, j;
    
    // get start time
    gettimeofday(&start, NULL);
    
    // iterate over full list of files
    #pragma omp parallel for private(i, j) schedule(dynamic) reduction(+:file_cmps) //collapse(2)
    for (i = 0; i < n_files - 1; ++i)
    {
        for (j = i + 1; j < n_files; ++j)
        {
            // get levenshtein distance
            levenshtein(&list[ j ], &list[ i ], &levs[i * n_files + j]);
            
            // increase compare counter
            //#pragma omp atomic
            file_cmps++;
        }
    }
    
    // get stop time
    gettimeofday(&end, NULL);
    
    // store time
    time_spent_leven = (((end.tv_sec - start.tv_sec) * 1000000L + end.tv_usec) - start.tv_usec) / 1e+6;
    
    printf("> READ %zu FILES IN %ldm %lds\n", n_files, (long)(time_spent_files/60), time_spent_files % 60);
    printf("> COMPARED ALL FILES IN %zu COMBINATIONS IN %ldm %lds\n\n", file_cmps, (long)(time_spent_leven/60), time_spent_leven % 60);
    printf("> LIST OF POSSIBLE FILE DUPLICATES:\n\n");
    
    // over all file duplicates
    size_t fdups = 0;
    
    // print percentages
    for (i = 0; i < n_files - 1; ++i)
    {
        // count the number of files that are exceeding the limit
        size_t cnt = 0;
        if (argc == 4)
        {
            // cout files that exceeded given limit
            for (j = i + 1; j < n_files; ++j)
            {
                double covering = (double)levs[i * n_files + j].duplicates / max(list[ j ].chars, list[ i ].chars) * 100;
                if (covering >= limit)
                {
                    cnt++;
                    fdups++;
                }
            }
            
            // skip output if there is no limit exceeded
            if (cnt == 0)
            {
                continue;
            }
        }
        
        // print information
        printf("'%s' (%zu chars):\n", list[ i ].file_name + strlen(directory) + 1, list[ i ].chars);
        for (j = i + 1; j < n_files; ++j)
        {
            // calculate covering
            size_t dups = levs[i * n_files + j].duplicates;
            double covering = (double)levs[i * n_files + j].duplicates / max(list[ j ].chars, list[ i ].chars) * 100;
            
            // format percentage
            char percentage[7];
            snprintf(percentage, 7, "%3.2f", covering);
            percentage[6] = '\0';
            
            // get levenshtein distance
            if (argc == 3)
            {
                printf("   * %6s%% identical to '%s' (%zu chars, %zu duplicate chars)\n", percentage, list[ j ].file_name + strlen(directory) + 1, list[ j ].chars, dups);
                
                // increase file dup counter
                fdups++;
            }
            else
            {
                if (covering >= limit)
                {
                    printf("   * %6s%% identical to '%s' (%zu chars, %zu duplicate chars)\n", percentage, list[ j ].file_name + strlen(directory) + 1, list[ j ].chars, dups);
                }
            }
        }
        printf("\n");
    }
    
    // Check if ouptput list is empty
    if (fdups == 0)
    {
        printf("> NO POSSIBLE FILE DUPLICATES WERE DETECTED.\n\n");
    }
    
    /*** CLEANUP ***/
    for (i = 0; i < n_files; ++i)
    {
        free( list[ i ].content );
        free( list[ i ].file_name );
    }
    
    free( list );
    free( directory );
    free( file_name );
    free( levs );
    
    return EXIT_SUCCESS;
}
