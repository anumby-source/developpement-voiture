# developpement-voiture
developpement autour d'une voiture multicapteur : nous regardons ensemble les évolutions possibles du modèle en partant de cette base simple à fabriquer : construire soi-même le support, ajouter des capteurs, préparer un habitacle avec des éclairages, installer une caméra pour faire de l'IA..

# le kit anumby voiture-robot
Anumby propose aux nouveaux adhérents de monter soi-même son voiture-robot. 10 kits seront disponibles en avril 2022 gratuitement pour les nouveaux membres ou avec une participation de 10 € avec un atelier ouvert à tous d'une journée. La voiture-robot obeit à des ordres simples pour avancer, reculer ou tourner

# de nouveaux capteurs poiur la voiture-multicapteur
Toutes les idées sont bonnes ! Vous pouvez ajouter de nouveaux capteurs à la voiture

derniers ajouts :

[Détection à ultrasons pour voiture](https://github.com/anumby-source/developpement-voiture/blob/main/esp-ultrason/ultrason.mp4)


développement autour d'une voiture évoluée : Les ateliers de robotique ont beaucoup de succès et ANUMBY a utilisé un kit de montage et un logiciel maison pour réaliser soi-même une voiture commandée par WIFI 

Le codage du logiciel n'a pas intéressé les nouveaux arrivants parce que l'intérêt immédiat était de repartir après une journée d'initiation avec chacun sa voiture ; nous regardons ensemble les évolutions possibles du modèle en partant de cette base simple à fabriquer : construire soi-même le support (socle), ajouter des capteurs, préparer un habitacle avec des éclairages, installer une caméra pour faire de l'IA..

# le socle

nous utilisons le socle en plexiglas des voitures jouets avec une électronique du commerce calculateur esp8266 et une électronique de commande de puissance L293D ; des variantes sont possibles en utilisant du bois ou du carton plutôt que du plastique

Le socle en bois est réalisé avec fixant les 2 moteurs avec des vis à bois

![affiche](https://github.com/anumby-source/developpement-voiture/blob/main/bois.JPG)

Nous pouvons réaliser un socle en carton solide avec plusieurs épaisseurs de cartons collées ensemble comme le propose Marianne du Repair Café d'Orsay

![affiche](https://github.com/anumby-source/developpement-voiture/blob/main/carton.JPG)

# les capteurs

## utilisation d'accéléromètres 

ce sont des composants très répandus, économiques et efficaces 

[utiliser un accéléromètre](https://github.com/arnaudrco/exemples/tree/main/de-accelerometre) :  les 6 DOF ( 3 degrés de liberté en position x y z + 3 angles ), 9 DOF ( avec magnétisme ), 10 DOF pour les drones avec la pression : 

Développement de Michel : En le bougeant j'animais des leds, je crois me souvenir que la couleur dépendait de la direction, et l'intensité de l'accélération.

## caméléon

Le capteur de couleurs peut être amélioré en utilisant le gamma, la couleur de température ou un algorithme de plus proche voisin pour afficher de meilleures couleurs 

[un caméleon avec 3 méthodes de colorimétrie](https://github.com/arnaudrco/exemples/blob/main/cameleon/read-me-cameleon.pdf)


## proximètre

[Détection à ultrasons pour voiture](https://github.com/anumby-source/developpement-voiture/blob/main/esp-ultrason/ultrason.mp4)



## GPS

![affiche](https://github.com/anumby-source/developpement-voiture/blob/main/gps.png)

## capteurs solaires

[une voiture pour suivre une ligne blanche](https://github.com/anumby-source/jouets/tree/main/voiture-eclairee)

# utilisation de l'intelligence artificielle (IA) 

Ceci va demander de l'apprentissage de notre part. J'attends la livraison d'un kit Réseau de Neurones associé au kit logiciel OpenVino et une caméra + PI pour me former à cet outillage:
- conduite automatique
- reconnaissance d'un trajet
- reconnaissance de panneaux

références
- OpenVino: https://docs.openvino.ai/latest/index.html
- Kit Réseau neurones: https://docs.rs-online.com/aa5a/A700000007838951.pdf



