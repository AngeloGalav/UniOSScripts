#!/bin/bash
for file in $(find "$1" -type f -name "*.c"); do #$1 prende il primo arg dato al comando
    echo "indentazione in corso"
    echo "checking file $file"
    ex -s '+norm!gg=G' +wq "$file" #perccato che questo comando non va...
done
