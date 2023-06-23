## Ce git contient les projets CubeIDE suivant : 
* principale utilisé à la coupe "PhobosBoardWifiAndStratV2" 2A/Com/PhobosBoardWifiAndStratV2
* principale avec correction de bug mais lidar non fonctionnel "PhobosMainBoard" 2A/Com/PhobosMainBoard
* actionneur "Project_Ax12" 2A/Project_Ax12
* lidar "LIDAR_Ares_v1_4" 2A/Detection/LIDAR_Ares_v1_4
* ihm "Final" 2A/Affichage/Final

## Setup du WiFi sur la rasp Pi 4 :
Pour configurer la rapsberry Pi 4 en hotspot il faut installer rasbpery Pi OS de préférence en 64 bits par soucis de compatibilité avec certaines librairies python.

Une fois le système installé et la rapsberry pi accessible par ssh ou VNC vous pouvez suivre ce tutoriel : https://www.youtube.com/watch?v=laeOmNDE-Ac&list=LL&index=231

Pour configurer l'hotspot exactement de la même manière que moi, un fichier txt contient ce que j'ai mis dans les différents fichiers de configuration de l'hotspot au chemin suivant 2A/Com/dhcpcd.txt

Une fois l'hotspot configuré vous pourrez faire fonctionner le serveur python au chemin 2A/Com/PythonServer/serverZMult.py

Ce serveur permet la communication entre robots quand le bit de poids fort est à 0. Il y a aussi une option debug qui peut être activée par les robots une fois connectée en Wi-Fi, cette option permet d'enregistrer un envoi de printf sur 10000 au log. Cela permet entre autres de renvoyer des données en continu comme la position et la distance de détection d'un obstacle.

Pour envoyer des commandes au robot ou au serveur il faut passer par un autre fichier python client au chemin 2A/Com/PythonServer/client.py

Ce client est très basique, il peut permettre de lancer le robot à distance et d'arrêter ou relancer le debug. Le but de ce client à l'avenir et de renvoyer des informations issues d'un traitement d'images. 
