/*
 * Arnaud https://github.com/arnaudrco/exemples/blob/main/Telecommande-IR-servomoteur/IR-esp-mp3-board-sleep/IR-esp-mp3-board-sleep.ino
 * ATTENTION rentrer les codes pour votre télécommande (emetteur IR)
 
 ajouter la bibliothèque IRremoteESP8266 dans outils > gérer les bibliothèque

MODELE PROGRAMMABLE
 
 */
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

#include <TM1637Display.h>

#define moteur1A D1
#define moteur1B D2

#define moteur2A D3
#define moteur2B D4

#define IR D6
#define GND D7
#define ISR D7

#define CLK  D5 //Set the CLK pin connection to the display
#define DIO  D0 //Set the DIO pin connection to the display

TM1637Display display(CLK, DIO); //set up the 4-Digit Display.

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

#define TEMPO 500 // temporisation 

int lastDirection = 1;
int lastRotation = 0; // sens de rotation 
int isr_compte; // décompte pour 1 tour 
int isr_z; // angle de rotation sur axe z
// ---------------------------Forth-------------
#define ERREUR 30 // erreur entre moteur
#define MAX 100
int Index=0;
byte Forth[MAX]; // pile Forth
int vitesse = 192;  // 0 à 255
int programmation=0;

IRrecv irrecv(IR);
decode_results results;
unsigned long mmm, dodo, timer; // arret au bout de 5 minutes
int countM=0 , countD=0 ; // nb d'impulsions IR reçues
int calib=400 ; // calibration 400 ms

void setup() {
 display.setBrightness(0x0a); //set the diplay TM167 to maximum brightness
 
  pinMode(ISR, INPUT_PULLUP);
    attachInterrupt(ISR,isr,CHANGE);
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
void delayG(int d) { // delay avec lise à jour de mpu
    unsigned long ttt =millis();
   delay(d);
    // while((millis()-ttt)< d )  mpu.update();
}
ICACHE_RAM_ATTR void isr() {
 //       Serial.println("isr");
  isr_compte++;
  isr_z += lastRotation; // angle de rotation
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
    isr_compte = 0;
    while ( ( millis() - timer ) < 1000)
    {
          moteurH(x);
        
    }

        stop();  
  
}
void moteurH(int x){ // impulsion de 10 ms puis arret 
  if (isr_compte > 29 ) {
     stop();  
  }
  else {
          moteur(x,vitesse); 
          delay(10);
           stop();    
           delay(10 +  isr_compte);

  }
 
  
}


void stop() { // avant arriere
    analogWrite(moteur1A, 0);
  analogWrite(moteur1B, 0);
    analogWrite(moteur2A, 0);
  analogWrite(moteur2B, 0);

}

void moteur( int x, int v){
    switch (x){
 //     if (lastDirection != x) v=255; // accélération de départ
      case avant :
            Serial.print("Moteur avant");
                      lastDirection = avant;   lastRotation = 0;    
                      analogWrite(moteur1A, 0);
                  analogWrite(moteur1B, v);
                 analogWrite(moteur2A, 0);
                  analogWrite(moteur2B, v + ERREUR);
                          Serial.print(isr_compte);Serial.print(":");
                          
        break;
        case arriere :
            Serial.print("Moteur arriere"); lastDirection = arriere; lastRotation = 0;
                        analogWrite(moteur1A, v);
                  analogWrite(moteur1B, 0);
                   //moteur 2
                 analogWrite(moteur2A, v + ERREUR);
                  analogWrite(moteur2B, 0);
                          Serial.print(isr_compte);Serial.print(":");

        break;

              case droite :
            Serial.println("Moteur droite"); lastRotation = 1; lastDirection = droite;
            analogWrite(moteur2A, 0);
            analogWrite(moteur2B, v + ERREUR);
            analogWrite(moteur1A, v);
            analogWrite(moteur1B, 0);
            Serial.print(isr_compte);Serial.print(" z:");Serial.println(isr_z);
 
      
        break;
        case gauche :
            Serial.println("Moteur gauche"); lastRotation = -1; lastDirection = gauche;
            analogWrite(moteur2A, v + ERREUR);
            analogWrite(moteur2B, 0);
            analogWrite(moteur1A, 0);
            analogWrite(moteur1B, v);
            Serial.print(isr_compte);Serial.print(" z:");Serial.println(isr_z);
        break;
  }
}
void loop() {
     if((millis()-timer)>100){ // print data every 10ms
  //      Serial.println(isr_compte);
           display.showNumberDec(isr_compte); //Display the numCounter value;
            timer = millis();  
  }
   
    if ((millis()- dodo) > 300000){ 
            ESP.deepSleep(0);
    }
  if ((millis()- mmm) > calib){ // 
    mmm= millis();
    if(countM > 0 ){
      countM --; 
    } else {
       stop();
    }
  }
 
  moteurH(lastDirection); // impulsion 
  
  if (irrecv.decode(&results)) {
  isr_compte = 0;
      mmm= millis();    dodo= millis();
    // print() & println() can't handle printing long longs. (uint64_t)
    serialPrintUint64(results.value, HEX);
    switch (results.value){
//-----------------------------   moteur 1 ou 2  ---------------------  <<  >>
      case HAUT : 
      case HAUT1 : 
           countM++; if(programmation)  empile( avant); else moteur(avant,vitesse);
             break;
      case BAS :
      case BAS1 :
           countM++; if(programmation) empile( arriere);else moteur(arriere,vitesse);
             break;
//-----------------------------   virage  ---------------------  +  -
       case DROITE : 
       case DROITE1 : 
            moteur(droite,vitesse);countM++;if(programmation) empile( droite);
            break;
       case   GAUCHE :
       case   GAUCHE1 :
            moteur(gauche,vitesse);countM++;if(programmation) empile( gauche);
            break;
//---------------- STOP---------------
      case   STOP :
      case   STOP1 :
            stop();
            Index=0;
            break;
            
//---------------- ARRET touche rouge ---------------
      case   ARRET :
      case   ARRET1 :
               Serial.println(programmation);
            if(programmation) execute(); else  stop(); ;// arret moteur
            programmation = 1 - programmation;
            break;
   }
    Serial.println("");
    irrecv.resume();  // Receive the next value
  
  }

}
