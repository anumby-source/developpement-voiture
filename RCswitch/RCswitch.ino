/*
 * Arnaud https://github.com/arnaudrco/exemples/blob/main/Telecommande-IR-servomoteur/IR-esp-mp3-board-sleep/IR-esp-mp3-board-sleep.ino
 * ATTENTION rentrer les codes pour votre télécommande (emetteur IR)
 
MODELE 433 MHz 


MODELE PROGRAMMABLE
 
 */
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>


#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup_rc() {
//  mpu.begin();
//  mpu.calcGyroOffsets(true);
  mySwitch.enableReceive(D7);  // Receiver on interrupt GPio 13 => that is pin D7 
  Serial.println("ESP8266 RF receiver scanning now...");

}


unsigned long timer = 0;


#define moteur1A D1
#define moteur1B D2

#define moteur2A D3 // rotation
#define moteur2B D4

#define IR D0
#define GND D5

// ------------- télécommande RC
#define RC1 1361
#define RC2 1364
// ------------- IR
#define HAUT 0xA05FE817
#define DROITE 0xA05FB847
#define BAS 0xA05F18E7
#define GAUCHE 0xA05F38C7
#define STOP  0xA05F6897
#define ARRET  0xA05F807F
// ------------- ajout d'un 2° télécommande
#define HAUT1 0xFF18E7
#define DROITE1 0xFF5AA5
#define BAS1 0xFF4AB5
#define GAUCHE1 0xFF10EF
#define STOP1  0xFF38C7
#define ARRET1  0xFFA25D 
// ------------- direction
#define avant 1
#define arriere 2
#define droite 3
#define gauche 4
int lastDirection = 1;


#define TEMPO 500 // temporisation 
// ---------------------------Forth-------------
#define MAX 100
int Index=0;
byte Forth[MAX]; // pile Forth
int vitesse = 192;  // 0 à 255
int programmation=0;

// Le digger a un moteur pour les roues et une commande de direction pour tourner à DROITE ou à GAUCHE

// le programme active le moteur ou les roues suivant une durée countM (moteur) ou count D (direction)


// An IR detector/demodulator is connected to GPIO pin 14(D5 on a NodeMCU
// board).
// Note: GPIO 16 won't work on the ESP8266 as it does not have interrupts.
// Note: GPIO 14 won't work on the ESP32-C3 as it causes the board to reboot.
#ifdef ARDUINO_ESP32C3_DEV
const uint16_t kRecvPin = 10;  // 14 on a ESP32-C3 causes a boot loop.
#else  // ARDUINO_ESP32C3_DEV
const uint16_t kRecvPin = 14;
#endif  // ARDUINO_ESP32C3_DEV


// kRecvPin 

IRrecv irrecv(IR);
decode_results results;
unsigned long mmm, dodo; // arret au bout de 5 minutes
int countM=0 , countD=0 ; // nb d'impulsions IR reçues

void empile( byte x ){
      Serial.print("empile ");
  Serial.println(Forth[Index]);
  if(Index < MAX ) Forth[Index++] = x;

}
void execute() {
  for(int x=0 ; x < Index ; x++){
    moteur(Forth[x],vitesse);
  delay(400); // bip
      stop();    
  delay(200); // bip
  }   
//  Index=0; mémorise le trajet depuis le début
}
void setup() {
  pinMode(GND, OUTPUT);      // board IR
  digitalWrite(GND, LOW);
  Serial.begin(115200);

// test FORTH
empile(avant);
empile(arriere);
//empile(droite);
//empile(gauche);
execute(); 
/*  irrecv.enableIRIn();  // Start the receiver
  while (!Serial)  // Wait for the serial connection to be establised.
    delay(50);
  Serial.println();
  Serial.print("IRrecvDemo is now running and waiting for IR message on Pin ");
  Serial.println(kRecvPin);*/
  mmm = millis();    dodo = millis();
    setup_rc();
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
            Serial.println("Moteur avant");
                      lastDirection = avant;       
                      analogWrite(moteur1A, 0);
                  analogWrite(moteur1B, v);
                 analogWrite(moteur2A, 0);
                  analogWrite(moteur2B, v);
        break;
        case arriere :
            Serial.println("Moteur arriere");
                       lastDirection = arriere;
                        analogWrite(moteur1A, v);
                  analogWrite(moteur1B, 0);
                   //moteur 2
                 analogWrite(moteur2A, v);
                  analogWrite(moteur2B, 0);
                     

        break;

              case droite :
            Serial.println("Moteur droite");
            analogWrite(moteur2A, 0);
            analogWrite(moteur2B, v);
            analogWrite(moteur1A, v);
            analogWrite(moteur1B, 0);
 
      
        break;
        case gauche :
            Serial.println("Moteur gauche");
            analogWrite(moteur2A, v);
            analogWrite(moteur2B, 0);
            analogWrite(moteur1A, 0);
            analogWrite(moteur1B, v);
        break;
  }
}
void loop() {

   
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
  if (mySwitch.available()) {
    if(mySwitch.getReceivedValue()){
            mmm= millis();    dodo= millis();
            
  Serial.print("Received ");
      Serial.print( mySwitch.getReceivedValue() );
      Serial.print(" / ");
      Serial.print( mySwitch.getReceivedBitlength() );
      Serial.print("bit ");
      Serial.print("Protocol: ");
      Serial.println( mySwitch.getReceivedProtocol() );

      switch (mySwitch.getReceivedValue()){
      case RC1 : // 2
        moteur(avant,vitesse);
      break;
      case RC2 : // 
        moteur(arriere,vitesse);
      break;
      
    }
    
  }
      mySwitch.resetAvailable(); 
  }
  if (irrecv.decode(&results)) {


    
      mmm= millis();    dodo= millis();
    // print() & println() can't handle printing long longs. (uint64_t)
    serialPrintUint64(results.value, HEX);
    switch (results.value){
//-----------------------------   moteur 1 ou 2  ---------------------  <<  >>
      case HAUT : // 2
      case HAUT1 : // 2
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
            
//---------------- ARRET---------------
      case   ARRET :
      case   ARRET1 :
            // ESP.deepSleep(0);
               Serial.println(programmation);
            if(programmation) execute(); else  stop(); ;// arret moteur
            programmation = 1 - programmation;
            break;
   }
    Serial.println("");
    irrecv.resume();  // Receive the next value
  }

}
