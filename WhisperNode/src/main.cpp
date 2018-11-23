#include <Arduino.h>
#include "RH_RF95.h"
#include "T2WhisperNode.h"
#include "hal/T2Flash.h"
#include <LowPower.h>
#include "button.h"
#include <LoRa.h>
#include <protocol/T2Message.h>
#include <string.h>

// Prototypes
void sendLORA(int idx, int src, int dst, int sdx, int cmd, const char* data, int dataLen);
void readLora();
void extractID(String msgReceived);

// Global state
RH_RF95 radio;
T2Flash flash;
const int PIN_BUTTON_RIGHT = 4; 
Button BUTTON_RIGHT(PIN_BUTTON_RIGHT);
T2Message myMsg;
uint8_t radioBuf[(T2_MESSAGE_HEADERS_LEN + T2_MESSAGE_MAX_DATA_LEN)];
int myID = 0;
int myNetID = 0;
int mySerialNumber = 3;


// Setup and Loop
void setup() {
    //Init SP
    Serial.begin(9600);

    //Init & disable radio (for now)
    //radio.init();
    //radio.sleep();

    // Turn off flash (not used)
    flash.init(T2_WPN_FLASH_SPI_CS);
    flash.powerDown();

    // Initialisation
    LoRa.setPins(10, 7, 2);
    if (!LoRa.begin(868E6)) {
        Serial.println("Starting LoRa failed");
        while(1);
    }

    // Emission du packet pour avoir un ID
    char buf[10];
    int len = 2;
    itoa(mySerialNumber, buf, 10);
    buf[1] = ';';
    sendLORA(0,myID,1,1,0, buf, len);
}

void loop() {

    //BUTTON_RIGHT.check();
    if (myID == 0) {
        readLora();
    }

    // HIBERNATION CODE
    //Serial.println("HIBERNATE");
    //LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
}


// --------------------------------
// Functions

/**
 * Send a message in LoRa protocol.
 */
void sendLORA(int idx, int src, int dst, int sdx, int cmd, const char* data, int dataLen) {
    uint8_t radioBufLen = 0;

    myMsg.idx = idx;
    myMsg.src = src;
    myMsg.dst = dst;
    myMsg.sdx = sdx;
    myMsg.cmd = cmd;
    memcpy(myMsg.data, data, dataLen);
    myMsg.len = dataLen;

    myMsg.getSerializedMessage(radioBuf, &radioBufLen);
    // myMsg.printMessage();
    LoRa.beginPacket();
    LoRa.write(radioBuf, radioBufLen);
    LoRa.endPacket();
    delay(10);
}

/**
 * Read a message from LoRa protocol.
 */
void readLora() {
    int i=0;
    int packetSize = LoRa.parsePacket();
    int radioBufLen = 0;
    if (packetSize) {
        while (LoRa.available()) {
            radioBuf[i++] = (char) LoRa.read();
        }
        radioBufLen = i;
        myMsg.setSerializedMessage(radioBuf, radioBufLen);
        String msgReceived = String((char *)myMsg.data);

        // When seeking the ID on init.
        if (myID == 0) {
            extractID(msgReceived);
        }
      
    }
}


/**
 * Extract our ID, network ID from a LoRa initialization message. 
 */
void extractID(String msgReceived) {
    Serial.println(msgReceived);
    char* token = strtok(msgReceived.c_str(), ";");
    int i = 0;
    int id = NULL;
    int netID = NULL;
    int serialNumber = NULL;
    while (token != NULL) {
        switch (i) {
            case 0:
                id = atoi(token);
                break;
            case 1:
                netID = atoi(token);
                break;
            case 2:
                serialNumber = atoi(token);
                break;
        }
        token = strtok(NULL, ";");
        i++;
    }

    if(serialNumber == mySerialNumber) {
        myID = id;
        myNetID = netID;
    }
}