#include <time.h>
#include <Arduino.h>
#include "button.h"

Button::Button(int pin) {
    this->m_pin = pin;
    this->m_counter = 0;
    this->m_flag = false;
    this->m_timer = 0l;
    pinMode(this->m_pin, INPUT);
}

void Button::check() {
    if (digitalRead(this->m_pin) == HIGH && !this->m_flag) {
        this->m_flag = true;
    }

    if (digitalRead(this->m_pin) == LOW && this->m_flag) {
        this->m_flag = false;
        this->m_counter += 1;
        this->m_timer = millis();
    }
}

int Button::getNumber() {
    while((millis() - this->m_timer) < 500) {
        this->check();
    }
    Serial.println(this->m_counter);
    int ret = this->m_counter;
    this->m_counter = 0;
    return ret;
}