#include "deplacer.h"
#include <stdio.h>
#include <stdbool.h> 

#include <Ultrasonic.h>

#define TRIG_PIN_D 2
#define ECHO_PIN_D 3
#define TRIG_PIN_C 7
#define ECHO_PIN_C 5

Ultrasonic ultra(TRIG_PIN_D,ECHO_PIN_D);
Ultrasonic ultra_2(TRIG_PIN_C,ECHO_PIN_C);



bool objet_devant(float distance_cm){
    if (ultra.read() < distance_cm){
        return true;
    }
    else if(ultra_2.read() < distance_cm){
        return true;
    }
    return false;
}

void calibrage(){
    deplacer(0,-90, 10); // tourner a droite de 90 deg
    while(!objet_devant(30)){ // avancer tant que le mur n'est pas a moins de 30cm
        deplacer(5, 0, 10)
    }
    deplacer(0,90, 10); // tourner a gauche de 90 deg
}