#!/bin/bash

# array to store command counts
declare -a counts

# array to store alphabet
letters=({a..z})

# loop through alphabet
for letter in {a..z}; do
	# get the count of commands starting with the current letter
	count=$(ls /bin/$letter* 2>/dev/null | wc -l) # redirect stderr to /dev/null

    # store the count in the array
    counts+=($count)
done

# print the table with letters and counts
for ((i = 0; i < ${#letters[@]}; i++)); do
	echo "${letters[$i]} ${counts[$i]}"
done