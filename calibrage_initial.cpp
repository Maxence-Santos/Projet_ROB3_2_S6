#include <stdio.h>
#include <math.h>

#include "deplacer.h"
#include "detecter.h"


void calibrage(){
    deplacer(0,-90, 100); // tourner a droite de 90 deg
    while(!objet_devant(30)){ // avancer tant que le mur n'est pas a moins de 30cm
        deplacer(0.03, 0, 100);
    }
    deplacer(0, 90, 100); // tourner a gauche de 90 deg
}