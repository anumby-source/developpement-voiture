/*
 * Arnaud https://github.com/arnaudrco/exemples/blob/main/Telecommande-IR-servomoteur/IR-esp-mp3-board-sleep/IR-esp-mp3-board-sleep.ino
 * ATTENTION rentrer les codes pour votre télécommande (emetteur IR)
 
 ajouter la bibliothèque IRremoteESP8266 dans outils > gérer les bibliothèque

MODELE PROGRAMMABLE

pas de 15 cm
correction des erreurs automatique entre les 2 moteurs ( v / TRIM ) 
freinage sur les derniers cm

 */
#include "telecommande.h"

#define moteur1A D1
#define moteur1B D2
#define moteur2A D3
#define moteur2B D4

#define IR D6
#define GND D8
#define ISR D7
#define ISR2 D5

// ------------- télécommande

#define HAUT 0x205D609F
#define DROITE 0x205D58A7
#define BAS 0x205D40BF
#define GAUCHE 0x205D708F
#define STOP  0x205DE01F
#define ARRET  0x205D38C7
// ------------- ajout d'un 2° télécommande
#define HAUT1 0x1FE1CE3
#define DROITE1 0x1FE9C63
#define BAS1 0x1FE02FD
#define GAUCHE1 0x1FEEC13
#define STOP1  0x1FEC837
#define ARRET1  0x1FE817E 
// ------------- direction
#define avant 1
#define arriere 2
#define droite 3
#define gauche 4

#define TEMPO 1000 // temporisation 

int lastDirection = 1;
int lastRotation = 0; // sens de rotation 
int isr_compte; // décompte pour 1 tour 
int isr_compte2; // décompte pour 1 tour 
unsigned long isr_timer; // timer pour 1 tour 
unsigned long isr_timer2; // timer pour 1 tour 
int isr_ms; // duree écoulée entre 2 ticks 
int isr_ms2; // duree écoulée entre 2 ticks
int isr_z; // angle de rotation sur axe z
int erreur=0;
int erreur2=0;

// ---------------------------Forth-------------
#define TRIM 2 // erreur entre moteur 
#define NEUTRE 0 // zone neutre aveugle entre moteurs
#define TOURS 20 // tours pour faire 90 °  
#define PARCOURS 25 // parcours 15 cm
#define FREINAGE 10 // freinage sur les derniers cm
#define MAX 100
int Index=0;
byte Forth[MAX]; // pile Forth
int vitesse = 255;  // 0 à 255
int programmation=0;
int frein=50;
int Sonde[PARCOURS]; // temps de parcours par tick
int Sonde2[PARCOURS];

IRrecv irrecv(IR);
decode_results results;
unsigned long mmm, dodo, timer; // arret au bout de 5 minutes
int countM=0 , countD=0 ; // nb d'impulsions IR reçues


void setup() {
  pinMode(ISR, INPUT_PULLUP);
    pinMode(ISR2, INPUT_PULLUP);
    attachInterrupt(ISR,isr,CHANGE);
        attachInterrupt(ISR2,isr2,CHANGE);
 //   attachInterrupt(ISR,isr,FALLING); 
  pinMode(GND, OUTPUT);      // board IR
  digitalWrite(GND, LOW);
  Serial.begin(115200);
    empile(droite);
 empile(gauche);
 execute();
//         test FORTH

  irrecv.enableIRIn();  // Start the receiver
  while (!Serial)  // Wait for the serial connection to be establised.
    delay(50);
  Serial.println();
  Serial.print("IRrecvDemo is now running and waiting for IR message ");
  mmm = millis();    dodo = millis();

}
ICACHE_RAM_ATTR void isr() {
  isr_compte--;
  isr_ms = millis()- isr_timer; // duree écoulée entre 2 ticks
    if ( isr_compte >= 0 ) Sonde[isr_compte] = isr_ms; // sonde sur les durées
  isr_timer += isr_ms;
}
ICACHE_RAM_ATTR void isr2() {
 //       Serial.println("isr");
  isr_compte2--;
  isr_ms2 = millis()- isr_timer2; // duree écoulée entre 2 ticks
  if ( isr_compte2 >= 0 ) Sonde2[isr_compte2] = isr_ms2;
  isr_timer2 += isr_ms2;
}
void printSonde(){
  for (int i =0 ; i < PARCOURS ; i++)  {
      Serial.print(Sonde[i]); // temps de parcours par tick
      Serial.print(","); 
      Serial.print(Sonde2[i]); // temps de parcours par tick
      Serial.print(" "); 
  }
        Serial.println(); 
}
void empile( byte x ){
      Serial.print("empile ");
  Serial.println(Forth[Index]);
  if(Index < MAX ) Forth[Index++] = x;

}
void execute() {
  for(int x=0 ; x < Index ; x++){
    moteurC(Forth[x]);
  }   
//  Index=0; mémorise le trajet depuis le début
}
void moteurC( int x){
      timer= millis();
    isr_compte = TOURS; isr_compte2 = TOURS;
    while ( ( millis() - timer ) < 1000)
    {
          moteurH(x);
        
    }
        stop();   
}
void moteurH(int x){ 
  int i=max(isr_compte,isr_compte2); // nb de ticks restants 
   if (i<= 0  ) { 
      stop();   
   } else {
      if ( i > 10 ) moteur(x, vitesse); else {
          int t = min( isr_ms, isr_ms2 ) ; // durée entre ticks
          int t_objectif =  frein ; 
          if ( t > t_objectif ) moteur(x, vitesse); else moteur(x, vitesse/2);
      }
   } 
}
void stop() { 
    analogWrite(moteur1A, 0);
  analogWrite(moteur1B, 0);
    analogWrite(moteur2A, 0);
  analogWrite(moteur2B, 0);
}
void moteur( int x, int v){
    switch (x){
 //     if (lastDirection != x) v=255; // accélération de départ
      case avant :
            // Serial.print("Moteur avant");
                      lastDirection = avant;   lastRotation = 0;    
                      analogWrite(moteur1A, 0);
                  analogWrite(moteur1B, v); 
                 analogWrite(moteur2A, 0);
                  analogWrite(moteur2B, v );
                  if(abs(isr_compte - isr_compte2) > NEUTRE ){ // correction de vitesse 
                    if( isr_compte < isr_compte2) analogWrite(moteur1B, v/TRIM); else{
                      analogWrite(moteur2B, v/TRIM);
                    }
                  }                            
        break;
        case arriere :
            // Serial.print("Moteur arriere"); 
            lastDirection = arriere; lastRotation = 0;
                  analogWrite(moteur1A, v);
                  analogWrite(moteur1B, 0);
                   //moteur 2
                 analogWrite(moteur2A, v );
                  analogWrite(moteur2B, 0);
                  if(abs(isr_compte - isr_compte2) > NEUTRE ){ // correction de vitesse 
                    if( isr_compte < isr_compte2) analogWrite(moteur1A, v/TRIM); else{
                      analogWrite(moteur2A, v/TRIM);
                    }
                  } 

        break;

              case droite :
              // Serial.println("Moteur droite"); 
             lastRotation = 1; lastDirection = droite;
            analogWrite(moteur1A, 0);
            analogWrite(moteur1B, v ); 
            analogWrite(moteur2A, v); 
                  if(abs(isr_compte - isr_compte2) > NEUTRE ){ // correction de vitesse 
                    if( isr_compte < isr_compte2) analogWrite(moteur1B, v/TRIM); else{
                      analogWrite(moteur2A, v/TRIM);
                    } 
                  }
 
      
        break;
        case gauche :
            // Serial.println("Moteur gauche"); 
            lastRotation = -1; lastDirection = gauche;
            analogWrite(moteur1A,  v ); 
            analogWrite(moteur1B, 0);
            analogWrite(moteur2A, 0);
            analogWrite(moteur2B, v);
                  if(abs(isr_compte - isr_compte2) > NEUTRE ){ // correction de vitesse 
                    if( isr_compte < isr_compte2) analogWrite(moteur1A, v/TRIM); else{
                      analogWrite(moteur2B, v/TRIM);
                    } 
                  }
        break;
  }
}
void loop() {
     if((millis()-timer)>100){ // print data every 10ms
        if ( max(isr_compte,isr_compte2) > 0 ) {
          Serial.print("ISR ");Serial.print(isr_compte);Serial.print(":");Serial.print(isr_compte2);Serial.print( "K ");Serial.println(frein);
          printSonde();
        }
            timer = millis();  
  }  
    if ((millis()- dodo) > 300000){ 
            ESP.deepSleep(0);
    }
  if ((millis()- mmm) > TEMPO){ // 
    mmm= millis();
    if(countM > 0 ){
      countM --; 
    } else {
       stop();
    }
  }
  moteurH(lastDirection); // impulsion 
  if (irrecv.decode(&results)) {
      mmm= millis();    dodo= millis();
    // print() & println() can't handle printing long longs. (uint64_t)
    serialPrintUint64(results.value, HEX);
    switch (results.value){
//------------------ vitesse --------------------
      case PLUS : 
      case PLUS1 : 
        // vitesse += 10;
        frein -= 5;
        if(frein < 0) frein=10;
             break;
      case MOINS :
      case MOINS1 :
        frein += 5;
        if(frein > 100 ) frein=100;
             break;
//-----------------------------   moteur 1 ou 2  ---------------------  <<  >>
      case UP : 
      case UP1 : 
           if(programmation)  {
            empile( avant); 
            stop();
           } else {
            isr_compte = PARCOURS ; isr_compte2 = PARCOURS ; // initialise avec erreur précédente
            moteur(avant,vitesse);countM++;
           }
             break;
      case DOWN :
      case DOWN1 :
            if(programmation){
              empile( arriere); stop();
            }
            else {
               isr_compte = PARCOURS ; isr_compte2 = PARCOURS ; // initialise avec erreur précédente
               moteur(arriere,vitesse);countM++;
            }
             break;
//-----------------------------   virage  ---------------------  +  -
       case RIGHT : 
       case RIGHT1 :
             isr_compte = TOURS ; isr_compte2 = TOURS ; // initialise avec erreur précédente
             moteur(droite,vitesse);countM++;
             if(programmation) empile( droite);        
             break;
       case   LEFT :
       case   LEFT1 :
            isr_compte = TOURS ; isr_compte2 = TOURS ; // initialise avec erreur précédente
            moteur(gauche,vitesse);countM++;       
            if(programmation) empile( gauche);
            break;
//---------------- STOP---------------
      case   ENTER :
      case   ENTER1 :
            stop();
            Index=0;erreur=0;erreur2=0;  isr_compte = 0 ;isr_compte2 = 0 ;lastDirection = avant;
            break; 
//---------------- ON/OFF touche rouge ---------------
      case   ONOFF :
      case   ONOFF1 :
            stop(); // arret moteur
            programmation = 1 ;
            Index=0;erreur=0;erreur2=0;  isr_compte = 0 ;isr_compte2 = 0 ;lastDirection = avant;
            break;
   //---------------- retour  ---------------
      case   BACK :
      case   BACK1 :
            stop();
           execute();
            programmation = 0 ;
            break;
   }
    Serial.println("");
    irrecv.resume();  // Receive the next value
  
  }

}
