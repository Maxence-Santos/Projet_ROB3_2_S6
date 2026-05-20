

#include <Arduino.h>   // <-- ADD THIS LINE

#include <Ultrasonic.h>
#include "chercher_objet.h"
#include "init_moteurs.h"
#include <SoftwareSerial.h>

#define TRIG_PIN_D 4
#define ECHO_PIN_D 3

Ultrasonic ultra(TRIG_PIN_D,ECHO_PIN_D);
SoftwareSerial soft_serial(7, 8);  // PIN NUMBER FOR RX/TX UART COMMUNICATION WITH YOUR PC
#define PC_SERIAL soft_serial

float revolNumber = 0;
float dist;
float tab_moy{3};
float moy;

void chercher_objet() {
    //PC_SERIAL.begin(115200);
    Serial.begin(115200);
    Motor* KTMOTORS = init_moteurs();

    do {
        moy = 0;
        KTMOTORS[0].sendVelocityCommand(0,CAN);
        KTMOTORS[1].sendVelocityCommand(0,CAN);
        for (int i=0;i<5;i++) {
            dist = ultra.read();
            moy += dist;
        }
        moy /= 5; 
        Serial.println(moy);
    } while (moy < 15);
    do {
        KTMOTORS[0].sendVelocityCommand(100,CAN);
        KTMOTORS[1].sendVelocityCommand(-100,CAN);
        for (int i=0;i<5;i++) {
           dist = ultra.read();
           moy += dist;
        }
        moy /= 5;   
        Serial.println(moy);
    } while(moy >= 15);
    revolNumber = KTMOTORS[0].revolNumber;
}
