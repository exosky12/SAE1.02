# SAE1.02

# ⚠ Suppimer ancien ```game.dat``` par précaution 
Bug pourrait survenir car non trié du à ancienne version

Devoir jeu en C

## Description  ```monsters.txt```

```
5
ChouinChouin,1,4,1
Dragonus,3,6,2
PetitePustule,2,6,1
LutinCordial,2,6,1
Chimere,3,6,2
```

Ici le fichier ```monsters.txt``` suis le format suivant:

```
<nombre de monstres>
<monstre1>,<niveau>,<nombre pv>,<nombre dégâts>
<monstre2>,<niveau>,<nombre pv>,<nombre dégâts>
...
```

## Description ```game.dat```

Fichier binaire contenant les données des joueurs

Le format ce présente comme suit:

```
<nombre de joueurs>
--struture de chaque joueur--
```

## Reste à faire

- partie prédéfinie à finir
- ajouter récursivité