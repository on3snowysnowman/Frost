#!/bin/bash

# Directory of the paths file.
paths_file="../../TargetPaths.txt"

# Extract the second line.
second_line=$(sed -n '2p' "$paths_file")

# Extract the third line.
third_line=$(sed -n '3p' "$paths_file")

cp -r "../../assets" "$third_line"
cp -r "../../data" "$third_line"

