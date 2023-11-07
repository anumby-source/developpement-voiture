/*
 * Arnaud https://github.com/arnaudrco/exemples/blob/main/Telecommande-IR-servomoteur/IR-esp-mp3-board-sleep/IR-esp-mp3-board-sleep.ino
 * ATTENTION rentrer les codes pour votre télécommande (emetteur IR)
 
 ajouter la bibliothèque IRremoteESP8266 dans outils > gérer les bibliothèque

MODELE PROGRAMMABLE
 
**********************************************************
 * IRremoteESP8266: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * This is very simple teaching code to show you how to use the library.
 * If you are trying to decode your Infra-Red remote(s) for later replay,
 * use the IRrecvDumpV2.ino (or later) example code instead of this.
 * An IR detector/demodulator must be connected to the input kRecvPin.
 * Copyright 2009 Ken Shirriff, http://arcfn.com
 * Example circuit diagram:
 *  https://github.com/crankyoldgit/IRremoteESP8266/wiki#ir-receiving
 * Changes:
 *   Version 0.2 June, 2017
 *     Changed GPIO pin to the same as other examples.
 *     Used our own method for printing a uint64_t.
 *     Changed the baud rate to 115200.
 *   Version 0.1 Sept, 2015
 *     Based on Ken Shirriff's IrsendDemo Version 0.1 July, 2009
 *     
 *     
 *    
 *    
 *     
 */
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>


#include <MPU6050_tockn.h>
#include <Wire.h>
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"

MPU6050 mpu(Wire);


unsigned long timer = 0;


#define moteur1A D1
#define moteur1B D2

#define moteur2A D3
#define moteur2B D4

#define IR D7
#define GND D8

#define SDA D6
#define SCL D5

SSD1306Wire display(0x3c, SDA, SCL);
// ------------- télécommande

#define HAUT 0x205D609F
#define DROITE 0x205D58A7
#define BAS 0x205D40BF
#define GAUCHE 0x205D708F
#define STOP  0x205DE01F
#define ARRET  0x205D38C7
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


IRrecv irrecv(IR);
decode_results results;
unsigned long mmm, dodo; // arret au bout de 5 minutes
int countM=0 , countD=0 ; // nb d'impulsions IR reçues
int calib=400 ; // calibration 400 ms
void empile( byte x ){
      Serial.print("empile ");
  Serial.println(Forth[Index]);
  if(Index < MAX ) Forth[Index++] = x;

}
void execute() {
  for(int x=0 ; x < Index ; x++){
    float alpha, delta;
    moteur(Forth[x],vitesse);

  delay(calib); // bip
        stop();   

  delay(200); // bip
  }   
//  Index=0; mémorise le trajet depuis le début
}
void calibration() {
  int alpha, delta=0, i=0;
       mpu.update();
        Serial.println(alpha=mpu.getAngleZ());
        timer=millis();
     moteur(droite,vitesse);
while (delta < 90 ) {
          mpu.update();
      delay(10); // bip
        Serial.println(delta = abs(alpha-mpu.getAngleZ()));
}
      calib= millis()-timer;
      Serial.println(calib);
       delay( calib );
  
      stop();    
  delay(200); // bip
      moteur(gauche,vitesse);
delay( calib );
        stop();    
  delay(200); // bip
}
void setup() {

    display.init();
  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_24);
    display.drawString(0, 26, "Arnaud");
    
  display.display();

  
  pinMode(GND, OUTPUT);      // board IR
  digitalWrite(GND, LOW);
  Serial.begin(115200);
    empile(avant);
 empile(arriere);
 execute();
 Wire.begin(SDA,SCL); // SDA SCL
//         test FORTH

  irrecv.enableIRIn();  // Start the receiver
  while (!Serial)  // Wait for the serial connection to be establised.
    delay(50);
  Serial.println();
  Serial.print("IRrecvDemo is now running and waiting for IR message on Pin ");
//  Serial.println(kRecvPin);
  mmm = millis();    dodo = millis();
  mpu.begin();
  mpu.calcGyroOffsets(true);


// calibration();

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
 //   mpu.begin();

   mpu.update();

     if((millis()-timer)>1000){ // print data every 10ms

  display.clear();
    display.drawString(0, 26, String(mpu.getAngleZ()));
    
  display.display();
      
  Serial.print("X : ");
  Serial.print(mpu.getAngleX());
  Serial.print("\tY : ");
  Serial.print(mpu.getAngleY());
  Serial.print("\tZ : ");
  Serial.println(mpu.getAngleZ());

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
  
  if (irrecv.decode(&results)) {
// -----------------------------MPU-----------------------
  mpu.update();
  Serial.println(mpu.getAngleZ());

    
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
