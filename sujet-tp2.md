# Travail pratique 2: Extension de l'application `kover`

Ce document décrit le travail pratique 2 du cours [INF3135 Construction et
maintenance de logiciels](https://etudier.uqam.ca/cours?sigle=INF3135) au
trimestre d'hiver 2025. Le cours est enseigné par Alexandre Blondin Massé,
chargé de cours au [département d'informatique](http://info.uqam.ca/),
à l'[Université du Québec à Montréal](https://uqam.ca/).

Le travail doit être remis au plus tard le **dimanche 23 mars à 23h59**.
À partir de minuit, une pénalité de **2 points** par heure de retard sera
appliquée.

## Objectifs pédagogiques

Les principaux objectifs pédagogiques visés sont les suivants:

* Approfondir votre maîtrise du langage de **programmation C**
* Organiser adéquatement votre développement à l'aide des **branches** d'un
  logiciel de contrôle de versions (Git)
* Vous familiariser avec la compilation d'un programme divisé en plusieurs
  modules à l'aide d'un **Makefile**
* Vous habituer à suivre un **style de programmation imposé**
* Rédiger des **tests fonctionnels** couvrant les modifications que vous avez
  apportées à l'application
* Vous assurer de ne pas introduire de **régression** lorsque vous apportez des
  modifications à un programme
* Mettre à jour la **documentation** pour refléter les modifications apportées
  à l'application

## Description du travail

Lors du [travail pratique 1](sujet-tp1.md), vous avez eu à mettre en place les
bases d'une application nommée `kover`. Dans ce deuxième travail, vous devez
apporter différentes modifications à cette application, que ce soit pour des
raisons de maintenance, d'ajout de nouvelles fonctionnalités ou de
réorganisation de la base de code. Afin de s'assurer que tout le monde
travaille à partir de la même base et pour faciliter la correction, nous vous
fournissons la solution du premier travail pratique. Vous devrez donc dupliquer
(*fork*) le dépôt courant dans votre espace personnel, le rendre privé (afin
d'éviter le plagiat) et donner accès aux utilisateurs `blondin_al` et
`guite-vinet.julien` en mode *Maintainer*, comme pour le premier travail.

Vous devez accomplir les 3 tâches suivantes:

1. `validate-scene`: Introduire une sous-commande `validate`, qui permet de
   valider si la scène lue sur l'entrée standard est valide
2. `introduce-house`: Introduire un deuxième type de bâtiment, la *maison*
   (`house`)
3. `cover-quality`: Introduire une notion de *qualité de couverture* d'une
   scène

Ces tâches sont détaillées dans les sections suivantes. Noter qu'elles doivent
être réparties sur des branches nommées exactement `validate-scene`,
`introduce-house` et `cover-quality`, en suivant la syntaxe `kebab-case` pour
la nomenclature.

## Tâches préliminaires à accomplir

Dans un premier temps, vous devez accomplir les actions suivantes:

1. Lisez le sujet du travail jusqu'à la fin
2. Créez un clone (*fork*) du [dépôt du
   projet](https://gitlab.info.uqam.ca/inf31351/20251/inf3135-251-tp2)
3. Assurez-vous que le dépôt soit privé, pour éviter le plagiat
4. Donnez accès à votre dépôt aux utilisateurs `blondin_al` et
   `guite-vinet.julien` en mode *Maintainer* (pour la correction)
5. Familiarisez-vous avec le contenu du dépôt, en étudiant chacun des fichiers
   présents (`README.md`, `sujet-tp1.md`, répertoire `bats`, `.gitlab-ci.yml`,
   etc.).
6. Testez les différentes cibles des fichiers Makefile, notamment `make build`
   et `make test`

Vous êtes maintenant prêt ou prête à commencer les tâches demandées!

## Tâche 1: `validate-scene`

La première tâche consiste essentiellement à réorganiser (réusiner ou
refactoriser) le code déjà existant afin de centraliser la validation
à l'intérieur d'une nouvelle sous-commande `validate`. Autrement dit, la
sous-commande `validate` doit faire seulement la validation actuellement faite
par les sous-commandes `bounding-box`, `describe` et `summarize` et rapporter
le résultat sur la sortie standard, de même qu'un message d'erreur sur la
sortie d'erreur s'il y a lieu, puis retourner un code de retour cohérent. Bien
sûr, les sous-commandes `bounding-box`, `describe` et `summarize` doivent
toujours s'assurer de valider la scène (ce comportement reste inchangé).
En plus de réorganiser le code source, vous devez mettre à jour les tests
fonctionnels (tests Bats) et la documentation du code source (*docstring*) de
même que le fichier `README.md` pour refléter les changements apportés.

Plus spécifiquement, vous devez minimalement apporter les modifications
suivantes, en plaçant tous vos *commits* sur la branche `validate-scene`:

1. Introduire une nouvelle sous-commande `validate`, qui ne prend aucun
   argument supplémentaire et qui lit une scène sur l'entrée standard
2. Lorsque la commande est exécutée, elle doit afficher `ok` ou `not ok` sur la
   première ligne de la sortie standard selon que la scène est valide ou
   invalide.
3. Le code retourné par l'application doit être `0` lorsque la scène est valide
   et `1` lorsqu'elle est invalide.
4. Lorsque la scène est invalide, un message d'erreur expliquant la raison pour
   laquelle la scène est invalide doit être affiché sur la sortie d'erreur, sur la ligne suivant la chaîne `not ok`
5. Réorganiser tous les tests Bats qui portent sur la validation afin d'éviter
   la redondance actuelle. Autrement dit, plutôt que de tester que toutes les
   validation sont faites pour chacune des sous-commandes `bounding-box`,
   `describe` et `summarize`, vous devez déplacer tous les tests fonctionnels
   liés à la validation dans un seul fichier nommé `test_validate.bats`.
6. Mettre à jour les *docstrings* s'il y a lieu, ajouter les *docstrings* aux
   nouvelles fonctions que vous avez introduites.
7. Mettre à jour le fichier `README.md` et l'aide (`kover help`).
8. Vous assurer de respecter le style de programmation existant
9. Répartir vos modifications le plus possible en différents *commits*
   atomiques, en formatant vos messages selon le style demandé dans le cadre du
   travail pratique 1

## Tâche 2: `introduce-house`

La deuxième tâche consiste à introduire un type de bâtiment résidentiel, qu'on
appellera simplement *maison* (`house`). Lors de la lecture d'une scène, il
y aura donc trois types de ligne qui pourront être lues: `antenna`, `building`
et `house`. Les paramètres qui déterminent une maison sont les mêmes que les
*buildings* (elles sont aussi de forme rectangulaire). Il faudra mettre à jour
les commandes existantes (`bounding-box`, `describe`, `summarize` et
`validate`) lorsqu'elles rapportent des informations sur la sortie standard.

Plus spécifiquement, vous devez minimalement apporter les modifications
suivantes, en plaçant tous vos *commits* sur la branche `introduce-house`:

1. Étendre le module `scene` (`scene.h`/`scene.c`) afin de supporter ces
   nouvelles modifications
2. Mettre à jour les tests fonctionnels (tests Bats) existants
3. Ajouter de nouveaux tests fonctionnels reflétant les modifications apportées
4. Mettre à jour la documentation (l'aide `kover help`, les *docstrings* des
   fonctions impactées, le fichier `README.md`) pour refléter les changements
   apportés
5. Vous assurer de respecter le style de programmation existant
6. Répartir vos modifications le plus possible en différents *commits*
   atomiques, en formatant vos messages selon le style demandé dans le cadre du
   travail pratique 1

## Tâche 3: `cover-quality`

Finalement, votre troisième tâche consiste à introduire une nouvelle
sous-commande `quality`, qui rapporte sur la sortie standard la qualité de la
couverture actuelle des *buildings* de la scène à l'aide des antennes.

Tout d'abord, étant donné un point $`P`$ du plan, on dit que $`P`$ est
*couvert* s'il existe au moins une antenne $`A`$ de rayon $`r`$ telle que $`P`$
se trouve à une distance au plus $`r`$ de $`A`$. Autrement dit, $`P`$ se trouve
dans le disque centré sur $`A`$ de rayon $`r`$.

Pour déterminer la qualité actuelle de la couverture d'un bâtiment, on utilise
la classification suivante:

- `A`: chacun des quatre coins du bâtiment est couvert
- `B`: exactement trois coins sur quatre du bâtiment sont couverts
- `C`: exactement deux coins sur quatre du bâtiment sont couverts
- `D`: exactement un coin sur quatre du bâtiment est couvert
- `E`: aucun des quatre coins du bâtiment n'est couvert

L'application `kover` doit donc rapporter sur la sortie standard le niveau de
couverture (`A`, `B`, `C`, `D` ou `E`) individuel des bâtiments. Par exemple,
imaginons qu'on ait la scène suivante:

```sh
$ cat example.scene
begin scene
  building b1 0 0 1 1
  house h1 2 4 1 1
  house h2 3 1 1 1
  antenna a1 0 3 4
end scene
```

Alors on s'attend donc au comportement suivant:

```sh
$ bin/kover quality < example.scene
building b1: C
house h1: A
house h2: C
```

Plus spécifiquement, vous devez minimalement apporter les modifications
suivantes, en plaçant tous vos *commits* sur la branche `cover-quality`:

1. Étendre la base de code afin de supporter cette nouvelle sous-commande
2. Mettre à jour les tests fonctionnels (tests Bats) existants s'il y a lieu
3. Ajouter de nouveaux tests fonctionnels reflétant les modifications apportées
4. Mettre à jour la documentation (l'aide `kover help`, les *docstrings* des
   fonctions impactées, le fichier `README.md`) pour refléter les changements
   apportés
5. Vous assurer de respecter le style de programmation existant
6. Répartir vos modifications le plus possible en différents *commits*
   atomiques, en formatant vos messages selon le style demandé dans le cadre du
   travail pratique 1

## Clone et création du dépôt

Comme pour le travail pratique 1, vous devez cloner le dépôt fourni et
l'héberger sur la plateforme [Gitlab du
département](https://gitlab.info.uqam.ca/). Votre dépôt devra se nommer
**exactement** `inf3135-251-tp2` et l'URL devra être **exactement**
`https://gitlab.info.uqam.ca/<utilisateur>/inf3135-251-tp2`, où `<utilisateur>`
doit être remplacé par votre nom d'utilisateur. Il devra être **privé** et
accessible seulement par vous et par les utilisateurs `blondin_al` et
`guite-vinet.julien`.

## Git

Il est important de continuer à utiliser correctement Git pour suivre
l'évolution de votre projet. N'oubliez pas de bien configurer correctement
votre fichier `.gitconfig` qui permet de vous identifier comme auteur de
*commits*, en y indiquant vos **véritables** prénom, nom et courriel. Comme
pour le travail pratique 1, les messages de *commit* doivent suivre [la
convention de Chris Beams](https://chris.beams.io/posts/git-commit/), adaptée
au français:

1. Séparer le sujet (la première ligne) du corps (le reste du message) par une
   ligne vide
2. Limiter le sujet à 50 caractères
3. Commencer le message par une lettre majuscule
4. Ne pas terminer le sujet avec un point
5. Utiliser un verbe à l'indicatif présent comme premier mot du sujet
6. Limiter les lignes du corps du message à 72 caractères
7. Utiliser le corps pour expliquer *ce que* (*what*) la modification apporte
   et *pourquoi* (*why*) elle apporte cette modification, plutôt que *comment*
   (*how*) elle apporte une modification

Vous pouvez vous inspirer des messages de *commit* déjà existants dans
l'historique. Aussi, n'oubliez pas de mettre les fichiers `.gitignore` du dépôt
si nécessaire en fonction de votre environnement de développement. Assurez-vous
de ne pas versionner de fichiers inutiles (les fichiers binaires, entre autres,
mais pas seulement).

**Attention!**. Une différence notable entre le premier travail pratique et
celui-ci est que vous ne devez pas modifier la branche `master`. Plutôt, vous
devez remettre trois branches nommées `validate-scene`, `introduce-house` et
`cover-quality`, sans les fusionner sur la branche `master`.

## Langue

Comme pour le premier travail pratique, le projet est développé dans deux
langues, soit le français et l'anglais. On rappelle les règles suivantes:

* Tous les messages de *commit* doivent être rédigés en français
* Le fichier `README.md` doit être rédigé en français
* Les commentaires et la documentation du code source (fichier `.c`, fichier
  `.bats`, etc.) doivent être rédigés en anglais

Cette convention n'est pas universelle, mais elle est souvent employée dans les
milieux bilingues.

## Barème

Les points sont répartis selon la grille suivante:

| Élément             |  Points |
|:--------------------|--------:|
| Tâche 1             |     /30 |
| Tâche 2             |     /30 |
| Tâche 3             |     /40 |
| Utilisation de Git  |     /30 |
| Total               |    /130 |

Plus précisément, les éléments suivants seront pris en compte:

- **Tâche 1 (30 points)**: L'application compile sans erreur et sans
  avertissement. La mémoire est gérée correctement. Elle introduit la
  sous-commande `validate`. Elle affiche les informations demandées sur les
  bons canaux (`stdout` et `stderr`) et retourne la bonne valeur (`0` ou `1`).
  Les tests fonctionnels existants ont été mis à jour pour refléter les
  modifications. Des nouveaux tests ont été ajoutés pour refléter l'ajout d'une
  nouvelle sous-commande. Les *docstrings* du code source ont été mises à jour
  ou ajoutées aux nouvelles fonctions. Le fichier `README.md` et l'aide (`kover
  help`) ont été mis à jour. Le style de programmation existant est respecté.

- **Tâche 2 (30 points)**: L'application compile sans erreur et sans
  avertissement. La mémoire est gérée correctement. Elle introduit le nouveau
  type de bâtiment (`house`). La lecture d'une scène et l'affichage
  d'informations a été ajusté en conséquence. Les tests fonctionnels existants
  ont été mis à jour pour refléter les modifications. Des nouveaux tests ont
  été ajoutés pour refléter l'ajout du nouveau type de bâtiment. Les
  *docstrings* du code source ont été mises à jour ou ajoutées aux nouvelles
  fonctions. Le fichier `README.md` et l'aide (`kover help`) ont été mis
  à jour. Le style de programmation existant est respecté.

- **Tâche 3 (40 points)**: L'application compile sans erreur et sans
  avertissement. La mémoire est gérée correctement. Elle introduit la nouvelle
  sous-commande `quality`. Elle rapporte correctement le niveau de couverture
  individuel des bâtiments d'une scène. Les tests fonctionnels existants ont
  été mis à jour pour refléter les modifications. Des nouveaux tests ont été
  ajoutés pour refléter l'ajout de la nouvelle sous-commande. Les *docstrings*
  du code source ont été mises à jour ou ajoutées aux nouvelles fonctions. Le
  fichier `README.md` et l'aide (`kover help`) ont été mis à jour. Le style de
  programmation existant est respecté.

- **Utilisation de Git (30 points)**: Les modifications sont réparties en
  *commits* atomiques. Le fichier `.gitignore` est complet. Les messages de
  *commit* sont significatifs, uniformes et suivent la convention suggérée.
  Les *commits* sont répartis sur des branches nommées respectivement
  `validate-scene`, `introduce-house` et `cover-quality`. Chaque *commit* est
  pertinent pour la branche sur laquelle il se trouve. Les branches sont
  organisées dans l'ordre des tâches demandées. Noter que si vous n'avez pas
  beaucoup avancé dans certaines des tâches demandées, vous ne pourrez pas
  obtenir beaucoup de points sur ce critère.

## Pénalités

Si votre programme ne compile pas pour une tâche donnée, une pénalité pouvant
aller jusqu'à **100%** pourra être appliquée pour la tâche en question.

**Note**: vous n'avez pas besoin d'insérer votre code permanent dans le fichier
`README.md`: nous avons établi l'association entre votre nom d'utilisateur et
votre code permanent dans le premier travail pratique.

En outre, si vous ne respectez pas les critères suivants, une pénalité pouvant
aller jusqu'à **50%** pourra être appliquée:

- Votre dépôt doit se nommer **exactement** `inf3135-251-tp2`
- L'URL de votre dépôt doit être **exactement**
  `https://gitlab.info.uqam.ca/<utilisateur>/inf3135-251-tp2` où
  `<utilisateur>` doit être remplacé par votre identifiant
- Les utilisateurs `blondin_al` et `guite-vinet.julien` doivent avoir accès
  à votre projet en mode *Maintainer*.
- Votre dépôt doit être un *fork* du [gabarit
  fourni](https://gitlab.info.uqam.ca/inf31351/20251/inf3135-251-tp2).
- Votre dépôt doit être **privé**.

## Remise

La remise se fait automatiquement en ajoutant les utilisateurs `blondin_al` et
`guite-vinet.julien` en mode *Maintainer*, vous n'avez rien de plus à faire.
À moins d'indication contraire de votre part, ce sera le dernier (*commit*)
disponible sur chacune de vos branches qui sera considéré pour la correction.

Le travail doit être remis au plus tard le **dimanche 23 mars à 23h59**.
À partir de minuit, une pénalité de **2 points** par heure de retard sera
appliquée.
