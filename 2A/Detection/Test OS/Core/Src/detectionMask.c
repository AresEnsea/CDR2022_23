#include "detectionMask.h"
#include <math.h>

#define pasAngulaire (16/360)*2*pi
#define pasDistance 25
#define rayonCapteurs 4.8


double * positionRelative(int number, int pseudoDistance, float angleDeg){
	float pas = pasDistance;				//Pas entre 2 incréments de distance mesuree
	float distance = pas*pseudoDistance;	//Distance reelle de l'obstacle
	float rayon = rayonCapteurs;			//Rayon de la tourelle
	float angleRob = 2*pi*angleDeg/360;		//Angle relatif du robot
	float angleCapt = number*pasAngulaire;	//Anlgle du capteur par rapport a l'avant du robot

	double OP[2];		//Position du point suivant la normale du capteur
	double gamma[2];	//Vecteur unitaire ortogonal à la normale
	double OA[2];		//VECTEUR de position maximale de detection dans le sens trigo
	double OB[2];		//VECTEUR de position maximale de detection dans le sens horaire


	double Xa, Ya;		//Position maximale de detection dans le sens trigo
	double Xb, Yb;		//Position maximale de detection dans le sens horaire
	
	double * sortie[2];	//

	//Calcul du point P
	OP[0] = (rayon + distance)*cos(angle + angleCapt);
	OP[1] = (rayon + distance)*sin(angle + angleCapt);

	//Creation de gamma
	gamma[0]=cos(angle + angleCapt + pi/2);
	gamma[1]=sin(angle + angleCapt + pi/2);

	//Calcul des points maximaux
	OA = OP - gamma*distance*tan(deltaAngle);
	OB = OP + gamma*distance*tan(deltaAngle);

	*sortie = OA;
	sortie++; *sortie = OB;

	return sortie;
}
