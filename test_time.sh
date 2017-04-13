#! /bin/sh

# Auteur : Lee Van Tuan &  Arvis Clément
# Version initiale : 12/04/2017


do_test(){
    local infile=$1
    local nb_elt=$2
    local test
    test=$( /usr/bin/time -f "%e" ./bin/compileBST "$nb_elt" "$infile"  2>&1 1>/dev/null  | cut -d ' ' -f 8- ) 
    test=${test//.} #on supprime le point
    test=$(echo "$test" | sed 's/^0*//')  #on supprime les zeros en tete
    test+='0' #on convertit en millisecondes
    echo "$test"  
}



printf "                          MIN   MAX   MOY  (en ms)\n"

for infile in `find benchmarks/ -name "*.in" | sort`
do
    nb_elt=$(wc -w "$infile" | awk '{print $1;}')
    test1=$(do_test $infile $nb_elt)
    test2=$(do_test $infile $nb_elt)
    test3=$(do_test $infile $nb_elt)
    test4=$(do_test $infile $nb_elt)
    test5=$(do_test $infile $nb_elt)

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
    printf "$min     "
    printf "$max     "
    printf "$moy     \n"

done
