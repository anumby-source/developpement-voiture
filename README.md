anumby : [developpement-voiture](https://github.com/anumby-source/developpement-voiture/wiki) [jouets](https://github.com/anumby-source/jouets/wiki) [raspberry-pico](https://github.com/anumby-source/raspberry-pico/wiki) arnaudrco : [exemples-de-montages](https://github.com/arnaudrco/exemples/wiki) [visio](https://github.com/arnaudrco/visio/wiki) [festisol](https://github.com/arnaudrco/festisol/wiki) [CROUS-micro-python](https://github.com/arnaudrco/CROUS-micro-python/wiki) [piano](https://github.com/arnaudrco/piano-pour-adulte-debutant/wiki) Ateliers Numériques [Repair Café d'Orsay](https://www.repaircafe-orsay.org/category/blog/ateliers-numeriques-blog/) 


https://user-images.githubusercontent.com/90700891/155990377-5c4f1362-04a4-40f6-9944-eb9b7a41df36.mp4


# developpement-voiture
developpement autour d'une voiture multicapteur : nous regardons ensemble les évolutions possibles du modèle en partant de cette base simple à fabriquer : construire soi-même le support, ajouter des capteurs, préparer un habitacle avec des éclairages, installer une caméra pour faire de l'IA..




# le kit anumby voiture-robot
Anumby propose aux nouveaux adhérents de monter soi-même sa voiture-robot. 10 nouveaux kits seront disponibles en avril 2022 gratuitement pour les nouveaux membres ou avec une participation de 10 € avec un atelier ouvert à tous d'une journée. La voiture-robot obeit à des ordres simples pour avancer, reculer ou tourner

# Robot multi fonctionnel
De nouveaux capteurs pour la voiture-multicapteur ! Toutes les idées sont bonnes ! Vous pouvez ajouter à la voiture-robot de nouveaux capteurs, un capot lumineux ou refaire le socle. Nous vous aidons à réaliser vos projets

derniers ajouts :

[Détection à ultrasons pour voiture](https://github.com/anumby-source/developpement-voiture/blob/main/esp-ultrason/ultrason.mp4)


développement autour d'une voiture évoluée : Les ateliers de robotique ont beaucoup de succès et ANUMBY a utilisé un kit de montage et un logiciel maison pour réaliser soi-même une voiture commandée par WIFI 

Le codage du logiciel était livré tout prêt pour les nouveaux arrivants parce que l'intérêt immédiat était de repartir après une journée d'initiation avec chacun sa voiture ; nous regardons ensemble les évolutions possibles du modèle en partant de cette base simple à fabriquer : construire soi-même le support (socle), ajouter des capteurs, préparer un habitacle avec des éclairages, installer une caméra pour faire de l'IA..

# le socle

nous utilisons le socle en plexiglas des voitures jouets avec une électronique du commerce calculateur esp8266 et une électronique de commande de puissance L293D ; des variantes sont possibles en utilisant du bois ou du carton plutôt que du plastique

Le socle en bois est réalisé avec fixant les 2 moteurs avec des vis à bois

![affiche](https://github.com/anumby-source/developpement-voiture/blob/main/bois.JPG)

Nous pouvons réaliser un socle en carton solide avec plusieurs épaisseurs de cartons collées ensemble comme le propose Marianne du Repair Café d'Orsay

![affiche](https://github.com/anumby-source/developpement-voiture/blob/main/carton.JPG)

# les capteurs

amusez vous à guider la voiture avec une cellule solaire et 2 diodes clignotantes même dans le noir !

https://user-images.githubusercontent.com/90700891/155979957-6dc05abc-9102-4dac-84f2-ec0a1cbd87c7.mp4

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

## moteurs supplémentaires

![l298](https://user-images.githubusercontent.com/90700891/156392381-5b2c2a66-36c1-4359-bf46-7a4571b0a055.png)

![IRFZ44](https://user-images.githubusercontent.com/90700891/156397244-41fe732b-15b9-498d-b7f8-b292f4cc4a9b.png)

## servo moteurs

https://user-images.githubusercontent.com/90700891/156402292-697312b3-5815-46db-a0f9-fc1f832c97cc.mp4


# utilisation de l'intelligence artificielle (IA) 

Ceci va demander de l'apprentissage de notre part. J'attends la livraison d'un kit Réseau de Neurones associé au kit logiciel OpenVino et une caméra + PI pour me former à cet outillage:
- conduite automatique
- reconnaissance d'un trajet
- reconnaissance de panneaux

références
- OpenVino: https://docs.openvino.ai/latest/index.html
- Kit Réseau neurones: https://docs.rs-online.com/aa5a/A700000007838951.pdf



