#include <Servo.h>

static const int servoPin1 = D5;
static const int servoPin2 = D8;


Servo servo1,servo2;

void setup() {
    Serial.begin(115200);
    servo1.attach(D5);
        servo2.attach(D8);
        
         servo1.write(0);
              servo2.write(0);
              delay(500);
               servo1.write(180);
              servo2.write(180);
              delay(500);
             
}

void loop() {
    for(int posDegrees = 0; posDegrees <= 180; posDegrees++) {
        servo1.write(posDegrees);
              servo2.write(posDegrees);
        Serial.println(posDegrees);
        delay(5);
    }

    for(int posDegrees = 180; posDegrees >= 0; posDegrees--) {
        servo1.write(posDegrees);
              servo2.write(posDegrees);
        Serial.println(posDegrees);
        delay(2);
    }
}
