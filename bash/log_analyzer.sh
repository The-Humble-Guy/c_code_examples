#!/bin/bash

path_to_log=$1

if [ ! -f "$path_to_log" ]; then
  echo "No such file or directory. Please try again."
  exit 1
fi

if [ $(echo $path_to_log | rev | cut -b 1-4 | rev) != ".log" ]; then
  echo "No matching filetype. Please input .log file"
fi

count_of_rows=$(wc -l $path_to_log | cut -d ' ' -f 8)
count_of_unique=$(sort $path_to_log | cut -d ' ' -f 1 | uniq | wc -l | cut -d ' ' -f 8)
hash_count=$(sort -k8 $path_to_log | cut -d ' ' -f 8 | uniq | wc -l | cut -d ' ' -f 8)

echo "Count of strings: $count_of_rows"
echo "Count of unique: $count_of_unique"
echo "Count of hash changes: $hash_count"
