#! /bin/sh

# Auteur : Lee Van Tuan &  Arvis Clément
# Version initiale : 12/04/2017

for infile in `find benchmarks/ -name "*.in" | sort`
do
    nb_elt=$(wc -w "$infile" | awk '{print $1;}')
    outfile="${infile%.in}".out
    outfile=$(tail -n +2 "$outfile") #on supprime la premiere ligne
    res=$(./bin/compileBST "$nb_elt" "$infile")
    outfile=${outfile// /} #On supprime les espaces
    res=${res// /} #On supprime les espaces
    if [ "$res" == "$outfile" ]
    then
	echo -e "\033[32m[PASSED] \033[0m $infile)" #resultat = expected
    else
	echo -e "\033[31m[FAILED] \033[0m $infile)"
    fi
done
