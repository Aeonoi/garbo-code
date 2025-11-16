#!/bin/bash
# c.sh

# Read the combined input
read combined_data

# Split combined_data into original input and language array string
inpt=$(echo "$combined_data" | cut -d'^' -f1)
lang_array_str=$(echo "$combined_data" | cut -d'^' -f2)

# Split language array string into individual languages
IFS='|' read -r -a langs <<< "$lang_array_str"

# Execute C code with original input
echo "$inpt" | gcc -o c_bin c.c && ./c_bin

# If there are more languages, pipe remaining to the next script
if [ ${#langs[@]} -gt 1 ]; then
    next_langs_str=$(printf "%s|" "${langs[@]:1}")
    next_langs_str=${next_langs_str%|}
    echo "${inpt}^${next_langs_str}" | ./"${langs[1]}".sh
fi
