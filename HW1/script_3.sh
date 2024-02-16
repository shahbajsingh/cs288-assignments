#!/bin/bash

# check if arguments are passed
if [ $# -eq 0 ]; then # check if no arguments are passed
    echo "ERROR: No parameters passed."
    exit 1
fi

# store parameters into array
params=("$@")

# get number of parameters
numParams=${#params[@]}

# perform insert sort
for ((i = 1; i < numParams; i++)); do # loop through array
    j=$i 
    # while j > 0 and params[j - 1] > params[j]
    while ((j > 0 && params[j - 1] > params[j])); do 
        # swap elements
        temp=${params[j - 1]} # store params[j - 1] in temp
        params[j - 1]=${params[j]} # move params[j] to params[j - 1]
        params[j]=$temp # move temp forward to params[j]
        j=$((j - 1)) # decrement j
    done
done

# print the sorted list
for ((i = 0; i < numParams; i++)); do
    echo -n "${params[$i]} "
done

echo
