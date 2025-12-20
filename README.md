\# 🎮 Wordle Solver Game en C



\## 📖 Description

Ce projet implémente le jeu Wordle en langage C avec deux programmes complémentaires :

\- WordleSolver.c : jeu interactif où l’utilisateur tente de deviner un mot secret choisi aléatoirement.

\- game.c : solver automatique qui résout le Wordle en filtrant le dictionnaire selon le feedback (vert/jaune/noir).



Projet réalisé dans le cadre du cours Algorithmes \& Structures de Données en C (ALGO3), par Skender Newfel et Azzouzi Mehdi.



---



\## 📂 Organisation des fichiers

\- WordleSolver.c : jeu interactif (lecture de saisie, feedback en couleurs ANSI, plateau d’essais).

\- game.c : solver automatique (mot initial “soare”, feedback g/y/b, filtrage progressif du dictionnaire).

\- words.txt : dictionnaire de mots (un mot de 5 lettres par ligne).

\- Mini\_Project\_ALGO3\_L2\_ISIL\_C.pdf : énoncé officiel.

\- Analyse\_et\_Documentation.pdf : rapport d’analyse (stratégie, structures, complexité).

\- README.md : ce document.



---



\## ⚙️ Installation et compilation

\- Prérequis : un compilateur C (gcc) et un terminal compatible ANSI.

\- Compilation :

```bash

gcc WordleSolver.c -o wordle

gcc game.c -o solver

▶️ Exécution

Lancer le jeu interactif :

bash



./wordle

Lancer le solver automatique :

bash



./solver



🧩 Exemple d’utilisation

Jeu interactif

Code



Welcome to C Wordle!

Guess the 5-letter word. You have 6 attempts.



Attempt 1/6 - Enter a 5-letter word: apple

. . . . .



Attempt 2/6 - Enter a 5-letter word: crane

c r a n e

Solver automatique

Code



=== WordSolver automatique fiable ===

Mot secret choisi !



&nbsp;soare   -> feedback coloré

&nbsp;crane   -> feedback coloré

&nbsp;...

Mot trouvé :CRANE 

Screenshots :
## 🎮 Captures d'écran
![wordle clone Screenshot](./image/wordle.png).


### Wordle
![Wordle](https://raw.githubusercontent.com/SON_ID/son-projet/main/wordle.png)

### Solver
![Solver](https://raw.githubusercontent.com/SON_ID/son-projet/main/solver.png)

🛠️ Stratégie du solver

Premier mot choisi : "soare", statistiquement optimal pour réduire l’espace de recherche.

À chaque tentative, le solver filtre le dictionnaire en fonction du feedback reçu (lettres vertes, jaunes, noires).

Complexité :

Chargement du dictionnaire : O(N)

Filtrage par tentative : O(N·L) avec L = 5

Sélection du prochain mot : O(N)

Résultats (rapport PDF) : en moyenne ~4,3 tentatives sur un dictionnaire de 2500 mots.



👨‍🏫 Crédits

Auteurs : Skender Newfel, Azzouzi Mehdi

Encadrement : ABADLI Badreddine

Cours : ALGO3 — ISIL L2



📌 Notes

Le projet fonctionne uniquement avec des mots de 5 lettres.

words.txt doit contenir un mot par ligne.

Les couleurs ANSI s’affichent dans les terminaux compatibles.

Le dépôt doit inclure au moins une capture d’écran du programme en exécution (voir section Screenshots).











