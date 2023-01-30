/* cherrycount_7seg Author : Ousmane Thiongane */
/* Ce code permet d'afficher sur un afficheur 7 segments le nombre de cerises déposées dans le panier */
/* Afficheur Anode commune */

//Définition des broches
const byte SEGMENT_A = 2;
const byte SEGMENT_B = 10;
const byte SEGMENT_C = 4;
const byte SEGMENT_D = 5;
const byte SEGMENT_E = 6;
const byte SEGMENT_F = 7;
const byte SEGMENT_G = 8;

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

//Définition des interrupteurs (temporaire) 
int lswitch = 0;
int rswitch = 0;

void setup() {
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


  //Initialisation des interrupteurs
  pinMode(13, INPUT);
  pinMode(12, INPUT);

}

void loop() {
  
  static byte units = 0;
  static byte tens = 0;



  lswitch = digitalRead(13);
  rswitch = digitalRead(12);

  //Appui bouton droit -> Incrémentation de la valeur chiffrée de 1
  if ((rswitch == HIGH) and (lswitch == LOW)){
    units++;
    Serial.print(tens);
    Serial.println(units);
    delay(250);
    if (units >= 10){
      units = 0;
      tens++;
    }
  }

  //Appui bouton gauche -> Décrémentation de la valeur chiffrée de 1
  else if ((lswitch == HIGH) and (rswitch == LOW)){
    units--;
    delay(250);
    Serial.print(tens);
    Serial.println(units);
    if (units == 255){
      units = 9;
      tens--;
    }
  }

  number_display(units);

  


}

//Fonction permettant d'afficher un nombre sur les 7 segments
void number_display(byte units) {

    if (units > 9) //Par sécurité
      return; 

    byte segments = SEGMENTS_STATE[units];

    digitalWrite(SEGMENT_A, !bitRead(segments, 0));
    digitalWrite(SEGMENT_B, !bitRead(segments, 1));
    digitalWrite(SEGMENT_C, !bitRead(segments, 2));
    digitalWrite(SEGMENT_D, !bitRead(segments, 3));
    digitalWrite(SEGMENT_E, !bitRead(segments, 4));
    digitalWrite(SEGMENT_F, !bitRead(segments, 5));
    digitalWrite(SEGMENT_G, !bitRead(segments, 6));

//Tests pour plus tard
//    delay(1000);
//    
//    segments = SEGMENTS_STATE[tens];
//
//    digitalWrite(PIN_SEGMENT_A, !bitRead(segments, 0));
//    digitalWrite(PIN_SEGMENT_B, !bitRead(segments, 1));
//    digitalWrite(PIN_SEGMENT_C, !bitRead(segments, 2));
//    digitalWrite(PIN_SEGMENT_D, !bitRead(segments, 3));
//    digitalWrite(PIN_SEGMENT_E, !bitRead(segments, 4));
//    digitalWrite(PIN_SEGMENT_F, !bitRead(segments, 5));
//    digitalWrite(PIN_SEGMENT_G, !bitRead(segments, 6));
//
//    delay(1000);

    
}
