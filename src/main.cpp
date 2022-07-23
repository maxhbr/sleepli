#include "Arduino.h"
#include <Adafruit_DotStar.h>

#define LED_BUILTIN 13

#define NUMPIXELS  1 
#define DATAPIN    7
#define CLOCKPIN   8
Adafruit_DotStar strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

#define RADAR 2

#define RUN_LENGTH 480000
#define INITIAL_PULSE_WIDTH 8000
#define PULSE_INCREMENT 100
#define MAX_BRIGHTNESS 100

int running = 0;
unsigned long startTime;
unsigned long pulseStartTime;
unsigned long pulseDuration = INITIAL_PULSE_WIDTH;

void setup() {
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode (RADAR, INPUT);
    strip.begin(); // Initialize pins for output
    for(int i=0; i<strip.numPixels(); i++) { 
        strip.setPixelColor(i, 0,255,0);
    }
    strip.setBrightness(0);
    strip.show();  // Turn all LEDs off ASAP
    delay(3000);
}

void setBrightness(int b) {
    strip.setBrightness(b);
    strip.show();
}

void startPulse() {
    pulseStartTime = millis();
}

void start() {
    startTime = millis();
    pulseDuration = INITIAL_PULSE_WIDTH;
    startPulse();
}

void loop() {
    if (running) {
        unsigned long curTime = millis();
        unsigned long timePassed = curTime - pulseStartTime;
        if (timePassed > pulseDuration) {
            pulseDuration = pulseDuration + PULSE_INCREMENT;
            if (curTime - startTime > RUN_LENGTH) {
                running = 0;
                return;
            }
            startPulse();
        } else {
            setBrightness((1-cos(2 * 3.15 * timePassed / pulseDuration))*MAX_BRIGHTNESS);
        }
    } else {
        strip.setBrightness(0);
        delay(100);

        digitalWrite(LED_BUILTIN, HIGH);
        delay(10);
        digitalWrite(LED_BUILTIN, LOW);

        running = digitalRead(RADAR);
        if (running) {
            start();
        }
    }
}
