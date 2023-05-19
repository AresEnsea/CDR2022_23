/* cherrycount_7seg Author : Ousmane Thiongane */
/* Description : Ce code permet d'afficher sur un afficheur 7 segments le nombre de cerises déposées dans le panier */
/* Afficheur en Anode commune */

//Définition des broches des segments
const byte SEGMENT_A = 4;
const byte SEGMENT_B = 13;
const byte SEGMENT_C = 11;
const byte SEGMENT_D = 3;
const byte SEGMENT_E = 2;
const byte SEGMENT_F = 5;
const byte SEGMENT_G = 10;

//Définition de la broche du compteur et de son état
const byte ButtonPin = A5;
int ButtonState = 0;

//Nombre de cerises
int number = 0;

//Variables de contrôle du temps entre deux détections
unsigned long timeprevious = 0;
unsigned long time_min = 50;

//Broche du moteur 
int in1 = 6;

//Définition des broches des transistors
const int transistor_tens = 8;
const int transistor_units = 7;

//Variable définissant le temps de commande du moteur
double timer = 60000;

//Tableau contenant les valeurs binaires des états des segments pour chaque chiffre
const byte SEGMENTS_STATE[] = {
  0b00111111,
  0b00000110,
  0b01011011,
  0b01001111,
  0b01100110,
  0b01101101,
  0b01111101,
  0b00000111,
  0b01111111,
  0b01101111,
};

void setup(){
  Serial.begin(115200);
 
//Initialisation de tous les segments sur leur état bas
  pinMode(SEGMENT_A, OUTPUT);
  digitalWrite(SEGMENT_A, LOW);
  pinMode(SEGMENT_B, OUTPUT);
  digitalWrite(SEGMENT_B, LOW);
  pinMode(SEGMENT_C, OUTPUT);
  digitalWrite(SEGMENT_C, LOW);
  pinMode(SEGMENT_D, OUTPUT);
  digitalWrite(SEGMENT_D, LOW);
  pinMode(SEGMENT_E, OUTPUT);
  digitalWrite(SEGMENT_E, LOW);
  pinMode(SEGMENT_F, OUTPUT);
  digitalWrite(SEGMENT_F, LOW);
  pinMode(SEGMENT_G, OUTPUT);
  digitalWrite(SEGMENT_G, LOW);
 
//Initialisation des deux transistors sur leur état bas
  pinMode(transistor_tens,OUTPUT);
  digitalWrite(transistor_tens, LOW);
  pinMode(transistor_units,OUTPUT);
  digitalWrite(transistor_units, LOW);

//Initialisation du moteur
  pinMode(in1,OUTPUT);
//Démarre la rotation du moteur
  analogWrite(in1,255); 

//Initialisation du switch de détection 
  pinMode(ButtonPin, INPUT);
}

void loop(){
  //Définition du chiffre des dizaines/unités
  double tens = number/10;
  double units = number%10;

  //Détection d'une cerise supplémentaire
  detection();    
  
  //Affichage du nombre de cerises
  number_display(units, tens);    

  detection();

  //Vérification du comptage dans le Serial Monitor        
  //Serial.println("Cerises :"+ String(number)); 
  
}


//Fonction permettant d'afficher un nombre sur les 7 segments
void number_display(byte tens, byte units) {

    if ((units > 9) or (tens > 9)) //Par sécurité
      return;

    //Affichage des unités
    byte segments = SEGMENTS_STATE[units];
    detection();
    digitalWrite(SEGMENT_A, !bitRead(segments, 0));
    digitalWrite(SEGMENT_B, !bitRead(segments, 1));
    detection();
    digitalWrite(SEGMENT_C, !bitRead(segments, 2));
    digitalWrite(SEGMENT_D, !bitRead(segments, 3));
    detection();
    digitalWrite(SEGMENT_E, !bitRead(segments, 4));
    digitalWrite(SEGMENT_F, !bitRead(segments, 5));
    detection();
    digitalWrite(SEGMENT_G, !bitRead(segments, 6));
    detection();

    //Alternance des deux affichages
    digitalWrite(transistor_tens, HIGH);
    digitalWrite(transistor_units, LOW);
    detection();
    
    for(int i=0; i<50; i++){
      detection();
      }
       
    //Affichage des dizaines
    segments = SEGMENTS_STATE[tens];
    detection();
    digitalWrite(SEGMENT_A, !bitRead(segments, 0));
    digitalWrite(SEGMENT_B, !bitRead(segments, 1));
    detection();
    digitalWrite(SEGMENT_C, !bitRead(segments, 2));
    digitalWrite(SEGMENT_D, !bitRead(segments, 3));
    detection();
    digitalWrite(SEGMENT_E, !bitRead(segments, 4));
    digitalWrite(SEGMENT_F, !bitRead(segments, 5));
    detection();
    digitalWrite(SEGMENT_G, !bitRead(segments, 6));

    //Alternance des deux affichages
    digitalWrite(transistor_tens, LOW);
    digitalWrite(transistor_units, HIGH);
    detection();

    for(int i=0; i<50 ;i++){
      detection();
      }

    //Temps de commande du moteur
    //if(millis() >= timer){
    //  analogWrite(in1,0);
    //}
   
}

//Fonction permettant le comptage du nombre de cerises
void detection(){
  ButtonState = analogRead(A5);
  if(ButtonState >= 900){
    if((millis() - timeprevious) >= time_min){
      number++;  
      timeprevious = millis(); 
    }
  }
}
