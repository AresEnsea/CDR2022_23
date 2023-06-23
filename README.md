## Ce git contient les projets CubeIDE suivant : 
* principale utilisé à la coupe "PhobosBoardWifiAndStratV2" 2A/Com/PhobosBoardWifiAndStratV2
* principale avec correction de bug mais lidar non fonctionnel "PhobosMainBoard" 2A/Com/PhobosMainBoard
* actionneur "Project_Ax12" 2A/Project_Ax12
* lidar "LIDAR_Ares_v1_4" 2A/Detection/LIDAR_Ares_v1_4
* ihm "Final" 2A/Affichage/Final

## Setup du WiFi sur la rasp Pi 4 :
Pour configurer la rapsberry Pi 4 en hotspot il faut installer rasbpery Pi OS de préférence en 64 bits par soucis de compatibilité avec certaine librairies python.
Une fois le systeme installé et la rapsberry pi accessible par ssh ou VNC vous pouvez suivre ce tutoriel : https://www.youtube.com/watch?v=laeOmNDE-Ac&list=LL&index=231
Pour configurer le hotspot exactement de la même manière que moi, un fichier txt contient ce que j'ai mis dans les différents fichier de configuration du hotspot au chemin suivant 2A/Com/dhcpcd.txt
Une fois le hotspot configuré vous pourrez faire fonctionner le serveur python qu chemin 2A/Com/PythonServer/serverZMult.py
Ce server permet la communication entre robot quand le bit de poid fort est a 0. Il y a aussi une option debug qui peut être activer par les robot un fois connecté en wifi, cette option permet d'enregistrer un 1 envoi de printf sur 10000 sur un log. Cela permet entre autre de renvoyer des donnnés en continu comme la position et la distance de détection d'un obstacle.
Pour envoyer des commande au robot ou au server il faut passer par un autre fichier python client 
