/*
 * tests.c
 *
 *  Created on: 29 oct. 2022
 *      Author: Victor Lesne
 */
#include "tests.h"


void tests(void){
	//test_fcts_annexes_calcSeg();
	//test_masque();
}


// Valide
void test_cardinal(void){
	float * sortie1;
	float * sortie2;
	float * sortie3;
	float ag = 90;
	float z = 0;

	sortie1 = positionRelative(0,3,&z);
	float X1 = sortie1[0];
	float Y1 = sortie1[1];
	sortie2 = positionRelative(4,3,&z);
	float X2 = sortie2[0];
	float Y2 = sortie2[1];
	sortie3 = positionRelative(0,3,&ag);
	float X3 = sortie3[0];
	float Y3 = sortie3[1];


}

// Valide
void test_OA_et_OB(void){
	float * sortie1;
	float * sortie2;
	float * sortie3;
	float ag = 90;
	float z = 0;

	sortie1 = positionRelative(0,3,&z);
	float AX1 = sortie1[0];
	float AY1 = sortie1[1];
	float BX1 = sortie1[2];
	float BY1 = sortie1[3];
	sortie2 = positionRelative(4,3,&z);
	float AX2 = sortie2[0];
	float AY2 = sortie2[1];
	float BX2 = sortie2[2];
	float BY2 = sortie2[3];
	sortie3 = positionRelative(0,3,&ag);
	float AX3 = sortie3[0];
	float AY3 = sortie3[1];
	float BX3 = sortie3[2];
	float BY3 = sortie3[3];
}

// Valide
void test_fcts_annexes_calcSeg(void){
	float A[2] = { 1., 0.};
	float B[2] = {0., 0.};

	int verif = between(&A[0], 0, 2);
	verif = between(&A[0], 0, 0.5);

	float valeur = module(&A, &B);
	A[1] = 1;
	valeur = module(&A, &B);

	verif = inTheTable(&A[0], &A[1]);
	A[0] = 400;
	verif = inTheTable(&A[0], &A[1]);
	A[0] = -2;
	verif = inTheTable(&A[0], &A[1]);
	A[0] = 1; A[1] = 301;
	verif = inTheTable(&A[0], &A[1]);
	A[0] = 201; A[1] = 150;
	verif = inTheTable(&A[0], &A[1]);

	char cas[4];
	charListWriting(&cas, "Ay=0");

}

// Valide
void test_masque(void){
	float A[2] = { -100., 0.};
	float B[2] = {100., 100.};
	float posRob[2] = {5., 5.};

	float taux = calculSegment(&posRob, &A, &B);

	A[0] = 100.; A[1] = 100.;
	B[0] =  -100.; B[1] = 0.;
	taux = calculSegment(&posRob, &A, &B);

	A[0] = 100.; A[1] = 100.;
	B[0] =  -100.; B[1] = -100.;
	taux = calculSegment(&posRob, &A, &B);

}
