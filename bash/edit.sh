#!/bin/bash

path_to_file=$1
replaced_str=$2
replace_on=$3

if [ ! -f "$path_to_file" ]; then
  echo "No such file or directory. Please try again."
  exit 1
fi

if [ -z $replaced_str ]; then
  echo "Pattern to substing cannot be empty. Please try again."
  exit 1
fi

value=$(cat $path_to_file)
value=${value//$replaced_str/$replace_on}

echo "$value" > $path_to_file

date_time=$(date +"%Y-%m-%d %T")
size=$(stat -f %z $path_to_file)
shasum=$(shasum -a 256 $path_to_file | cut -d ' ' -f 1)

echo "$path_to_file - $size - $date_time - $shasum - sha256" >> /Users/deadshot/School/T02D02-0/src/files.log