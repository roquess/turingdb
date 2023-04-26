#!/bin/bash

py_files=$(find $WORKSPACE/src/python -name '*.py')
for file in $py_files
do
    python3 -m py_compile $file
done
