#!/bin/bash

if [[ -z "$1" ]]; then # check if parameter passed
    echo "ERROR: No file passed as parameter."
    exit 1
fi

file=$1

# array to store dates
declare -a dates

# create temp array to store dates in YYYY-MM-DD format
declare -a temp_dates

# use associative array to store dates in YYYY-MM-DD format as keys and original dates as values
declare -A dates_assoc

# read file line by line
while IFS= read -r line; do
    # extract dates in MM/DD/YYYY, MM-DD-YYYY, or MM.DD.YYYY format using regex
    dates+=( $(echo "$line" | grep -Eo '(0[1-9]|1[0-2])[/.-](0[1-9]|[1-2][0-9]|3[0-1])[/.-](\d{1,4})') )
done < "$file"

# iterate array and convert dates to YYYY-MM-DD format and store in temp array
for i in "${!dates[@]}"; do
    temp_dates+=( $(echo "${dates[$i]}" | sed -E 's/([0-9]{2})[\/.-]([0-9]{2})[\/.-]([0-9]{4})/\3-\1-\2/') )
done

# iterate assoc array and store dates in YYYY-MM-DD format as keys and original dates as values
for i in "${!dates[@]}"; do
    dates_assoc["${temp_dates[$i]}"]="${dates[$i]}"
done

# sort dates in YYYY-MM-DD format
sorted_dates=($(printf '%s\n' "${!dates_assoc[@]}" | sort))

# print dates in original format
for i in "${!sorted_dates[@]}"; do
    echo "${dates_assoc[${sorted_dates[$i]}]}"
done

exit 0
