# Instroduction

Ma propre lib de vecteurs. C'est relativement simple à utiliser, faut juste penser à cast vers le bon type à chaque fois

# Pourquoi cette bibliothèque ?

A première vue, on se dirait qu'on pourrait tout aussi bien utiliser un simple tableau dynamique et d'allouer sa mémoire lorsque nécessaire. Cependant, ce choix pose son lot de problème : oubli de réallocation, impossibilité de retrouver la taille réelle du tableau, code moins lisible, et ainsi de suite. Et même, tout simplement, une telle implémentation peut rebuter des débutants.

Cette bibliothèque apporte donc une implémentation du Vector (ou vecteur, dans la langue de Molière), en apportant quelques fonctionnalités du type du même nom du C++. Cette implémentation permet, entre autres :
- l'instanciation de vecteurs contenant n'importe quel type
- l'insertion et la suppression d'éléments à la fin du vecteur ou à un indice donné
- l'augmentation automatique de l'espace mémoire allouée en cas de dépassement de la mémoire déjà allouée
- la concaténation de deux vecteurs


# Maniement du Vector

## Le type Vector

Le type Vector est une interface entre l'utilisateur et le fonctionnement interne qui permet de masquer au maximum la syntaxe des pointeurs. Par défaut, tout maniement d'un Vector se fait par adresse. Il est donc inutile de déréférencer un Vector.

## Création d'un Vector

La création d'un Vector peut se faire de deux manières : soit en créant un Vector vide, soit depuis un tableau.

### Vector vide

```c
Vector int_vector = new_vector(v_type(int), 3);  // vecteur d'entiers
//                             ^^^^^^^^^^^  ^
//                              type des   Capacité
//                              éléments   initiale (en nombre d'éléments)
Vector double_vector = new_vector(type(double), 2); // vecteur de flottants double précision
Vector empty_vector = new_vector(type(long), 0);  // on peut initialiser un vecteur vide
Vector ptr_vector = new_vector(type(int *), 2); // vecteur de pointeurs
Vector vect_vector = new_vector(type(Vector), 2); // vecteur de vecteurs

/* De manière générale, on peut initialiser des vecteurs de n'importe quelle type */
/* NB : type(t) est une macro définie dans le cadre de la bibliothèque et équivaut à écrire sizeof(t). */
```

### A partir d'un tableau

```c
int[5] array = {1, 2, 3, 4, 5};  // création d'un tableau
Vector arr_vector = new_vector_from_array(array, v_type(int), 5);
//                                        ^^^^^  ^^^^^^^^^  ^
//                                      adresse  type des   nombre d'éléments
//                                   du tableau  éléments   dans le tableau
```

## Libération mémoire d'un Vector

Lorsque l'usage d'un Vector est terminé, il doit impérativement être libéré de la mémoire avec la fonction drop_vector().

```c
Vector vect = new_vector(v_type(int), 4);

/* Utilisation du vecteur */

drop_vector(vect); // libération du vecteur
```

Si les éléments d'un vecteur ont été dynamiquement alloués, ils doivent être eux-mêmes libérés avant la libération du vecteur.

```c
// v_size() et vector_get() sont des fonctions vues plus bas
for (int i = 0; i < v_size(vect); ++i) {
    free(vector_get(vect, i));
}
drop_vector(vect);
```

## Aggrandir un Vector

### En ajoutant des éléments en dernière position

Deux méthodes existent pour insérer en dernière position, en fonction de si on veut insérer une valeur ou bien plusieurs.
Quelle que soit la fonction utilisée, l'augmentation de l'espace mémoire alloué au vecteur se fait automatiquement. L'utilisateur ne doit surtout pas essayer de gérer lui-même la mémoire allouée au vecteur (à moins d'être prêt à se prendre des bonnes grosses erreurs dans sa gueule).

* une seule valeur : vector_push()

```c
Vector vect = new_vector(v_type(int), 2);
int new_elem = 5;

vector_push(vect, &new_elem);
            ^^^^  ^^^^^^^^^
//       vecteur  adresse de l'élément
//     à étendre  à ajouter

new_elem = 4;
vector_push(vect, &new_elem);

new_elem = 3;
vector_push(vect, &new_elem);
// On peut ajouter plus d'éléments que la capacité initiale prévue lors de la création du vecteur.
// Le vecteur est étendu automatiquement lorsqu'on dépasse sa capacité

/* Le vecteur contient à présent les valeurs 5, 4 et 3, dans l'ordre */
```

* un tableau de valeurs : vector_push_array()
* 
```c
Vector vect = new_vector(v_type(int), 2);
int arr_for_push[5] = {1, 3, 5, 7, 11};

vector_push_array(vect, arrray_for_push, 5);
                  ^^^^  ^^^^^^^^^^^^^^^  ^
//             vecteur  adresse du      nombre d'éléments
//           à étendre  tableau         du tableau

/* Le vecteur contient à présent les valeurs 1, 3, 5, 7, 11, dans l'ordre */
```
# Exemple de code

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
}
