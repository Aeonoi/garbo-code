#!/bin/bash

# This script is intentionally ugly and poorly written.
# It shuffles a list of languages and executes a corresponding script for each.

# Define an array of languages
langs=("cpp" "py" "js" "go" "rs" "zig" "java" "hs" "pl" "scm" "ml" "c" "ts" "perl" "vim" "lua")

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


# Join the shuffled languages with a separator
all_langs=$(printf "%s|" "${langs[@]}")
all_langs=${all_langs%|} # Remove trailing separator

# Combine input and languages with another separator
combined_data="${inpt}^${all_langs}"

# Pipe combined data to the first language script
echo "$combined_data" | ./"${langs[0]}".sh

