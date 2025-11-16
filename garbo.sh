#!/bin/bash

# This script is intentionally ugly and poorly written.
# It shuffles a list of languages and executes a corresponding binary.
# Modified so that each script's output feeds into the next one.

# Define an array of languages
langs=("cpp" "py" "js" "go" "rs" "zig" "java" "hs" "pl" "scm" "ml" "c" "ts" "perl" "vim" "lua")

# Shuffle the array (still ugly)
for (( i=${#langs[@]}-1; i>0; i-- )); do
    j=$(( RANDOM % (i+1) ))
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

# Loop through shuffled languages
for lang in "${langs[@]}"; do
    echo "=== Running $lang.sh ==="
    cat "$lang.sh"
    echo "Input: $inpt"

    # Capture output and store it in inpt
    inpt="$(echo "$inpt" | "./$lang.sh")"

    echo "Output -> New input: $inpt"
    echo
done

# Final steps
cd ./HackRPI-2025/ || exit 1
make
./asteroids "$inpt"

