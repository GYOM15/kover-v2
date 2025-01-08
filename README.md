# `kover`: gestion du positionnement d'antennes de communication

Ce dépôt contient les fichiers nécessaires à la construction d'une application
en ligne de commande appelée `kover`, qui sert à optimiser le positionnement
d'antennes de communication pour une zone donnée.

## Dépendances

La construction de l'application dépend des composantes suivantes:

* **GCC**: le compilateur C de GNU. Celui-ci peut être installé à l'aide d'un
  gestionnaire de paquets
* **Make**: l'outil en ligne de commande facilitant la mise en place de tâches
  automatiques. Cet outil peut aussi être installé à l'aide d'un gestionnaire
  de paquets.
* **Bats**: une suite d'application facilitant la mise en place de tests
  unitaires shell. Il n'est pas nécessaire d'installer Bats, qui est livré avec
  ce dépôt dans le répertoire `bats`
