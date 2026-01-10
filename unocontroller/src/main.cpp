/**
 * @file main.cpp
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief
 * @version 0.2
 * @date Created: 2025-11-22
 * @modified Last Modified: 2026-01-09
 *
 * @copyright Copyright (c) 2025
 */

#include <Arduino.h>

#define POT_LEFT A0
#define POT_RIGHT A1
#define BTN_RIGHT A4
#define BTN_LEFT A2
#define BTN_UP A3
#define BTN_DOWN A5

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);

    while (!Serial) {
        ;
    }

    Serial.println("Serial Connection Established");

    // Setup Pins
    pinMode(POT_RIGHT, INPUT);
    pinMode(POT_LEFT, INPUT);
    pinMode(BTN_UP, INPUT);
    pinMode(BTN_DOWN, INPUT);
    pinMode(BTN_LEFT, INPUT);
    pinMode(BTN_RIGHT, INPUT);
}

char buffer[5];
void loop() {
    // snprintf(buffer, 200, "L: %d, R: %d, UB: %d DB: %d LB: %d RB: %d\n",
    //          analogRead(POT_LEFT), analogRead(POT_RIGHT),
    //          digitalRead(BTN_UP), digitalRead(BTN_DOWN),
    //          digitalRead(BTN_LEFT), digitalRead(BTN_RIGHT));
    uint16_t pl = analogRead(POT_LEFT);
    uint16_t pr = analogRead(POT_RIGHT);
    buffer[0] = 0xAA;
    buffer[1] = pl & 0xFF;
    buffer[2] = pr & 0xFF;
    uint8_t btn = 0;
    btn |= !(digitalRead(BTN_UP) & 0x01) << 0;
    btn |= !(digitalRead(BTN_DOWN) & 0x01) << 1;
    btn |= !(digitalRead(BTN_LEFT) & 0x01) << 2;
    btn |= !(digitalRead(BTN_RIGHT) & 0x01) << 3;
    buffer[3] =
        (((pl >> 8) & 0x03) << 6) | (((pr >> 8) & 0x03) << 4) | (btn & 0x0F);
    buffer[4] = buffer[1] ^ buffer[2] ^ buffer[3];
    Serial.write(buffer, 5);
    delay(100);
}
