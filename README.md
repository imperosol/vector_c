# Introduction

Ma propre lib de vecteurs. C'est relativement simple à utiliser, faut juste penser à cast vers le bon type à chaque fois

# Pourquoi cette bibliothèque ?

A première vue, on se dirait qu'on pourrait tout aussi bien utiliser un simple tableau dynamique et d'allouer sa mémoire lorsque nécessaire. Cependant, ce choix pose son lot de problème : oubli de réallocation, impossibilité de retrouver la taille réelle du tableau, code moins lisible, et ainsi de suite. Et même, tout simplement, une telle implémentation peut rebuter des débutants.

Cette bibliothèque apporte donc une implémentation du Vector (ou vecteur, dans la langue de Molière), en apportant quelques fonctionnalités du type du même nom du C++. Cette implémentation permet, entre autres :
- l'instanciation de vecteurs contenant n'importe quel type
- l'insertion et la suppression d'éléments à la fin du vecteur ou à un indice donné
- l'augmentation automatique de l'espace mémoire allouée en cas de dépassement de la mémoire déjà allouée
- la concaténation de deux vecteurs

# Installation de la bibliothèqe

1. Télécharger les fichiers
    * Depuis Github : code > download ZIP
    * Depuis Git : `git clone https://github.com/imperosol/vector_c.git <répertoire de téléchargement>`
2. Copier les fichiers dans le répertoire du projet
3. Ajouter les fichiers aux sources du projet :
    * Sur Code::Blocks : project > add files > sélectionner les fichiers
    * Sur CLion : ouvrir CMakeLists.txt > ajouter `vector.c vector.h` à la fin de la commande `add_executable()`

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
Vector double_vector = new_vector(v_type(double), 2); // vecteur de flottants double précision
Vector empty_vector = new_vector(v_type(long), 0);  // on peut initialiser un vecteur vide
Vector ptr_vector = new_vector(v_type(int *), 2); // vecteur de pointeurs
Vector vect_vector = new_vector(v_type(Vector), 2); // vecteur de vecteurs

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

### A partir d'un autre Vector

La fonction new_vector_from_vector crée une copie d'un Vector déjà existant. Le nouveau Vector est totalement indépendant de l'ancien et ses modifications n'affectent pas le vecteur d'origine (et vice-versa).

```c

int[5] array = {1, 2, 3, 4, 5};  // création d'un tableau
Vector vector = new_vector_from_array(array, v_type(int), 5);

Vector copy_vector = new_vector_from_vector(vector);
                                            ^^^^^^
                                            vecteur
                                            copié
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

## Effacer un Vector

Il est également possible d'effacer un vecteur sans le détruire complètement en utilisant la fonction clear_vector(). Le vecteur est alors vidé de tous ses éléments sans pour autant être libéré de la mémoire.

Cette fonction est à utiliser lorsque l'on veut effacer le contenu du vecteur mais que l'on compte continuer malgré à utiliser ce vecteur. Si l'usage du vecteur est terminé, la fonction drop_vector() doit être utilisée.

Attention : après appel de la fonction clear_vector(), le type des éléments contenus dans le vecteur ne change pas !

Tout comme pour la fonction drop_vector(), si des éléments d'un vecteur sont alloués dynamiquement, ils doivent être libérés **avant** l'appel de la fonction clear_vector().

```c
Vector vect = new_vector(v_type(int), 4);

/* Utilisation du vecteur */

clear_vector(vect); // réinitialisation du vector

int a = 5;
vector_push(vect, &a);  // Le vecteur peut encore être utilisé
// La fonction vector_push() est abordée plus loin

drop_vector(vect);  // Bien sûr, il ne faut pas oublier de jeter le vecteur à la fin de son usage.

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
//          ^^^^  ^^^^^^^^^
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
```c
Vector vect = new_vector(v_type(int), 2);
int arr_for_push[5] = {1, 3, 5, 7, 11};

vector_push_array(vect, arrray_for_push, 5);
//                ^^^^  ^^^^^^^^^^^^^^^  ^
//             vecteur    adresse du    nombre d'éléments
//           à étendre    tableau       du tableau

/* Le vecteur contient à présent les valeurs 1, 3, 5, 7, 11, dans l'ordre */
```

### En insérant des éléments à un indice donné

La fonction vector_insert() permet d'insérer un élément à un indice donné. Comme pour les tableaux, l'indice du premier élément d'un vecteur est 0.

```c
int[5] array = {1, 2, 3, 4, 5}; 
Vector vect = new_vector_from_array(array, v_type(int), 5);

int new_elem = 6;
vector_insert(vect, &new_elem,  2);
//            ^^^^  ^^^^^^^^^   ^
//         vecteur  adresse de  indice dans le
//                  l'élément   vecteur où effectuer
//                  à insérer   l'insertion

/* Le vecteur contient à présent les valeurs 1, 2, 6, 3, 4, 5, dans l'ordre */
```

## Retirer un élément d'un Vector

Tout comme pour ajouter un élément dans un Vector, il existe deux cas pour retirer un élément d'un Vector : soit en retirant le dernier élément, soit en retirant l'élément d'un index donné.

Contrairement aux fonctions d'insertion, qui ne retournent rien, les fonctions de délétion retournent un pointeur vers l'élément qui a été retiré du tableau.

### Retirer le dernier élément du Vector

Pour retirer le dernier élément, on utilise la fonction vector_pop()
```c
int[5] array = {1, 2, 3, 4, 5}; 
Vector vect = new_vector_from_array(array, v_type(int), 5);

vector_pop(vect); // On retire le dernier élément sans chercher à le récupérer
//         ^^^^
//         vecteur
//         dont on enlève
//         le dernier élément

// v_elem est une macro qui convertit le pointeur retourné par les fonctions de délétion dans le type donné en argument
int poped_elem = v_elem(int, vector_pop(vect));
                        ^^^  ^^^^^^^^^^
                  type vers  fonction vector_pop()
               lequel faire  retourne un pointeur
              la conversion  qui doit être converti
              
printf("%d", poped_elem); // Affiche "4"

/* Le vecteur contient à présent les valeur 1, 2 et 3, dans l'ordre */
```

### Retirer un élément à un indice donné

Pour retirer à un indice donné, on utilise la fonction vector_remove(). Tout comme vector_pop(), vector_remove() renvoie un pointeur qui doit être converti vers le type voulu avec la macro v_type().
```c
int[5] array = {1, 2, 3, 4, 5}; 
Vector vector = new_vector_from_array(array, v_type(int), 5);

/* On supprime l'élément à l'indice 2 (ici : 3) */
vector_remove(vector, 2); // On retire le dernier élément sans chercher à le récupérer
//            ^^^^^^  ^
//           vecteur  indice de l'élément
//                    à retirer

/* On supprime l'élément qui se trouve à présent à l'indice 2 (ici : 4) et on récupère cet élément */
int poped_elem = v_elem(int, vector_remove(vect, 2));
              
printf("%d", poped_elem); // Affiche "4"

/* Le vecteur contient à présent les valeur 1, 2 et 5, dans l'ordre */
```

## vector_get() et vector_set()

vector_get() et vector_set() permettent respectivement de connaitre un élément à un indice de donner et de modifier un élément à un indice donné.

Comme pour les fonctions de délétion, ces fonctions renvoient un pointeur qui doit être converti vers le type voulu à l'aide de v_elem.

*Remarque : vector_get() et vector_set() peuvent respectivement être abbrégés en v_get() et v_set(), écriture qui sera utilisée dans la suite de la documentation.*

### vector_get() (ou v_get())

```c
int[5] array = {1, 2, 3, 4, 5}; 
Vector vector = new_vector_from_array(array, v_type(int), 5);

void *elem = v_get(vector, 2);
//                 ^^^^^^  ^
//                vecteur  indice 
//                         de l'élément

// Conversion et affichage
printf("%d", v_elem(int, elem));  // Affiche : "3"

```


### vector_set() (ou v_set())

```c
int[5] array = {1, 2, 3, 4, 5}; 
Vector vector = new_vector_from_array(array, v_type(int), 5);

int new_value = 9;
v_set(vector, 2, &new_value);
//    ^^^^^^  ^   ^^^^^^^^^
//   ecteur indice  adresse du nouvel
//                  élément

/* Les éléments de vector sont à présent 1, 2, 9, 4 et 5, dans l'ordre */

```

## Opérations entre vecteurs

### Concaténation de deux Vector

On peut concaténer deux vecteurs avec la fonction `vector_append()`

```c
vector_append(vect_dest, vect_src);
// L'appel de cette fonction ajoute le contenu de vect_src à vect_dest
```

## Les macros

La bibliothèque vector.h comprend aussi des macros. Là où la plupart des fonctions ont un nom débutant par `vector_`, les macros ont des noms plus courts, commençant généralement par `v_`.

### v_get()

v_get() est un alias pour vector_get().

### v_set()

v_set() est un alias pour vector_set().

### v_elem()

v_elem() convertit un pointeur retourné par certaines fonctions en une variable exploitable
```c
int a = v_elem(int, v_get(vector, 0));
//             ^^^
//             type
//        de conversion
```
### v_size()

v_size() permet de connaitre le nombre d'éléments dans un vecteur
```c
int size = v_size(vector);
```

### v_display()

v_diplay() permet d'afficher le contenu du vecteur dans le stdout. 
```c
int arr[5] = {1, 2, 3, 4, 5}
Vector vector = new_vector_from_array(arr, v_type(int), 5);

v_display(vector, int, "%d ; ");
//        ^^^^^^  ^^^  ^^^^^^^
//       vecteur  type  instructions 
//    à afficher  du      de formatage
//               vecteur

/*Ici, v_display affichera "1 ; 2 ; 3 ; 4 ; 5"*/
```

