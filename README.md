# FDCMP #

The FDCMP tool can be used to compare the content of all recursive occurrences of a specific file. The congruence of those compares is indicated by a relative value. The value 0% indicates that the files have no congruence. The value 100% indicates that both files are exactly identical. The tool uses several special header files that are only available on Mac OS X and Linux. To compare the files the tool processes all combination of files and uses the levenshtein distance to calculate the congruence of those files. 

### How do I get set up? ###

To compile the project a CMakeLists.txt file is delivered that can be used by cmake. To compile the project to a binary the following commands can be used:

```mkdir ~/FDCMP

 cd ~/FDCMP
 
 cmake <PATH_TO_PROJECT_DIRECTORY>
 
 make```

After those commands, the binary is located in `~/FDCMP/fdcmp`. The application uses the `-fopenmp` flag to make use of multi core systems. For that purpose the gcc Compiler with Version >= 4 should be used to compile the binary. To manually change the C Compiler that cmake uses to compile the the sources, the 

```export CC=<PATH_TO_CUSTOM_GCC>```

command can be used before the `cmake <PATH_TO_PROJECT_DIRECTORY>` command.