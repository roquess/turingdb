#!/bin/bash

base_path=$(pwd)
samples=$(cat ./test.list)
if [ -z "$samples" ]; then
    echo "No samples found"
    exit 1
fi

echo '- Running samples:'
for sample in $samples; do
    echo "    $sample"
    cd "$sample" || exit 1
    bash run.sh
    code=$?
    if [ $code -ne 0 ]; then
        echo "Failed to run sample $sample. Exit code: $code"
        exit 1
    fi

    cd "$base_path" || exit 1
done
