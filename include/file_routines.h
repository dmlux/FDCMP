//
//  file_routines.h
//  fcmp
//
//  Created by Denis-Michael Lux on 28.11.15.
//  Copyright © 2015 Denis-Michael Lux. All rights reserved.
//

#ifndef file_routines_h
#define file_routines_h

#include <stdio.h>
#include <stdlib.h>

/*!
 * @brief           An structure for a file
 */
typedef struct file
{
    char* file_name;      //!< The name of the file
    char* content;        //!< The content of the file
    size_t chars;         //!< The number of characters in the file
} file;

/*!
 * @brief           Count the number of files with the given name
 *
 * @param[in]       directory The path where the files are counted recursivly
 * @param[in]       file_name The name of the files that should be counted
 *
 * @return          The number of counted elements.
 */
int count_files(const char* directory, const char* file_name);

/*!
 * @brief           Get a list of paths to the files with the given name
 *
 * @param[in,out]   list The array containing the paths to the files
 * @param[in,out]   nei The next empty element index in list
 * @param[in]       directory The directory where to start the recursive search
 * @param[in]       file_name The name of the file that should be listed
 */
void fetch_files(file* list, size_t* nei, const char* directory, const char* file_name);

#endif /* file_routines_h */
