/*
  Ktech_motor.h - Library for using Ktech motors with the CAN bus communication.
  Created by Guillaume Morel, Dec 8, 2025.
  Released into the public domain.
*/
#ifndef Ktech_motor_h
  #define Ktech_motor_h
  // Libraries for CAN communications
  #include <mcp2515_can.h>
  #define MAX_DATA_SIZE 8

  class Motor
  {
    public:
      Motor(int motorID);
      void readState(mcp2515_can CANCom);
      void ON(mcp2515_can CANCom);
      void OFF(mcp2515_can CANCom);
      void sendVelocityCommand(long int vel, mcp2515_can CANCom);
      float posInDeg;
      float velInDegPerSec;
      float prevPosInDeg;
      int posEncoder;
      int offsetEncoder;
      int revolNumber;
    private:
      int _motorID;
  };

#endif