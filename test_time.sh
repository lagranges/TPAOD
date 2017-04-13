#! /bin/sh

# Auteur : Lee Van Tuan &  Arvis Clément
# Version initiale : 12/04/2017


printf "                          MOY   MIN   MAX\n"

for infile in `find benchmarks/ -name "*.in" | sort`
do
    nb_elt=$(wc -w "$infile" | awk '{print $1;}')
    test1=$( /usr/bin/time -f "%e" ./bin/compileBST "$nb_elt" "$infile"  2>&1 1>/dev/null  | cut -d ' ' -f 8- ) 
    test1=${test1//.} #on supprime le point
    test1=$(echo "$test1" | sed 's/^0*//')  #on supprime les zeros en tete
    test1+='0' #on convertit en millisecondes
    test2=$( /usr/bin/time -f "%e" ./bin/compileBST "$nb_elt" "$infile"  2>&1 1>/dev/null  | cut -d ' ' -f 8- ) 
    test2=${test2//.} #on supprime le point
    test2=$(echo "$test2" | sed 's/^0*//')  #on supprime les zeros en tete
    test2+='0' #on convertit en millisecondes
    test3=$( /usr/bin/time -f "%e" ./bin/compileBST "$nb_elt" "$infile"  2>&1 1>/dev/null  | cut -d ' ' -f 8- ) 
    test3=${test3//.} #on supprime le point
    test3=$(echo "$test3" | sed 's/^0*//')  #on supprime les zeros en tete
    test3+='0' #on convertit en millisecondes
    test4=$( /usr/bin/time -f "%e" ./bin/compileBST "$nb_elt" "$infile"  2>&1 1>/dev/null  | cut -d ' ' -f 8- ) 
    test4=${test4//.} #on supprime le point
    test4=$(echo "$test4" | sed 's/^0*//')  #on supprime les zeros en tete
    test4+='0' #on convertit en millisecondes
    test5=$( /usr/bin/time -f "%e" ./bin/compileBST "$nb_elt" "$infile"  2>&1 1>/dev/null  | cut -d ' ' -f 8- ) 
    test5=${test5//.} #on supprime le point
    test5=$(echo "$test5" | sed 's/^0*//')  #on supprime les zeros en tete
    test5+='0' #on convertit en millisecondes

    #echo "$test1 $test2 $test3 $test4 $test4 $test5"
    moy=$(( $((test1)) + $((test2)) + $((test3)) + $((test4)) + $((test5)) ))
    moy=$(($moy/5))
    arr=("$((test1))" "$((test2))" "$((test3))" "$((test4))" "$((test5))")
    max=${arr[0]}
    for n in "${arr[@]}" ; do
        ((n > max)) && max=$((n))
    done

    min=${arr[0]}
    for n in "${arr[@]}" ; do
        ((n < min)) && min=$n
    done

    printf "$infile  "
    printf "$moy     "
    printf "$min     "
    printf "$max     \n"

done
