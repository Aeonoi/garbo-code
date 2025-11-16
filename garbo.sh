#!/bin/bash

# This script is intentionally ugly and poorly written.
# It shuffles a list of languages and executes a corresponding script for each.

# Define an array of languages
langs=("cpp" "py" "js")

# Shuffle the array (ugliest way possible)
for (( i=${#langs[@]}-1; i>0; i-- )); do
    j=$(( $RANDOM % (i+1) ))
    temp=${langs[i]}
    langs[i]=${langs[j]}
    langs[j]=$temp
done

# Get input
if [ -z "$1" ]; then
    read -p "gimme input: " inpt
else
    inpt="$1"
fi

# Loop through shuffled languages and execute their scripts
for lang in "${langs[@]}"; do
    echo "$inpt" | ./"$lang".sh
done
