#include <Arduino.h>
#include <Button.h>

const int LED_ORANGE = 9;
const int LED_BLUE = 6;
const int PIN_BUTTON_LEFT = 5;
const int PIN_BUTTON_RIGHT = 4;
const Button* BUTTON_LEFT;
const Button* BUTTON_RIGHT;

void setup() {
    Serial.begin(9600);
    Serial.print("Hello World.");
    pinMode(LED_BLUE, OUTPUT);
    pinMode(LED_ORANGE, OUTPUT);
    BUTTON_LEFT = new Button(PIN_BUTTON_LEFT);
    BUTTON_RIGHT = new Button(PIN_BUTTON_RIGHT);
}

void loop() {
    BUTTON_LEFT->check();
    BUTTON_LEFT->getNumber();

    // BUTTON_RIGHT->check();
    // Serial.print(BUTTON_RIGHT->getNumber());
}