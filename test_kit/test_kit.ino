#include <Servo.h>
#include <Ultrasonic.h>
#include "moteur.h"

#define TRIG_PIN_D 2
#define ECHO_PIN_D 3
#define TRIG_PIN_C 7
#define ECHO_PIN_C 5

Servo servo1;
Ultrasonic ultra(TRIG_PIN_D,ECHO_PIN_D);
Ultrasonic ultra_2(TRIG_PIN_C,ECHO_PIN_C);

int pin1 = 9;
int feedback = A15;

void setup() {
  // put your setup code here, to run once:
  servo1.attach(pin1);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Velocityforward(500,1,2);
  Serial.println("""""Test ultrason""""");
  test_ultrason();
  delay(1000);
  Serial.println("""""Test servomoteur""""");
  test_servo();
  delay(1000);
}

void test_servo() {
  for (int i=0;i<=180;i+=20) {
    servo1.write(i);
    Serial.println(analogRead(feedback));
    delay(100);
  }
  for (int i=0;i<=180;i+=10) {
    servo1.write(180-i);
    Serial.println(analogRead(feedback));
    delay(100);
  }
}

void test_ultrason() {
  for (int i=0;i<=10;i++) {
    Serial.print("D : ");
    Serial.print(ultra.read());
    Serial.println("cm");
    delay(1000);
    //d=measureDistanceMm();
    Serial.print("C : ");
    Serial.print(ultra_2.read());
    Serial.println("cm");
    delay(1000);
    i++;
  }
}