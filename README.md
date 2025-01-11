# `kover`: gestion du positionnement d'antennes de communication

Ce dépôt contient les fichiers nécessaires à la construction d'une application
en ligne de commande appelée `kover`, qui sert à optimiser le positionnement
d'antennes de communication pour une zone donnée.

## Dépendances

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

## Construction

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

## Tests

On peut aussi lancer la suite de tests Bats à l'aide de `make`:

```sh
$ make test
```
