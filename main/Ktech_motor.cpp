/*
  Ktech_motor.cpp - Interfacing a KTECH motor
  Created by Guillaume Morel, Dec 8, 2025.
  Released into the public domain.
*/
#include <Arduino.h>
#include <stdlib.h>
#include <stdio.h>
#include "Ktech_motor.h"

Motor::Motor(int motorID)
{
  _motorID = motorID;
}

#define MAX_COUNTER_FOR_CAN_CHECK 20

void Motor::readState(mcp2515_can CANCom) {
  uint32_t CANId;
  uint8_t len;
  byte cdata[MAX_DATA_SIZE] = { 0 };

  int data2_3, data4_5, data6_7;

  int currentMotorVelRaw;
  int counterForCANCheck = 0;
  // wait for data
  while ( (CAN_MSGAVAIL != CANCom.checkReceive()) && counterForCANCheck < MAX_COUNTER_FOR_CAN_CHECK){
    counterForCANCheck ++;
    delayMicroseconds(10);
  }
  if (counterForCANCheck == MAX_COUNTER_FOR_CAN_CHECK) {
    Serial.println("I DON'T RECEIVE ANYTHING FROM THE CAN BUS");
  }
  // read data, len gets the data length, cdata -- the data buffer
  CANCom.readMsgBuf(&len, cdata);
  CANId = CANCom.getCanId();

  // Check if the received ID matches the motor ID
  if ((CANId - 0x140) == _motorID) {
    data4_5 = cdata[4] + pow(2, 8) * cdata[5];
    velInDegPerSec = (float)(data4_5);
    data6_7 = cdata[6] + pow(2, 8) * cdata[7];
    posEncoder = (int)data6_7;
  }

  // Conversion of the position (with motor revolution counting) and writing in the Motor class public variabels
  posEncoder -= offsetEncoder; // Adds the offset
  posInDeg = ((float)(revolNumber)*360.0) + (((float)posEncoder) * 180.0 / 32768.0);  // converts encoder to degree assuming no encoder jump.

  if ((posInDeg - prevPosInDeg) < -20.0) { // add to the number of revolution counter if required (negative encoder jump)
    revolNumber++;
    posInDeg = ((float)(revolNumber)) * 360.0 + ((float)posEncoder) * 180.0 / 32768.0;
  }
  if ((posInDeg - prevPosInDeg) > 20.0) { // remove to the number of revolution counter if required (positive encoder jump)
    revolNumber--;
    posInDeg = ((float)(revolNumber)) * 360.0 + ((float)posEncoder) * 180.0 / 32768.0;
  }

  prevPosInDeg = posInDeg;  // writing prev pos to be ready for next call
}


void Motor::ON(mcp2515_can CANCom)
{
  unsigned char msg[MAX_DATA_SIZE] = { 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  CANCom.sendMsgBuf(0x140 + _motorID, 0, 8, msg);
  readState(CANCom); // MUST READ THE STATE AFTER ANY MESSAGE SENT
}

void Motor::OFF(mcp2515_can CANCom) {
  unsigned char msg[MAX_DATA_SIZE] = { 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  CANCom.sendMsgBuf(0x140 + _motorID, 0, 8, msg);
  readState(CANCom); // MUST READ THE STATE AFTER ANY MESSAGE SENT
}

void Motor::sendVelocityCommand(long int vel, mcp2515_can CANCom) {
  long int local_velocity;
  local_velocity = 100*vel; // Convert into hundreth of degrees per second
  unsigned char *adresse_low = (unsigned char *)(&local_velocity);
  unsigned char msg[MAX_DATA_SIZE] = { 0xA2, 0x00, 0x00, 0x00,
    *(adresse_low), *(adresse_low + 1), *(adresse_low + 2), *(adresse_low + 3) };
  CANCom.sendMsgBuf(0x140 + _motorID, 0, 8, msg);
  readState(CANCom); // MUST READ THE STATE AFTER ANY MESSAGE SENT
}
