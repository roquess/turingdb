#!/bin/bash

set -e

py_files=$(find python -name '*.py')
for file in $py_files
do
    python3 -m py_compile $file
done
