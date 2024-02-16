#!/bin/bash

# read directory name from user into variable dirname
read -p "Enter directory name: " dirname
if [[ ! -d "$dirname" ]]; then # check if directory exists
  echo "ERROR: Directory does not exist."
  exit 1 # exit with error
fi

# change directory to dirname
cd "$dirname"
files=(*) # store all files in directory into array

# prompt user to select file from menu
select file in "${files[@]}"; do 
  if [ -f "$file" ]; then # check if file exists
    lines=10 # set lines variable to 10
    file_lines=$(wc -l < "$file") # store number of lines in file into variable file_lines
    while true; do # loop until user chooses to exit
      clear # clear screen (for better readability using tail command)
      tail -n "$lines" "$file" # display last $lines lines of file
      if [[ $lines -ge $file_lines ]]; then # check if end of file is reached or exceeded
        echo "Beginning of file reached." 
        exit 0 # exit with success
      fi
      read -p "Display more lines? (y/n): " choice # prompt user to display more lines
      if [[ "$choice" == [Yy] ]]; then # check if user wants to display more lines
        lines=$((lines + 10)) # increment lines variable by 10
        if [[ $lines -gt $file_lines ]]; then # check if lines variable exceeds number of lines in file
          lines=$file_lines # set lines variable to number of lines in file
        fi
      elif [[ "$choice" == [Nn] ]]; then # check if user wants to exit
        break # break out of loop
      else # user entered invalid choice
        echo "Invalid choice."
      fi
    done
    break
  else # file chosen is not a readable file
    echo "ERROR: $file is not a readable file."
    read -n1 -p "Select another file or enter 'n' to exit. " choice
    if [[ "$choice" == [Nn] ]]; then
	  echo
	  exit 0
    fi
    # exit 1 # exit with error
  fi
done
