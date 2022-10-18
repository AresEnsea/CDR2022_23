#include "detectionMask.h"
#include <math.h>

#define pi 3.14
#define pasAngulaire (16/360)*2*pi
#define pasDistance 25
#define rayonCapteurs 4.8
#define deltaAngle (20/360)*2*pi


float * positionRelative(int number, int pseudoDistance, float angleDeg){
	float pas = pasDistance;				//Pas entre 2 incréments de distance mesuree
	float distance = pas*pseudoDistance;	//Distance reelle de l'obstacle
	float rayon = rayonCapteurs;			//Rayon de la tourelle
	float angleRob = 2*pi*angleDeg/360;		//Angle relatif du robot
	float angleCapt = number*pasAngulaire;	//Anlgle du capteur par rapport a l'avant du robot

	float OP[2];		//Position du point suivant la normale du capteur
	float gamma[2];	//Vecteur unitaire ortogonal à la normale
	float OA[2];		//VECTEUR de position maximale de detection dans le sens trigo
	float OB[2];		//VECTEUR de position maximale de detection dans le sens horaire


	//float Xa, Ya;		//Position maximale de detection dans le sens trigo
	//float Xb, Yb;		//Position maximale de detection dans le sens horaire
	
	static float  sortie[4];	//

	//Calcul du point P
	OP[0] = (rayon + distance)*cos(angleRob + angleCapt);
	OP[1] = (rayon + distance)*sin(angleRob + angleCapt);

	//Creation de gamma
	gamma[0] = cos(angleRob + angleCapt + pi/2);
	gamma[1] = sin(angleRob + angleCapt + pi/2);

	//Calcul des points maximaux
	//OA = OP - gamma*distance*tan(deltaAngle);
	OA[0] = OP[0] - gamma[0]*distance*tan(deltaAngle);
	OA[1] = OP[1] - gamma[1]*distance*tan(deltaAngle);

	//OB = OP + gamma*distance*tan(deltaAngle);
	OB[0] = OP[0] + gamma[0]*distance*tan(deltaAngle);
	OB[1] = OP[1] + gamma[1]*distance*tan(deltaAngle);

	sortie[0] = OA[0];
	sortie[1] = OA[1];

	sortie[2] = OB[0];
	sortie[3] = OB[1];

	return sortie;
}
