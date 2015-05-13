# Projet Embarqué - Xenomai
## Contenu
2 dossiers :
  - socketclientgood
      => dossier à mettre sur la Raspberry Pi
  - socketserveurgood
      => dossier à garder sur le portable

## Mise en place
* Récupérer le code 
```
git clone https://github.com/Mooneba/xenomai.git
```

* Mettre le dossier socketclientgood sur la raspberry Pi
```
scp socketclientgood -r root@<@ip Rasp>:/root/
```

* Effectuer ```make``` dans les deux dossiers

## Exécution
* Exécuter le serveur
```
./serverudp
```

* Récupérer le n° du port que nous donne le serveur

* Exécuter le client (côté Raspberry Pi)
```
./clientudp <@ip PC> <n° port récupéré>
```

* A la fin de l'exécution, afficher le graphe du côté du serveur
```
$socketserveurgood > cd xgraph
$socketserveurgood/xgraph > ./xgraph ../sint.gx
```

## Résultat souhaité
![Image of Sint](https://github.com/Mooneba/xenomai/blob/master/image.png)
