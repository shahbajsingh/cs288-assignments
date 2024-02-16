#!/bin/bash

# function to traverse directory and find duplicate files
function traverseDir() {
  local directory="$1" # get the directory name from the first argument
  declare -A duplicates # declare array to store the duplicates

  while IFS= read -r -d '' file; do # while there are files in the directory
    filename=$(basename "$file") # get the filename from the path
    if [[ -n ${duplicates[$filename]} ]]; then # if the filename already exists in the array it is duplicate
      echo "Duplicate files found:"
      echo "${duplicates[$filename]}"
      echo "$file"

      removeFile "$file" # call remove function to prompt for file removal
    else # else add the filename to the array
      duplicates[$filename]="$file"
    fi
  done < <(find "$directory" -type f -print0) # find all files in the directory and store them in the array
}

# function to prompt for file removal and execute it
function removeFile() {
  local file="$1" # get the file name from the first argument

  # must use </dev/tty to read from terminal instead of stdin (or else it will read from $file)
  read -p "Remove '$file'? (y/n) " choice </dev/tty

  # check if the choice is yes or no
  if [[ "$choice" == [Yy] ]]; then
    rm "$file"
    echo "File '$file' removed."
  elif [[ "$choice" == [Nn] ]]; then
    echo "File '$file' not removed."
  else
    echo "Invalid choice. File '$file' not removed."
  fi
}

# SCRIPT STARTS HERE

# check if directory argument is provided
if [ -z "$1" ]; then
  echo "ERROR: No directory argument provided."
  exit 1
fi

traverseDir "$1"
