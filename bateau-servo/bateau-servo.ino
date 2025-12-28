/*
  Example for receiving
  
*/

#include <RCSwitch.h>
#include <Servo.h>

Servo myservo,myservo2; 
RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(9600);
 Serial.println(F("Tft Commande Bateau DC switch"));
  mySwitch.enableReceive(0);  // Receiver on inerrupt 0 => that is pin #2
}

void loop() {
  int value;
  if (mySwitch.available()) {
        myservo.attach(3);
          myservo2.attach(4);
    value=mySwitch.getReceivedValue();
        output(mySwitch.getReceivedValue(), mySwitch.getReceivedBitlength(), mySwitch.getReceivedDelay(), mySwitch.getReceivedRawdata(),mySwitch.getReceivedProtocol());
    Serial.println(value);
    if(value<240) {
         myservo.write(45 + 90 * value / 240);
          Serial.print("barre: ");
          Serial.println(45 + 90 * value / 240);
         }
         else { myservo2.write((value-240)*18 / 24);
          Serial.print("voile : ");
          Serial.println((value-240)*18 / 24);
          }
         delay(1000);
          myservo.detach();
           myservo2.detach();
    mySwitch.resetAvailable();
  }
}
