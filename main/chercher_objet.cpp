#include "chercher_objet.h"
#include "Ultrasonic.h"
#include "init_moteurs.h"

#define TRIG_PIN_D 2
#define ECHO_PIN_D 3

Ultrasonic ultra(TRIG_PIN_D,ECHO_PIN_D);

float revolNumber = 0;

void chercher_objet() {
    Motor* KTMOTORS = init_moteurs();
    float dist = ultra.read();

    while (dist > 10) {
        KTMOTORS[0].sendVelocityCommand(100,CAN);
        KTMOTORS[1].sendVelocityCommand(100,CAN);
        dist = ultra.read();
    }
    KTMOTORS[0].sendVelocityCommand(0,CAN);
    KTMOTORS[1].sendVelocityCommand(0,CAN); 
    revolNumber = KTMOTORS[0].revolNumber;
}