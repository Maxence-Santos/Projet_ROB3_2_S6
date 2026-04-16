#include "Ktech_motor.h"
#include "mcp2515_can.h"
#include "init_moteurs.h"

const int SPI_CS_PIN = 9;
mcp2515_can CAN(SPI_CS_PIN);
static Motor KTMOTORS[2] = {Motor(1), Motor(2)};

Motor* init_moteurs() {
    CAN.begin(CAN_500KBPS);
    KTMOTORS[0].prevPosInDeg = 0.0;
    KTMOTORS[0].revolNumber = 0;
    KTMOTORS[0].offsetEncoder = 0;
    KTMOTORS[1].prevPosInDeg = 0.0;
    KTMOTORS[1].revolNumber = 0;
    KTMOTORS[1].offsetEncoder = 0;

    return KTMOTORS;
}
