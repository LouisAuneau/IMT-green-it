#include <Arduino.h>
#include "RH_RF95.h"
#include "T2WhisperNode.h"
#include "hal/T2Flash.h"
#include <LowPower.h>
#include "button.h"
#include <LoRa.h>
#include <protocol/T2Message.h>

RH_RF95 radio;
T2Flash flash;

const int PIN_BUTTON_RIGHT = 4; 
Button BUTTON_RIGHT(PIN_BUTTON_RIGHT);
T2Message myMsg;
uint8_t radioBuf[(T2_MESSAGE_HEADERS_LEN + T2_MESSAGE_MAX_DATA_LEN)];
int myID = 0;

void setup() {
    //Init SP
    Serial.begin(9600);
    Serial.println("EXCELLENT");

    //Init & disable radio (for now)
    radio.init();
    radio.sleep();

    // Turn off flash (not used)
    flash.init(T2_WPN_FLASH_SPI_CS);
    flash.powerDown();
}

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
    myMsg.printMessage();
    LoRa.beginPacket();
    LoRa.write(radioBuf, radioBufLen);
    LoRa.endPacket();
    delay(10);
}

void loop() {

    //BUTTON_RIGHT.check();
    //Serial.print(BUTTON_RIGHT.getNumber());

    // Initialisation
    LoRa.setPins(10, 7, 2);
    if (!LoRa.begin(868E6)) {
        Serial.println("Starting LoRa failed");
        while(1);
    }

    // Emission de données
    char buf[10];
    int len = 2;
    itoa(6, buf, 10);
    buf[1] = ';';
    sendLORA(0,myID,1,1,0, buf, len);

    // Reception des données
     // try to parse packet
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        // received a packet
        Serial.print("Received packet '");

        // read packet
        while (LoRa.available()) {
        Serial.print((char)LoRa.read());
        }

        // print RSSI of packet
        Serial.print("' with RSSI ");
        Serial.println(LoRa.packetRssi());
    }

    //char buf[10];
    //itoa(serialNb, buf, 10);
    //len = strlen(bur);

    // HIBERNATION CODE
    //Serial.println("HIBERNATE");
    delay(60000);
    Serial.println("UP");
    //LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
}