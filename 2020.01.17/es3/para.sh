#!/bin/bash

IN="$*" #prende input l'intera stringa
IFS="//" read -r -a arrIN <<< "$IN" #la suddivide in sottostringhe in base a //
for(( j=0; j<${#arrIN[@]}; j++ ));
do
    echo "evaluating $j to ${#arrIN[@]}"
    eval ${arrIN[$j]} & # esegue i programmi in modo concorrente
done
wait
echo "commands over."