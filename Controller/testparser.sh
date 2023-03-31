#!/bin/bash

# Check that the .txt file with all the commands is given in argument
if [ $# -ne 1 ]
then
    echo "Usage: $0 <command_file>"
    exit 1
fi

# Check if the given .txt file exists and is readable
if [ ! -r $1 ]
then
    echo "Error: $1 is not readable"
    exit 1
fi

# Remove result.txt if it exists
if [ -f result.txt ]
then
    rm result.txt
fi

# Remove test_parser if it exists
if [ -f test_parser ]
then
    rm test_parser
fi

# Clean and compile test_parser.c in test_parser
make clean
make test_parser

# Read line by line and execute the parsing function on it
./prompt
while read line
do
    echo "tested command : \"" $line "\""
done < $1
