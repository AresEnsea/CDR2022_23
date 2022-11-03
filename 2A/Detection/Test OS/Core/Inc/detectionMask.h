float * positionRelative(int number, int pseudoDistance, float * angleDeg);
/*
 * Calcule les positions extremes mesurees par le capteur
 * relatives a la position du robot
 * @param
 * number 			: numero du capteur du la carte de detection
 * pseudoDistance 	: distance normalisee renvoyee par le capteur
 * angleDeg 		: angle du robot sur la table
 *
 * @return
 * 	sortie[4]=[x1,y1,x2,y2], une liste de 4 flottants representant le segment de l'obstacle detecte,
 * 	les 2 points peuvent etre interchanges
 */

float calculSegment(float * XY, float * OA, float * OB);
/*
 * Le but de cette fonction est de mesurer le taux du segment AB qui est a l'interieur de la table
 * @param
 * 	XY: la position du robot
 * 	OA: le point de visibilite relative extreme pour un angle faible
 * 	OB: le point de visibilite relative extreme pour un angle eleve
 *
 * 	@return
 * 	le taux du segment AB a l'interieur de la table entre 0 et 100
 * 	(-1 si le calcul a echoue = tentative de division par 0 a un moment
 * 	ou surtout le point exterieur pas entre 2 droites d'arrete paralleles)
 */

int inTheTable(float * x, float * y);
/*
 * Determine si le point de coordonnees (x,y) est dans la table
 * @param
 * x : abscisse du point
 * y : ordonnee du point
 * @return
 * 1 si le point est dans la table et 0 sinon
 */

float module(float * A, float * B);
/*
 * Calcule la distance entre 2 points
 * @param
 * A : liste de 2 flottants
 * B : liste de 2 flottants
 * @return
 * la distance entre A et B
 */

int between(float * X, float min, float max);
/*
 * Calcule la presence d'un point dans un intervalle
 * @param
 * X : flottant
 * @return
 * 1 si X est entre min et max (inclus) et 0 sinon
 */

void charListWriting(char * cas, char * replacement);
/*
 * Permet de modifier le contenu de chaines de caracteres si replacement est plus courte que cas
 * (idealement de meme taille)
 *
 */
