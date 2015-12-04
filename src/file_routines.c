//
//  file_routines.c
//  fcmp
//
//  Created by Denis-Michael Lux on 28.11.15.
//  Copyright © 2015 Denis-Michael Lux. All rights reserved.
//

#include "file_routines.h"

#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#define BUFFER_SIZE 2048

/*!
 * @brief           Count the number of files with the given name
 *
 * @param[in]       directory The path where the files are counted recursivly
 * @param[in]       file_name The name of the files that should be counted
 *
 * @return          The number of counted elements.
 */
int count_files(const char* directory, const char* file_name)
{
    // create Variables
    DIR* dir;
    struct dirent *entry;
    
    // The counter for the number of files
    int file_cnt = 0;
    
    // Check if directory is valid
    if (!(dir = opendir(directory)) || !(entry = readdir(dir)))
    {
        printf("[fdcmp] Could not open directory '%s'\n", directory);
        return file_cnt;
    }
    
    // iterate over all files and directories
    do {
        
        // If the current entry is a directory
        if (entry->d_type == DT_DIR)
        {
            // create new directory string by concatenate the directory and the entry
            char path[BUFFER_SIZE];
            int len = snprintf(path, BUFFER_SIZE - 1, "%s/%s", directory, entry->d_name);
            path[len] = '\0';
            
            // Skip the '.' and '..' directory
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }
            
            // increase counter recursively
            file_cnt += count_files(path, file_name);
        }
        
        // Otherwise the entry is a file
        // if the file has the same name then print path
        else if (strcmp(file_name, entry->d_name) == 0)
        {
            file_cnt++;
        }
        
    } while((entry = readdir(dir)));
    
    closedir(dir);
    return file_cnt;
}

/*!
 * @brief           Get a list of paths to the files with the given name
 *
 * @param[in,out]   list The array containing the paths to the files
 * @param[in,out]   nei The next empty element index in list
 * @param[in]       directory The directory where to start the recursive search
 * @param[in]       file_name The name of the file that should be listed
 */
void fetch_files(file* list, size_t* nei, const char* directory, const char* file_name)
{
    // create Variables
    DIR* dir;
    struct dirent *entry;
    
    // Check if directory is valid
    if (!(dir = opendir(directory)) || !(entry = readdir(dir)))
    {
        printf("[fdcmp] Could not open directory '%s'\n", directory);
        return;
    }
    
    // iterate over all files and directories
    do {
        
        // If the current entry is a directory
        if (entry->d_type == DT_DIR)
        {
            // create new directory string by concatenate the directory and the entry
            char path[BUFFER_SIZE];
            int len = snprintf(path, BUFFER_SIZE - 1, "%s/%s", directory, entry->d_name);
            path[len] = '\0';
            
            // Skip the '.' and '..' directory
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }
            
            // increase counter recursively
            fetch_files(list, nei, path, file_name);
        }
        
        // Otherwise the entry is a file
        // if the file has the same name then print path
        else if (strcmp(file_name, entry->d_name) == 0)
        {
            // build the path string
            char* path = (char*) malloc(BUFFER_SIZE * sizeof(char));
            int len = snprintf(path, BUFFER_SIZE - 1, "%s/%s", directory, entry->d_name);
            path[len] = '\0';
            
            // store path string in the list entry
            list[ *nei ].file_name = path;
            
            // next read content
            // open filestream with current file
            FILE* input_file = fopen(path, "rb");
            
            // set indicator to EOF position
            fseek(input_file, 0, SEEK_END);
            
            // get length of file
            list[ *nei ].chars = ftell(input_file);
            
            // change indicator to position 0
            rewind(input_file);
            
            // get dynamic memory for both files
            list[ *nei ].content = (char*) malloc((list[ *nei ].chars + 1) * sizeof(char));
            
            // read file content to fc1
            fread(list[ *nei ].content, sizeof(char), list[ *nei ].chars, input_file);
            
            // mark file end with termination symbol
            list[ *nei ].content[ list[ *nei ].chars ] = '\0';
            
            // close file stream
            fclose(input_file);
            
            (*nei)++;
        }
        
    } while((entry = readdir(dir)));
    
    closedir(dir);
    return;
}

