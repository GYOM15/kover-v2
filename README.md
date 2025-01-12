# `kover`: gestion du positionnement d'antennes de communication

Ce dépôt contient les fichiers nécessaires à la construction d'une application
en ligne de commande appelée `kover`, qui sert à optimiser le positionnement
d'antennes de communication pour une zone donnée.

## Introduction

L'application `kover` est une application en ligne de commande qui facilite le
positionnement d'*antennes* de communication dans une *scène* afin de desservir
des clients de façon adéquate. Les emplacements des clients sont représentés
par des *buildings*, qui correspondent simplement à des boîtes rectangulaires.

Plus formellement, un *building* est représenté par les éléments suivants:

* `id`: un *identifiant* unique, sous forme de chaîne de caractères;
* `x` et `y`: une *position* $`(x,y)`$ dans le plan, sous forme de deux entiers
  (négatifs, nuls ou positifs);
* `rx` et `ry`: des *rayons* horizontal et vertical $`(r_x, r_y)`$, qui sont
  des entiers strictement positifs.

Une *antenne* est représentée par les éléments suivants:

* `id`: un *identifiant* unique, sous forme de chaîne de caractères;
* `x` et `y`: une *position* $`(x,y)`$ dans le plan, sous forme de deux entiers
  (négatifs, nuls ou positifs);
* `r`: un rayon (ou une *portée*) $`r`$, qui est un entier strictement
  positif.

Ainsi, un *building* ne peut pas avoir une aire nulle et une antenne a toujours
une portée décrivant un disque d'aire strictement positive. Finalement, une
*scène* est représentée par les éléments suivants:

* `buildings`: une collection de buildings qui ne se chevauchent pas,
  c'est-à-dire qu'une scène ne peut contenir deux buildings dont l'intersection
  occupe une aire non nulle.
* `antennas`: une collection d'antennes qui occupent des positions distinctes.

Pour décrire une scène à l'aide d'un flux de texte, on convient d'utiliser une
syntaxe spécifique:

1. La première ligne du texte doit être exactement `begin scene`, en minuscules;
2. La dernière ligne du texte doit être exactement `end scene`, en minuscules;
3. Chaque ligne entre la première ligne et la dernière ligne doit être une
   ligne décrivant un *building* ou une ligne décrivant une *antenne*;
4. Une ligne décrivant un building doit être de la forme `building ID X Y RX
   RY`, où `building` est en minuscules, `ID` est l'identifiant du building,
   (`X`, `Y`) est la position du building et (`RX`, `RY`) est une paire de
   rayons horizontal et vertical;
5. Une ligne décrivant une antenne doit être de la forme `antenna ID X Y R`, où
   `antenna` est en minuscules, `ID` est l'identifiant de l'antenne,
   (`X`, `Y`) est la position de l'antenne et `R` est la portée de l'antenne.

Par exemple, le flux de texte suivant décrit une scène composée de deux
buildings (identifiés par `b1` et `b2`) et d'une antenne (identifiée par `a`):

```
begin scene
  building b1 0 0 1 1
  building b2 7 8 2 3
  antenna a 5 4 6
end scene
```

L'application `kover` permet donc de manipuler des scènes et d'optimiser le
positionnement d'antennes dans ces scènes afin de couvrir adéquatement les
buildings qui occupent cette scène.

## Installation

### Dépendances

La construction de l'application dépend des composantes suivantes:

* [GCC](https://gcc.gnu.org/): le compilateur C de GNU. Celui-ci peut être
  installé à l'aide d'un gestionnaire de paquets
* [Make](https://www.gnu.org/software/make/): un outil en ligne de commande
  facilitant la mise en place de tâches automatiques. Cet outil peut aussi être
  installé à l'aide d'un gestionnaire
  de paquets.
* [Bats](https://github.com/bats-core/bats-core): une suite d'application
  facilitant la mise en place de tests unitaires shell. Il n'est pas nécessaire
  d'installer Bats, qui est livré avec ce dépôt dans le répertoire `bats`

### Construction (*build*)

Une fois les dépendances installées, on peut compiler l'application `kover`
à l'aide de la commande `make`:

```sh
$ make
```

Cette commande produit l'exécutable `kover` dans le répertoire `bin`.

Il est possible en tout temps de supprimer l'exécutable:

```sh
$ make clean
```

### Tests

On peut aussi lancer la suite de tests Bats à l'aide de `make`:

```sh
$ make test
```
