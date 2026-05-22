#include "Ktech_motor.h"


void avancer(float distance_m, int vitesse) {
  unsigned long temps;
  temps = (unsigned long)(fabs(distance_m) * MOVE_TIME_1_METER);
  int signe = (distance_m > 0) ? 1 : -1;

  KTMOTORS[0].sendVelocityCommand(signe * vitesse, CAN);
  KTMOTORS[1].sendVelocityCommand(-signe * vitesse, CAN);

  delay(temps);

  KTMOTORS[0].sendVelocityCommand(0, CAN);
  KTMOTORS[1].sendVelocityCommand(0, CAN);

}


void tourner(float angle_deg, int vitesse) {
  unsigned long temps;
  temps = (unsigned long)((fabs(angle_deg) / 90.0) * TURN_TIME_90_DEG);
  int signe = (angle_deg > 0) ? 1 : -1;

  KTMOTORS[0].sendVelocityCommand(-signe * vitesse, CAN);
  KTMOTORS[1].sendVelocityCommand(-signe * vitesse, CAN);

  delay(temps);

  KTMOTORS[0].sendVelocityCommand(0, CAN);
  KTMOTORS[1].sendVelocityCommand(0, CAN);
}

void deplacer(float distance_m, float angle_deg, int vitesse){
  tourner(angle_deg, vitesse);
  avancer(distance_m, vitesse);
}
