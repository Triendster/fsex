#!/bin/bash 

# Run fsex, capture the output and navigate to the output directory
target_dir=$(./fsex "$@")

# If fsex returns a valid directory, navigate to it
if [ -d "$target_dir" ]; then
    cd "$target_dir"
fi
 
