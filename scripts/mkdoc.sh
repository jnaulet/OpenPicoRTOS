#!/bin/sh
DIR=$(dirname $0)

while IFS='' read -r line
do
    cmd=$(echo $line | awk -F'!' '/^[!](.*)/ {print $2}' -)
    [ -z "$cmd" ] && echo "$line" || $DIR/$cmd
done < "$1"
