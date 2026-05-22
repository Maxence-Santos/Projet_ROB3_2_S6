#include <stdio.h>
#include <math.h>

#include "deplacer.h"

void corriger_droite(float distance_cm, float angle, int vitesse){ // rester à une certaine distance du mur à droite, tourner d'un angle positif à une vitesse
    if (ultra2.read() < distance_cm*0.90){
    tourner(angle, vitesse); // tourner à gauche
    }
    else if (ultra2.read() > distance_cm*1.10){
    tourner(-angle, vitesse); // tourner à droite
    }
}