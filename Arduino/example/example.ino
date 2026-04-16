/*******************************************************************************
* Uses code lines copied from example "velocity_mode" of the library Dynamixel2Arduino
* Copyright 2016 ROBOTIS CO., LTD.
* Licensed under the Apache License, Version 2.0 (the "License");
* Adapted in Feb 2026 By G. Morel for the POLYTECH ROB3 PROJECT
* In this example, you have code examples to control a KTECH motor, a DYNAMIXEL motor
* (model compatible with protocol 1.0 such as MX-28), and a servo motot controlling a gripper
*******************************************************************************/
#include <math.h>

// Libraries for KT motors 
// Arduino library for communacting with SPI devices
#include <SPI.h>
// CAN communications
#include <mcp2515_can.h>
const int SPI_CS_PIN = 9; 
mcp2515_can CAN(SPI_CS_PIN);  // Creates the mcp2515_can class object for CAN communication, while setting CS pin
// Homemade KTmotor interface
#include "Ktech_motor.h"
// Declaration of the two motors with their ID
#define NUMBER_OF_MOTORS 2
Motor KTMOTORS[NUMBER_OF_MOTORS] = {Motor(1),Motor(2)};  // 1 and 2 are the motor IDs for CAN protocol

// Libraries for DXL motor
#include <Dynamixel2Arduino.h>
#include <SoftwareSerial.h>
SoftwareSerial soft_serial(7, 8);  // PIN NUMBER FOR RX/TX UART COMMUNICATION WITH YOUR PC
#define PC_SERIAL soft_serial
#define DXL_SERIAL Serial
const int DXL_DIR_PIN = 2;  // DYNAMIXEL Shield DIR PIN
const uint8_t DXL_ID = 1;
const float DXL_PROTOCOL_VERSION = 1.0;
// creates dxl as the dynamixel object
Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);
//This namespace is required to use Control table item names
using namespace ControlTableItem;  

// Gripper PIN
#define GRIPPER_PIN 9

// Constant definitions
#if !defined(FALSE)
#define FALSE 0
#define TRUE 1
#endif
#define MY_PI 3.14159265359

#define SEND_A_POSITION_TO_DXL TRUE
#define SEND_A_VELOCITY_TO_KTM TRUE
#define MEASURE_THE_US_SENSORS TRUE
#define MOVE_THE_GRIPPER TRUE
#define PRINTING_ON TRUE

// Global variables
// Variables for US sensors
#define NUMBER_OF_US_SENSORS 2 
const int trigPinUS[NUMBER_OF_US_SENSORS] = {2, 7};
const int echoPinUS[NUMBER_OF_US_SENSORS] = {3, 5};
float durationUS[NUMBER_OF_US_SENSORS] = {0.0, 0.0};
float distanceUS[NUMBER_OF_US_SENSORS] = {0.0, 0.0};


// Clock
unsigned long currentTimeInMicros = 0;
float currentTimeInS = 0.0;

void setup() {
  int i;
  
  // Setup digital I/O for US sensor
  for (i = 0; i < NUMBER_OF_US_SENSORS; i++) {
    pinMode(trigPinUS[i], OUTPUT);
    pinMode(echoPinUS[i], INPUT);
  }

  // Use UART port of DYNAMIXEL Shield to debug.
  PC_SERIAL.begin(115200);
  Serial.begin(115200);
  Serial.println("test");

  // Initialization of the CAN communication. THis will wait until the motor is powered on
  while (CAN_OK != CAN.begin(CAN_500KBPS)) {
    PC_SERIAL.println("CAN init fail, retry ...");
    delay(500);
  }

  // Set Port baudrate to 57600bps. This has to match with DYNAMIXEL baudrate.
  dxl.begin(1000000);
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  /*/****dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);**** A RAJOUTER POUR DYNAMIXEL !!
  // Get DYNAMIXEL information
  dxl.ping(DXL_ID);
  // Turn off torque when configuring items in EEPROM area
  dxl.torqueOff(DXL_ID);
  dxl.setOperatingMode(DXL_ID, OP_POSITION);
  dxl.torqueOn(DXL_ID);
  // Limit the maximum velocity in Position Control Mode. Use 0 for Max speed
  dxl.writeControlTableItem(PROFILE_VELOCITY, DXL_ID, 5);*/

  // Initialization of the motor command and the position measurement variables
  KTMOTORS[0].prevPosInDeg = 0.0;
  KTMOTORS[0].revolNumber = 0;
  KTMOTORS[0].offsetEncoder = 0;
  // Send motor off then motor on command to reset
 // KTMOTORS[0].OFF(CAN);

 // KTMOTORS[0].ON(CAN);
 // KTMOTORS[0].sendVelocityCommand((long int)(0), CAN);
  KTMOTORS[0].revolNumber = 0;
  KTMOTORS[0].prevPosInDeg = 0.0;

  // Initialization of the motor command and the position measurement variables
  KTMOTORS[1].prevPosInDeg = 0.0;
  KTMOTORS[1].revolNumber = 0;
  KTMOTORS[1].offsetEncoder = 0;
  // Send motor off then motor on command to reset
//  KTMOTORS[1].OFF(CAN);

//  KTMOTORS[1].ON(CAN);
//  KTMOTORS[1].sendVelocityCommand((long int)(0), CAN);
  KTMOTORS[1].revolNumber = 0;
  KTMOTORS[1].prevPosInDeg = 0.0;

  // Initialize the gripper to output
  pinMode(GRIPPER_PIN, OUTPUT);


  PC_SERIAL.println("");
  PC_SERIAL.println("");
  PC_SERIAL.println("************************************************************");
  PC_SERIAL.println(" LE SYSTEME EST PRET A FONCTIONNER. SIGNAL DE COMMANDE = 0 *");
  PC_SERIAL.println("************************************************************");
  PC_SERIAL.println("");

}

#define SWITCH_PERIOD 4
int firstTimeInHalfPeriod1 = TRUE;
int firstTimeInHalfPeriod2 = TRUE;

void loop() {
  int i;

  float dxlPositionInDegrees;
  float motorVelocityCommandInDegPerSec;

  currentTimeInMicros = micros();
  currentTimeInS = 0.000001*((float)currentTimeInMicros);

  // US 
  if (MEASURE_THE_US_SENSORS) {
    for (i = 0; i < NUMBER_OF_US_SENSORS; i++) {
      digitalWrite(trigPinUS[i], LOW);
      delayMicroseconds(2);
      digitalWrite(trigPinUS[i], HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPinUS[i], LOW);
      durationUS[i] = pulseIn(echoPinUS[i], HIGH);
      distanceUS[i] = (durationUS[i]*.0343)/2;
      Serial.println(distanceUS[i]);
    }
  }
  else {
    for (i = 0; i < NUMBER_OF_US_SENSORS; i++) {
      distanceUS[i] = 10000.0; // arbitrarily high
    }
  }

  if ( ( ((int)(currentTimeInS)) % SWITCH_PERIOD) < SWITCH_PERIOD/2){
    if (firstTimeInHalfPeriod1) {
      if (MOVE_THE_GRIPPER) {
        if(distanceUS[1] > 8) {
        analogWrite(GRIPPER_PIN, 50); // FULLY OPEN DON'T GO LOWER
        delay(100); // It is advised to add this delay as when sending other commands the gripper servo bugs
        }
      }
      if(PRINTING_ON) {
        PC_SERIAL.println("Switching Commands 1");
        if (MEASURE_THE_US_SENSORS) {
          PC_SERIAL.print("US measure 1 : ");
          PC_SERIAL.print(distanceUS[0]);
          Serial.println(distanceUS[0]);
          PC_SERIAL.print(", 2 : ");
          PC_SERIAL.println(distanceUS[1]);
          Serial.println(distanceUS[1]);
        }
        /*if (SEND_A_POSITION_TO_DXL) {
          PC_SERIAL.print("DXL Measured Position (deg) : ");
          PC_SERIAL.println(dxl.getPresentPosition(DXL_ID, UNIT_DEGREE));
        } A RAJOUTER POUR DYNAMIXEL */
        if (SEND_A_VELOCITY_TO_KTM) {
          PC_SERIAL.print("KTM Measured Velocity first motor (deg/s) : ");
          PC_SERIAL.println(KTMOTORS[0].velInDegPerSec);
          PC_SERIAL.print("KTM Measured Velocity second motor (deg/s) : ");
          PC_SERIAL.println(KTMOTORS[1].velInDegPerSec);
        }
      }
      /*if (SEND_A_POSITION_TO_DXL) {
        // Please refer to e-Manual(http://emanual.robotis.com) for available range of value.
        // Set Goal Position in DEGREES
        dxlPositionInDegrees = 100.0;
        if(distanceUS[1] > 8) {
          dxl.setGoalPosition(DXL_ID, dxlPositionInDegrees, UNIT_DEGREE);
        }
      } A RAJOUTER POUR DYNAMIXEL*/ 
      if (SEND_A_VELOCITY_TO_KTM) {
        motorVelocityCommandInDegPerSec = 50.0;
        if (distanceUS[0] > 8) {
          KTMOTORS[0].sendVelocityCommand((long int)(motorVelocityCommandInDegPerSec), CAN); // Sent motorVelocityCommandInDegPerSec to the motor
          KTMOTORS[1].sendVelocityCommand((long int)(motorVelocityCommandInDegPerSec), CAN); // Sent motorVelocityCommandInDegPerSec to the motor
        }
        else{
          KTMOTORS[0].sendVelocityCommand((long int)(0), CAN); // Sent 0 to the motor
          KTMOTORS[1].sendVelocityCommand((long int)(0), CAN); // Sent 0 to the motor
        }
      }
      firstTimeInHalfPeriod1 = FALSE;
      firstTimeInHalfPeriod2 = TRUE;
    }
  }
  else {
    if(firstTimeInHalfPeriod2) {
      if (MOVE_THE_GRIPPER) {
        if(distanceUS[1] > 8) {
          analogWrite(GRIPPER_PIN, 245); // Fully closed don't go higher
          delay(100); // It is advised to add this delay as when sending other commands the gripper servo bugs
        }
      }
      if( PRINTING_ON ) {
        PC_SERIAL.println("Switching Commands 2");
        if (MEASURE_THE_US_SENSORS) {
            PC_SERIAL.print("US measure 1 : ");
            PC_SERIAL.print(distanceUS[0]);
            Serial.println(distanceUS[0]);
            PC_SERIAL.print(", 2 : ");
            PC_SERIAL.println(distanceUS[1]);
            Serial.println(distanceUS[1]);
        }
        /*if (SEND_A_POSITION_TO_DXL) {
          PC_SERIAL.print("DXL Measured Position (deg) : ");
          PC_SERIAL.println(dxl.getPresentPosition(DXL_ID, UNIT_DEGREE));
        } A RAJOUTER POUR DYNAMIXEL*/
        if (SEND_A_VELOCITY_TO_KTM) {
          PC_SERIAL.print("KTM Measured Velocity first motor (deg/s) : ");
          PC_SERIAL.println(KTMOTORS[0].velInDegPerSec);
          PC_SERIAL.print("KTM Measured Velocity second motor (deg/s) : ");
          PC_SERIAL.println(KTMOTORS[1].velInDegPerSec);
        }
      }
      /*if (SEND_A_POSITION_TO_DXL) {
        // Please refer to e-Manual(http://emanual.robotis.com) for available range of value.
        // Set Goal Position in DEGREES
        dxlPositionInDegrees = 10.0;
        if(distanceUS[1] > 8) {
          dxl.setGoalPosition(DXL_ID, dxlPositionInDegrees, UNIT_DEGREE);
        }
      } A RAJOUTER POUR DYNAMIXEL */
      if (SEND_A_VELOCITY_TO_KTM) {
        motorVelocityCommandInDegPerSec = -50.0;
        if (distanceUS[0] > 8) {
          KTMOTORS[0].sendVelocityCommand((long int)(motorVelocityCommandInDegPerSec), CAN); // Sent motorVelocityCommandInDegPerSec to the motor
          KTMOTORS[1].sendVelocityCommand((long int)(motorVelocityCommandInDegPerSec), CAN); // Sent motorVelocityCommandInDegPerSec to the motor
        }
        else{
          KTMOTORS[0].sendVelocityCommand((long int)(0), CAN); // Sent 0 to the motor
          KTMOTORS[1].sendVelocityCommand((long int)(0), CAN); // Sent 0 to the motor
        }
      }
      firstTimeInHalfPeriod1 = TRUE;
      firstTimeInHalfPeriod2 = FALSE;
    }

  }
}
