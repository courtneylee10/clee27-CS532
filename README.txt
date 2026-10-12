CS 532 Homework 2: Search Program

Files

search.c — C source code

Makefile — builds the program

README.txt — compilation, execution, and testing instructions

Compilation

To compile the program using the Makefile, run:

make

Alternatively, compile the program directly using GCC:

gcc -Wall -o search search.c

The -Wall option enables common compiler warnings, and -o search names the executable search.

Execution

Run the program in the current directory:

./search

Specify a starting directory:

./search ../programs

Command-Line Options

-S — Display file size, permissions, and last access time.

-s SIZE — List entries whose size is less than or equal to the specified number of bytes.

-f PATTERN DEPTH — List matching entries within the specified depth.

-t f — List regular files only.

-t d — List directories only.

Examples

./search -S ../programs
./search -s 1024 ../programs
./search -f jpg 1 ../programs
./search -t f ../programs
./search -t d ../programs
./search -S -s 1024 ../programs

Testing

Run ./search to test traversal of the current directory.

Run ./search ../programs to test a specified starting directory.

Run ./search -S to check file metadata output.

Run ./search -s 1024 to test the file-size filter.

Run ./search -f jpg 1 to test filename matching and depth.

Run ./search -t f and ./search -t d to test the graduate-level file-type filters.

Combine options to verify that all specified filters are applied together.

Test symbolic links, if available, to verify that the link name and target are displayed.

Implementation Notes

The program uses opendir(), readdir(), lstat(), and readlink() for file and directory operations. It uses function pointers to apply filters and getopt() to process command-line options.

