# FDCMP #

The FDCMP tool can be used to compare the content of all recursive occurrences of a specific file. The congruence of those compares is indicated by a relative value. The value 0% indicates that the files have no congruence. The value 100% indicates that both files are exactly identical. The tool uses several special header files that are only available on Mac OS X and Linux. To compare the files the tool processes all combination of those occurrences and calculates a score value that is translated into the relative congruence value by use of the levenshtein distance. 

### How do I get set up? ###

To compile the project a CMakeLists.txt file is delivered that can be used by cmake. To compile the project to a binary the following commands can be used:
```
mkdir ~/FDCMP
cd ~/FDCMP
cmake <PATH_TO_PROJECT_DIRECTORY>
make
```
After those commands, the binary is located in `~/FDCMP/fdcmp`. CMake uses the `-fopenmp` flag to compile a multi-threaded version of FDCMP. For that purpose the gcc Compiler with Version >= 4 should be taken by CMake to compile the binary. To manually change the C Compiler the command
```
export CC=<PATH_TO_CUSTOM_GCC>
```
should be entered before the `cmake <PATH_TO_PROJECT_DIRECTORY>` command. If no suitable GCC compiler is installed on the system, the application will be compiled without taking advantage of multiple cores. The execution time of the multi-threaded version can be much shorter compared to the sequential version depending on the system. 

If CMake is not installed on the system the application can be compiled manually. Therefore enter the following commands
```
cd ~/FDCMP
gcc -O3 -o fdcmp -I./include/ src/main.c src/analysis.c src/file_routines.c src/levenshtein.c -lm -fopenmp
```
If you cloned the repository to another directory then change the first of the above two commands to the right directory.

### How should I use the application? ###

The FDCMP tool takes 2 parameters and one optional parameter. To run the application a `directory` for a starting point in the file tree of the system should be entered. In this file tree the application is searching recursivly all occurrences of the given `file`. This file is the second parameter. Optionally a `limit` value can be mentioned. For example the following commands can be executed
```
./fdcmp <PATH_TO_SOME_DIRECTORY> <FILE_NAME>
./fdcmp <PATH_TO_SOME_DIRECTORY> <FILE_NAME> [<LIMIT_AS_DOUBLE_VALUE>]

./fdcmp ~/Desktop/ example.txt
./fdcmp ~/Desktop/ example.txt 90
```
The limit value is used by the application to filter out some output. If no limit was entered to the application the output contains all congruence values for all file combinations, which can be very much if there are a lot of occurences. If a limit value was entered the output contains only those files that congruence value exceeds the limit including the limit value itself.
