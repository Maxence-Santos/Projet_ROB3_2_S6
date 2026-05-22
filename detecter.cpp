#include <stdio.h>
#include <math.h>

#include "deplacer.h"
#include "detecter.h"

#define TRIG_PIN_D 0 // cable bleu
#define ECHO_PIN_D 0 // cable blanc

#define TRIG_PIN_C 0 // cable jaune
#define ECHO_PIN_C 0 // cable gris

Ultrasonic ultra(TRIG_PIN_D,ECHO_PIN_D); // celui de devant
Ultrasonic ultra2(TRIG_PIN_C,ECHO_PIN_C); // celui de devant

bool objet_devant(float distance_cm){
    if (ultra.read() < distance_cm){
        return true;
    }
    return false;
}

bool objet_droite(float distance_cm){
    if (ultra2.read() < distance_cm){
        return true;
    }
    return false;
}
