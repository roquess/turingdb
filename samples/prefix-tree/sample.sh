#!/bin/bash

shuf -n $(($1/2)) data/linuxwords > data/linuxqueries

shuf -n $(($1/4)) data/linuxwords | while read word; do 
    len=${#word}; 
    trunc=$((RANDOM % 4)); 
    newlen=$((len > trunc ? len - trunc : 1)); 
    echo "${word:0:$newlen}"
done >> data/linuxqueries

for i in $(seq 1 $(($1/4))); do
    len=$((RANDOM % 11 + 5));
    tr -dc 'a-z' < /dev/urandom | head -c $len
    echo
done >> data/linuxqueries
