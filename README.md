# Description
Ma propre lib de vecteurs. C'est relativement simple à utiliser, faut juste penser à cast vers le bon type à chaque fois


# Comment ça marche ?

Un exemple vaut mieux que 1000 mots, donc voici un morceau de code qui utilise toutes les fonctions mises à disposition : 

```c
#include <stdio.h>
#include "vector.h"

// affichage du vecteur
void display_vector(Vector vector) {
    // vector_get_size() renvoie le nombre d'éléments présents dans le vecteur
    int * elem_ptr;
    for (int i = 0; i < vector_get_size(vector); ++i) {
        // la fonction vector_get() renvoie un pointeur void. Il est impossible de l'afficher directement.
        // Il faut d'abord caster le résultat de vector_get() vers un pointeur du type voulu,
        // puis déréférencer ce pointeur
        elem_ptr = (int *) vector_get(vector, i);
        printf("%d; ", *elem_ptr);
    }
    printf("\n");
}

int main() {
    // La propriété la plus intéressante du vecteur est qu'il est extensible :
    // Si le vecteur est trop petit pour accueillir un nouvel élément, il s'étend automatiquement,
    // sans que l'utilisateur ait besoin de se soucier de ce détail d'implémentation

    // ATTENTION : le C ne contient pas de notion d'encapsulation, cependant, vous êtes priés de ne
    // jamais, JAMAIS essayer d'accéder directement aux champs de la structure du vecteur.
    // Passez impérativement par les fonctions dédiées, ça évitera les erreurs, et tout le monde se portera mieux

    // Création du vecteur. Ici, on crée un vecteur d'entiers pouvant stocker trois éléments
    Vector my_vector = new_vector(/*taille en mémoire d'un élément*/sizeof(int), /*Capacité initiale du vecteur*/3);
    int a = 5; // Création d'un élément à insérer
    // La fonction vector_push() insère un élément à la dernière position du vecteur.
    // Les éléments doivent impérativement être donnés par adresse
    vector_push(my_vector, /*adresse de l'élément à ajouter*/ &a);
    a = 4;
    // vector_push(my_vector, a); ne marche pas
    // vector_push(my_vector, 4); ne marche pas non plus
    // vector_push(my_vector, &4); toujours non, n'essayez même pas
    vector_push(my_vector, &a);
    a = 9;
    // on peut insérer plusieurs fois un même élément
    vector_push(my_vector, &a);
    vector_push(my_vector, &a);
    display_vector(my_vector); // 5; 4; 9; 9;

    // La fonction vector_insert() insère un élément à un index donné et décale les éléments suivants
    // si l'index donné est hors des limites du vecteur, l'insertion ne se fait pas
    a = 1;
    vector_insert(my_vector, /*adresse de l'élément à ajouter*/ &a, /*index de l'élément*/ 2);
    display_vector(my_vector); // 5; 4; 1; 9; 9;

    // La fonction vector_pop() retire le dernier élément du vecteur et renvoie cet élément
    // si le vecteur ne contient aucun élément, la fonction renvoie NULL
    vector_pop(my_vector);
    display_vector(my_vector); // 5; 4; 1; 9;

    // vector_remove() retire l'élément à l'index donné en argument et décale les éléments suivants
    // d'un rang vers la gauche.
    // vector_remove() renvoie l'élément retiré
    vector_remove(my_vector, 1);
    display_vector(my_vector); // 5; 1; 9;

    // drop_vector() détruit le vecteur et libère la mémoire qu'il occupe
    // ATTENTION : si des éléments du vecteur sont alloués dynamiquement, ils doivent être libérés au préalable
    // pour éviter tout fuite de mémoire
    drop_vector(my_vector);
    return 0;
}```
