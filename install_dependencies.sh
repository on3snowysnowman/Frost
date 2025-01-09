#!/bin/bash

# Directory of the paths file.
paths_file="TargetPaths.txt"

# Extract the third line and store it in a variable
third_line=$(sed -n '3p' "$paths_file")

cp -r "assets" "$third_line"
cp -r "data" "$third_line"

