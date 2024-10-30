#!/bin/bash

new_git_hash=$(git -C $2 rev-parse HEAD)

if [[ ! -f $1 ]]; then
    echo $new_git_hash > $1
    bash $2/build.sh $2 &
    wait;
    exit;
fi

existing_git_hash=$(cat $1)

if [ "$new_git_hash" != "$existing_git_hash" ]; then
    echo $new_git_hash > $1
    bash $2/build.sh $2 &
    wait;
    exit;
fi

