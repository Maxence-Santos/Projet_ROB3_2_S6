#include "Ktech_motor.h"


void deplacer(float distance, float angle, float vitesse_roues){
  float R = 0.025 ; // rayon des roues
  float e = 20; // La moitié de la distance entre les roues                       i dont know????????????????????
  float w_gauche = vitesse_roues; // vitesse de rotation du moteur gauche 
  float w_droite = vitesse_roues;  // ~ droit
  float t_debut, t_ecoule, t;
  // tourner
  t = angle / (-(R/e)*w_gauche + (R/e)*w_droite);
  t_debut = micros();
  t_ecoule = 0;
  while (t_ecoule < t){
    KTMOTORS[0].sendVelocityCommand((long int)(-w_gauche), CAN);
    KTMOTORS[1].sendVelocityCommand((long int)(w_droite), CAN);
    t_ecoule = t_debut - micros();
  }
  KTMOTORS[0].sendVelocityCommand((long int)(0), CAN);
  KTMOTORS[1].sendVelocityCommand((long int)(0), CAN);


  // avancer
  t = distance / ((R/2)*w_gauche + (R/2)*w_droite);
  t_debut = micros();
  t_ecoule = 0;
  while (t_ecoule < t){
    KTMOTORS[0].sendVelocityCommand((long int)(w_gauche), CAN);
    KTMOTORS[1].sendVelocityCommand((long int)(w_droite), CAN);
    t_ecoule = t_debut - micros();
  }
  KTMOTORS[0].sendVelocityCommand((long int)(0), CAN);
  KTMOTORS[1].sendVelocityCommand((long int)(0), CAN);
}
