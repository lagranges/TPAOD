/*! \file compileABROpt.c
 *  \brief	   Programme permets d'afficher l'arbre d'optimal sur la sortie standard.
 *  \author    Lucie Pansart
 *  \author    Jean-Louis Roch
 *  \author    ARVIS Clément
 *  \author    LE Van Tuan
 *  \version   1.0
 *  \date      14/4/2017
 *  \warning   Usage: compileBST n originalFile 
 *  \copyright GNU Public License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <float.h>

/* Le tableau stockée les coûts*/
int **e;
/* Le tableau stockée les racines*/
int **r;
/* Le tableau stockée les BSTtree*/
int **BSTtree;

/**
 * \brief Mettre recursivement les valeur de BStree [r[i,j]][] à jour. 
 * \param start première élément de l'arbre T[i][j] 
 * \param end dernière élément de l'arbre T[i][j]
 */
void maj_btree(int start,int end){
    int parcourir = r[start][end];
    // arbre soit une racine unique
    if(start == end) {
        BSTtree[start][0] = -1;
        BSTtree[end][1] = -1;
    } 
    // arbre droite soit une racine unique  
    else if(parcourir == start){
        BSTtree[parcourir][1] = r[parcourir+1][end];
        BSTtree[parcourir][0] = -1;
        // on parcourt à gauche
        maj_btree(parcourir+1, end);
    } 
    // arbre gauche soit une racine unique
    else if(parcourir == end){
        BSTtree[parcourir][0] = r[start][parcourir-1];
        BSTtree[parcourir][1] = -1;
        // on parcourt à droit
        maj_btree(start, parcourir-1);
    } 
    // sinon
    else {
        BSTtree[parcourir][0] = r[start][parcourir-1];
        BSTtree[parcourir][1] = r[parcourir+1][end];
        // on parcourt à droit
        maj_btree(parcourir+1, end);
        // on parcourt à gauche
        maj_btree(start, parcourir-1);
    }
}    

/**
 * Main function
 * \brief Main function
 * \param argc  A count of the number of command-line arguments
 * \param argv  An argument vector of the command-line arguments.
 * \warning Must be called with a positive long integer, n,  and a filename, freqFile, as commandline parameters and in the given order.
 * \returns { 0 if succeeds and prints C code implementing an optimal ABR on stdout; exit code otherwise}
 */
int main (int argc, char *argv[]) {
  long n = 0 ; // Number of elements in the dictionary
  FILE *freqFile = NULL ; // File that contains n positive integers defining the relative frequence of dictinary elements 
  
  if(argc != 3){
    fprintf(stderr, "!!!!! Usage: ./compileBST n  originalFile !!!!!\n");
      exit(EXIT_FAILURE); /* indicate failure.*/
  }

  { // Conversion of parameter n in a long 
    int codeRetour = EXIT_SUCCESS;
    char *posError;
    long resuLong;
    n = atol(argv[1] ) ;
   
    assert(argc >= 2);
    // Conversion of argv[1] en long
    resuLong = strtol(argv[1], &posError, 10);
    // Traitement des erreurs
    switch (errno)
    {
      case EXIT_SUCCESS :
         // Conversion du long en int
         if (resuLong > 0)
         {
            n = (long)resuLong;
            fprintf(stderr, "Number of elements in the dictionary: %ld\n", n);         
         }
         else
         {
            (void)fprintf(stderr, "%s cannot be converted into a positive integer matching the number of elements in the dicionary.\n", argv[1]) ; 
            codeRetour = EXIT_FAILURE;
         }
      break;
      
      case EINVAL :
         perror(__func__);
         (void)fprintf(stderr, "%s does not match a long integer value. \n", argv[1]);
         codeRetour = EXIT_FAILURE;
      break;
      
      case ERANGE :
         perror(__func__);
         (void)fprintf(stderr, "%s does not fit in a long int :\n" "out of bound [%ld;%ld]\n",
                       argv[1], LONG_MIN, LONG_MAX);
         codeRetour = EXIT_FAILURE;
      break;
      default :
         perror(__func__);
         codeRetour = EXIT_FAILURE;
    } // switch (errno)
    if  (codeRetour != EXIT_SUCCESS) return codeRetour ;
  }

  freqFile = fopen(argv[2] , "r" );
  if (freqFile==NULL) {fprintf (stderr, "!!!!! Error opening originalFile !!!!!\n"); exit(EXIT_FAILURE);}

/**
 * Lecture du freqFile pour extraite p[i]
 * p[i] probabilité d'accèss à l'élément s[i] i=0..n-1
 */    
 
    int p[n]; // probabilité d'accèss à l'élément i i = 0..n-1
    int i;
    for(i=0;i<n;i++){
        fscanf(freqFile,"%d",&p[i]);
    }
    fclose(freqFile);


/**
 * Arbre Binaire de Recherche Optimal
 * Les clefs 1,2,...n avec posibilité P1, P2, .... Pn
 * Tij un ABR pour les clefs ai, a(i+1), ... aj
 * w[i][j] = P[i] + P[i+1] + .. P[j]
 * r[i][j] la racine de l'arbre Tij;
 * e[i][j] le coût de l'arbre Tij
 */
    int w[n+1];
    int j,l,k,m;
    int t, mint;
    r = malloc(n*sizeof(int *));
    for(i = 0; i< n; i++)
        r[i] = (int *)malloc(n*sizeof(int));
    e = malloc((n+2)*sizeof(int *));
    for(i = 0; i< n+2; i++)
        e[i] = (int *)malloc((n+1)*sizeof(int));

    // initialiser 
    for(i=0;i<n;i++)
        r[i][i] = i;

    for(i=1; i<n+1;i++){
        e[i][i-1] = 0;
        e[i][i] = p[i-1];
        w[i] = w[i-1] + p[i-1];
    }
    // calcule 
    for(l=1;l<n;l++)
    for(i=1;i<n-l+1;i++){
        j = i+l;
        mint = INT_MAX;
        // r[i][j-1] <= r[i][j] <= r[i+1][j]
        for(k=r[i-1][j-2]+1;k<r[i][j-1]+2;k++){
            // déterminer k pour que la valeur de e[i][j] soit minimal
            t = e[i][k-1] + e[k+1][j];
            if(t<mint) {
                mint = t;
                m = k;
            }
        }
        e[i][j] = mint + w[j] - w[i-1];
        r[i-1][j-1] = m-1;
    }

/**
 * Ecrire l'arbre optimal au-dessus sur fichier .c
 *
 */
    BSTtree = malloc(n*sizeof(int *));
    for(i = 0; i< n; i++)
        BSTtree[i] = (int *)malloc(2*sizeof(int));
    for(i=0;i<n;i++){
        BSTtree[i][0] = -1;
        BSTtree[i][1] = -1;
    }
    
    
    maj_btree(0,n-1);

    /*FILE * fichierArbre = NULL;
    // le nom du Fichier généré formé nomSource_arbre.c
    char * nomFichier = strcat(argv[2],"_arbre.c");
    // creer fichier sous nom nomFichier
    fichierArbre = fopen(nomFichier,"w");
    if (freqFile==NULL) {
        fprintf (stderr, "Error opening File \n"); // TODO 
        exit(EXIT_FAILURE);
    }*/
    // écire les arbres
    // La racine de l'arbre
    printf("static int BSTroot = %d;\n", r[0][n-1]); 
    // La valeur de BST
    printf("static int BSTtree[%ld][2] = {\n",n);
    for(i=0;i<n;i++){
        printf("{");
        printf("%d",BSTtree[i][0]);
        printf(", ");
        printf("%d",BSTtree[i][1]);
        printf("}");
        // sans ',' pour le dernier élément
        if(i!=n-1)
            printf(", \n");
    }
    printf(" };");
  return 0;
}

