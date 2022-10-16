#ifndef radio_h
#define radio_h
#include <RFM69.h>

#define NETWORKID       100
#define FREQUENCY       RF69_433MHZ 
#define KEY             "HomeControl01Key" //has to be same 16 characters/bytes on all nodes, not more not less!
#define RFM69_INT_PIN   3
#define ADDRESS         0x34

RFM69 radio(SS, RFM69_INT_PIN, false, 1);

void setupRadio() {
    radio.initialize(FREQUENCY, ADDRESS, NETWORKID);
    radio.encrypt(KEY);
}

typedef union {
    float number;
    uint8_t bytes[4];
} FLOATUNION_t;

byte out[36];
FLOATUNION_t myFloat;

void broadcast(float vcc, float Vsol, float Isol, float Vbat, float Ibat, uint8_t dc, float batPerc, float eff) {
    out[0] = 0x00;    //Function 0x00=Status
    out[1] = ADDRESS; //Address this node
    out[2] = 0x0B;    //Register  address 0x0B

    myFloat.number = vcc;
    out[3] = myFloat.bytes[0];
    out[4] = myFloat.bytes[1];
    out[5] = myFloat.bytes[2];
    out[6] = myFloat.bytes[3];

    myFloat.number = Vsol;
    out[7] = myFloat.bytes[0];
    out[8] = myFloat.bytes[1];
    out[9] = myFloat.bytes[2];
    out[10] = myFloat.bytes[3];

    myFloat.number = Isol;
    out[11] = myFloat.bytes[0];
    out[12] = myFloat.bytes[1];
    out[13] = myFloat.bytes[2];
    out[14] = myFloat.bytes[3];

    myFloat.number = Vsol * Isol;
    out[15] = myFloat.bytes[0];
    out[16] = myFloat.bytes[1];
    out[17] = myFloat.bytes[2];
    out[18] = myFloat.bytes[3];

    out[19] = dc;

    myFloat.number = Vbat;
    out[20] = myFloat.bytes[0];
    out[21] = myFloat.bytes[1];
    out[22] = myFloat.bytes[2];
    out[23] = myFloat.bytes[3];

    myFloat.number = Ibat;
    out[24] = myFloat.bytes[0];
    out[25] = myFloat.bytes[1];
    out[26] = myFloat.bytes[2];
    out[27] = myFloat.bytes[3];

    myFloat.number = batPerc;
    out[28] = myFloat.bytes[0];
    out[29] = myFloat.bytes[1];
    out[30] = myFloat.bytes[2];
    out[31] = myFloat.bytes[3];

    myFloat.number = eff;
    out[32] = myFloat.bytes[0];
    out[33] = myFloat.bytes[1];
    out[34] = myFloat.bytes[2];
    out[35] = myFloat.bytes[3];

    radio.send(RF69_BROADCAST_ADDR, out, sizeof(out));
    radio.sleep();
}

#endif