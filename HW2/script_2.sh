#!/bin/bash

if [[ -z "$1" ]]; then # check if parameter passed
    echo "ERROR: No file passed as parameter."
    exit 1
fi

file=$1

# array to store names and ages
names=()
ages=()

# associative array to store names as keys and ages as values
declare -A people

# read file
while IFS= read -r line; do
    # filter lines for people from cities with two or more words
    city=$(echo "$line" | awk -F ', ' '{print $3}')
    if [[ "$city" =~ [[:alpha:]]+[[:space:]]+[[:alpha:]]+ ]]; then
        # extract dob and calculate age for each person
        birth_date=$(echo "$line" | awk -F ', ' '{print $2}')
        current_year=2023
        age=$((current_year - ${birth_date:0:4}))

        # extract full name
        name=$(echo "$line" | awk -F ', ' '{print $1}')

        # store age in people array with name as the key
        people["$name"]=$age
    fi
done < "$file"

# sort and print people by age descending
for i in "${!people[@]}"; do
    echo "$i is ${people[$i]} years old."
done | sort -k3 -r
