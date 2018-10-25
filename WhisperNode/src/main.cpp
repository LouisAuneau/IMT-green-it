#include <Arduino.h>
#include "RH_RF95.h"
#include "T2WhisperNode.h"
#include "hal/T2Flash.h"
#include <LowPower.h>

RH_RF95 radio;
T2Flash flash;

void setup() {
    //Init SP
    Serial.begin(9600);
    
    //Init & disable radio (for now)
    radio.init();
    radio.sleep();

    // Turn off flash (not used)
    flash.init(T2_WPN_FLASH_SPI_CS);
    flash.powerDown();
}

void loop() {
    Serial.println("HIBERNATE");
    delay(1000);
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
}