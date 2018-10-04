#include <Arduino.h>

const int LED_ORANGE = 9;
const int LED_BLUE = 6;
const int BUTTON_LEFT = 5;
const int BUTTON_RIGHT = 4;

void setup() {
    Serial.begin(9600);
    Serial.print("Hello World.");

    pinMode(LED_BLUE, OUTPUT);
    pinMode(LED_ORANGE, OUTPUT);
    pinMode(BUTTON_LEFT, INPUT);
    pinMode(BUTTON_RIGHT, INPUT);
}

void loop() {
    if (digitalRead(BUTTON_RIGHT) == HIGH) {
        digitalWrite(LED_ORANGE, HIGH);
    } else {
        digitalWrite(LED_ORANGE, LOW);
    }

    if (digitalRead(BUTTON_LEFT) == HIGH) {
        digitalWrite(LED_BLUE, HIGH);
    } else {
        digitalWrite(LED_BLUE, LOW);
    }
}