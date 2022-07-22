#include "Arduino.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

#define PIN PCINT4
#define DELAYVAL 500
#define NUMPIXELS 7
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

unsigned long pulseStartTime;
unsigned long pulseDuration = 4000;

void setup()
{

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
      clock_prescale_set(clock_div_1);
#endif
    pinMode(LED_BUILTIN, OUTPUT);
    pixels.begin();
}

void setBrightness(int b) {
    for(int i=0; i<NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(b, 0, 0));
    }
    pixels.show();
}

void startPulse() {
    pulseStartTime = millis();
}

void loop()
{
    unsigned long curTime = millis();
    unsigned long timePassed = curTime - pulseStartTime;
    if (timePassed > pulseDuration) {
        pulseDuration = pulseDuration + 100;
        startPulse();
    } else {
        setBrightness((1-cos(2 * 3.15 * timePassed / pulseDuration))*30);
    }

    digitalWrite(LED_BUILTIN, HIGH);
    delay(10);
    digitalWrite(LED_BUILTIN, LOW);
    delay(10);
}
