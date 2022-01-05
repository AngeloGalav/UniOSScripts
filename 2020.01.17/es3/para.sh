#!/bin/bash
# A sample Bash script, by Ryan

IN="$*"
IFS="//" read -r -a arrIN <<< "$IN"
for(( j=0; j<${#arrIN[@]}; j++ ));
do
    echo "evaluating $j to ${#arrIN[@]}"
    eval ${arrIN[$j]} &
done
wait
echo "commands over."


# some_command &
# P1=$!
# other_command &
# P2=$!
# wait $P1 $P2