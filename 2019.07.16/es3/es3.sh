#!/bin/bash
for file in $(find "$1" -type f -print); do #$1 prende il primo arg dato al comando
    for file2 in $(find "$1" -type f -print); do 
        if cmp -s "$file" "$file2"; then
            if [[ "$(stat -c '%i' "$file")" != "$(stat -c '%i' "$file2")" ]] 
            then
                echo "dovrei creare un hard link per $file e $file2"
                rm "$file2"
                ln "$file" "$file2"
            fi
        fi
    done
done
