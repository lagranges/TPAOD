#! /bin/sh

# Auteur : Lee Van Tuan &  Arvis Clément
# Version initiale : 12/04/2017

for infile in `find benchmarks/ -name "*.in" | sort`
do
    nb_elt=$(wc -w "$infile" | awk '{print $1;}')
    outfile="${infile%.in}".out
    outfile=$(tail -n +2 "$outfile") #on supprime la premiere ligne
    res=$(./bin/compileBST "$nb_elt" "$infile" 2> /dev/null )
    outfile=${outfile%static int opt*}
    outfile=${outfile// /} #On supprime les espaces
    outfile=$(echo $outfile | tr -d '\r') #on supprime les retour à la ligne
    res=${res// /} #On supprime les espaces
    res=$(echo $res | tr -d '\r') #on supprime les retour à la ligne
    if [ "$res" == "$outfile" ]
    then
	echo -e "\033[32m[PASSED] \033[0m $infile" #resultat = expected
    else
	echo -e "\033[31m[FAILED] \033[0m $infile"
    fi
done
