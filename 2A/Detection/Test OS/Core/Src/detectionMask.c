#include "detectionMask.h"
#include <math.h>

#define pi 3.1416
#define pasAngulaire (1/16.)*2*pi
#define pasDistance 64/(10.*4.)
#define rayonCapteurs 4.8
#define deltaAngle (13/360.)*2*pi


float * positionRelative(int number, int pseudoDistance, float * angleDeg){
	float pas = pasDistance;				//Pas entre 2 incréments de distance mesuree
	float distance = pas*pseudoDistance;	//Distance reelle de l'obstacle
	float rayon = rayonCapteurs;			//Rayon de la tourelle
	float angleRob = 2*(float)pi*(*angleDeg)/360.;		//Angle relatif du robot
	float angleCapt = (float)number*pasAngulaire;	//Anlgle du capteur par rapport a l'avant du robot

	float OP[2];		//Position du point suivant la normale du capteur
	float gamma[2];		//Vecteur unitaire ortogonal à la normale
	float OA[2];		//Vecteur de position maximale de detection dans le sens trigo
	float OB[2];		//Vecteur de position maximale de detection dans le sens horaire

	
	static float  sortie[4];	//

	//Calcul du point P
	OP[0] = (rayon + distance)*cos(angleRob + angleCapt);
	OP[1] = (rayon + distance)*sin(angleRob + angleCapt);

	//Creation de gamma
	gamma[0] = cos(angleRob + angleCapt + pi/2.);
	gamma[1] = sin(angleRob + angleCapt + pi/2.);

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


#define longueurTable 300.
#define hauteurTable 200.


float calculSegment(float * XY, float * OA, float * OB){
	float A[2];
	float B[2];

	A[0] = XY[0] + OA[0];
	A[1] = XY[1] + OA[1];

	B[0] = XY[0] + OB[0];
	B[1] = XY[1] + OB[1];

	char cas[4] = "0000";
	/* Definit le cas de figure avec un caractère nul au milieu pour plus de lisibilite
	 * 1 : Nom du point en dehors de la table (A ou B)
	 * 2 : Direction de la droite d'intersection avec la table (x ou y)
	 * 3 : Confirmation de l'identification de l'intersection si mis a '=', '0' par defaut
	 * 4 : Valeur de la droite d'intersection (0, L ou H)
	*/

	// On elimine d'abord le cas ou les 2 points sont dans la table
	if(inTheTable(&A[0],&A[1]) && inTheTable(&B[0],&B[1]))
		return 100.;
	// On elimine ensuite le cas ou les 2 points sont en dehors de la table
	if(!inTheTable(&A[0],&A[1]) && !inTheTable(&B[0],&B[1]))
		return 0.;

	if(!inTheTable(&A[0],&A[1]))
		cas[0] = 'A';
	if(!inTheTable(&B[0],&B[1]))
		cas[0] = 'B';

	float distanceAB = module(&A, &B);

	// Vecteur de direction de A vers B, non normalise pour reduire les erreurs
	float u[2];
	u[0] = (B[0] - A[0]);
	u[1] = (B[1] - A[1]);

	// On identifie le bord qui coupe le segment
	switch(cas[0]){
		case 'A':
			if(A[0] < 0 && between(&A[1], 0, hauteurTable)){
				charListWriting(&cas, "Ay=0");
			}
			if(A[0] > longueurTable && between(&A[1], 0., hauteurTable)){
				charListWriting(&cas, "Ay=H");
			}
			if(A[1] < 0 && between(&A[0], 0., longueurTable)){
				charListWriting(&cas, "Ax=0");
			}
			if(A[1] > hauteurTable && between(&A[0], 0., longueurTable)){
				charListWriting(&cas, "Ax=L");
			}
		case 'B':
			if(B[0] < 0 && between(&B[1], 0., hauteurTable)){
				charListWriting(&cas, "By=0");
			}
			if(B[0] > longueurTable && between(&B[1], 0., hauteurTable)){
				charListWriting(&cas, "By=H");
			}
			if(B[1] < 0 && between(&B[0], 0., longueurTable)){
				charListWriting(&cas, "Bx=0");
			}
			if(B[1] > hauteurTable && between(&B[0], 0, longueurTable)){
				charListWriting(&cas, "Bx=L");
			}
	}


	if(cas[2] == '0'){
		return -1.; // Intersection non trouvée
	}


	float da; // Distance du point d'intersection au point A

	// On place le point d'intersection C sur le bord de la table
	switch(cas[1]){
		case 'x':
			if(u[1] == 0.)
				return -1.;// On evite la division par 0

			if(cas[3]=='L'){
				da = distanceAB*(longueurTable - A[1])/u[1];
			}
			if(cas[3]=='0'){
				da = distanceAB*(0 - A[1])/u[1];
			}
		case 'y':
			if(u[0] == 0.)
				return -1.;// On evite la division par 0

			if(cas[3]=='H'){
				da = distanceAB*(hauteurTable - A[0])/u[0];
			}
			if(cas[3]=='0'){
				da = distanceAB*(0 - A[0])/u[0];
			}
	}

	switch(cas[0]){
		case'A':// La distance AC est a l'exterieur de la table
			return 100.*(distanceAB-da)/distanceAB;
		case 'B':// La distance AC est a l'interieur de la table
			return 100.*da/distanceAB;
	}



}


int inTheTable(float * x, float * y){
	if(*x >= 0 && *x <= longueurTable){
		if(*y >= 0 && *y <= hauteurTable){
			return 1;
		}
	}
	return 0;
}


float module(float * A, float * B){
	return sqrt(pow(A[0] - B[0], 2) + pow(A[1] - B[1], 2));
}


int between(float * X, float min, float max){
	if(*X >= min && *X <= max)
		return 1;
	else
		return 0;
}


void charListWriting(char * cas, char * replacement){
	char * index;
	int i = 0;
	while(*replacement != '\0'){
		*cas = *replacement;
		cas++; replacement++; i++;
	}
}
