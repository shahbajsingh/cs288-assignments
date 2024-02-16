#!/bin/bash

# function to reverse an array
function reverse() {
    local array=("$@") # get the array passed as argument
    local reversed_array=() # declare an empty array to store the reversal

    # iterate over the array in reverse order and append 
    # only file names to the reversed array
    for ((i = ${#array[@]} - 1; i >= 0; i--)); do
        reversed_array+=($(basename "${array[$i]}"))
    done

    # print the reversed array
    echo "${reversed_array[@]}"

}

# check if arguments are passed
if [ $# -eq 0 ]; then
    echo "ERROR: No directory name passed."
    exit 1
fi

# check if directory exists
if [[ ! -d "$1" ]]; then
    echo "ERROR: Directory does not exist."
    exit 1
fi

directory=$1 # get the directory name from the first argument

# use filename expansion to get the entries in the directory
entries=("$directory"/*)

# call reverse function passing the entries array
reversed_entries=$(reverse "${entries[@]}")

# print reversed entries
echo "$reversed_entries"

