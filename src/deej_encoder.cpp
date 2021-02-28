#include <Arduino.h>
#include <ESP32Encoder.h>
#include <FastLED.h>

// LED setup:
// Number of LEDs (generally same as NUM_SLIDERS):
#define NUM_LEDS 5
// LED control pin:
#define DATA_PIN 2
#define ledBrightness 50
#define minHue 14
#define maxHue 102
#define mutecolor Red
#define colorCorrectionMode Typical8mmPixel
CRGB leds[NUM_LEDS];

const int NUM_SLIDERS = 5;
int analogSliderValues[NUM_SLIDERS];

// avoid using pins with LEDs attached
// buttons pins:
const int button_pin[NUM_SLIDERS] = {27, 33, 35, 32, 34};

ESP32Encoder encoder[NUM_SLIDERS];

int knobState[NUM_SLIDERS];
int Mute[NUM_SLIDERS];
// int Master, Discord, Chrome, Gaming, Music;
int knobVal[NUM_SLIDERS];

void sendSliderValues();
void printSliderValues();
void checkButtons();
void checkEncoders();

void setup() {
    LEDS.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS)
        .setCorrection(colorCorrectionMode);
    LEDS.setBrightness(ledBrightness);

    Serial.begin(9600);

    ESP32Encoder::useInternalWeakPullResistors = UP;
    // set encoder pins A & B (CLK & DT) here:
    encoder[0].attachHalfQuad(15, 16);
    encoder[1].attachHalfQuad(17, 18);
    encoder[2].attachHalfQuad(25, 26);
    encoder[3].attachHalfQuad(19, 21);
    encoder[4].attachHalfQuad(22, 23);

    for (int i = 0; i < NUM_SLIDERS; i++) {
        analogSliderValues[i] = 512;
        pinMode(button_pin[i], INPUT_PULLUP);
        knobState[i] = 0;
        Mute[i] = 0;
        encoder[i].setCount(51);
    }
}

void loop() {
    for (int i = 0; i < NUM_SLIDERS; i++) {
        knobVal[i] = encoder[i].getCount();
    }
    // Master = encoder1.getCount(); //replaced with knobVal
    // Discord = encoder2.getCount();
    // Chrome = encoder3.getCount();
    // Gaming = encoder4.getCount();
    // Music = encoder5.getCount();

    checkButtons();
    checkEncoders();
    sendSliderValues();
    // printSliderValues();//debug
    FastLED.show();
}

void sendSliderValues() {
    String builtString = String("");

    for (int i = 0; i < NUM_SLIDERS; i++) {
        builtString += String((int)analogSliderValues[i]);

        if (i < NUM_SLIDERS - 1) {
            builtString += String("|");
        }
    }

    Serial.println(builtString);
}

void printSliderValues() {
    for (int i = 0; i < NUM_SLIDERS; i++) {
        String printedString = String("Slider #") + String(i + 1) +
                               String(": ") + String(analogSliderValues[i]) +
                               String(" mV");
        Serial.write(printedString.c_str());

        if (i < NUM_SLIDERS - 1) {
            Serial.write(" | ");
        } else {
            Serial.write("\n");
        }
    }
}

void checkButtons() {
    for (int i = 0; i < NUM_SLIDERS; i++) {
        if (digitalRead(button_pin[i]) == 0 && knobState[i] == 0) {
            Mute[i] = !Mute[i];
            knobState[i] = {1};
            delay(25);
        }
        if (digitalRead(button_pin[i]) == 1) {
            knobState[i] = {0};
        }
    }
}

void checkEncoders() {
    for (int i = 0; i < NUM_SLIDERS; i++) {
        if (knobVal[i] > 0 && knobVal[i] < 102 && Mute[0] == 0) {
            // Normal volume levels
            analogSliderValues[i] = knobVal[i] * 10;
            int encodertoLedHue[i] = {map(knobVal[i], 0, 102, minHue, maxHue)};
            leds[i].setHue(encodertoLedHue[i]);
        } else if (Mute[i] == 0 && (knobVal[i] > 102 || knobVal[i] == 102)) {
            // constrain to max volume
            analogSliderValues[i] = 102 * 10;
            encoder[i].setCount(102);
            leds[i].setHue(maxHue);
        } else if (Mute[i] == 1) {
            // is muted, output volume of 0
            analogSliderValues[i] = 0;
            leds[i] = CRGB::mutecolor;
        } else {
            // not muted, but volume = 0
            analogSliderValues[i] = 0;
            encoder[i].setCount(0);
            leds[i].setHue(minHue);
            // minHue is probably better since
            // we aren't actually muted
        }
    }

    // // Discord (encoder2)
    // if (Discord > 0 && Discord < 102 && Mute[1] == 0) {
    //     analogSliderValues[1] = Discord * 10;
    //     int encoder2toLedHue = map(Discord, 0, 102, minHue, maxHue);
    //     leds[1].setHue(encoder2toLedHue);
    // } else if (Mute[1] == 0 && (Discord > 102 || Discord == 102)) {
    //     analogSliderValues[1] = 102 * 10;
    //     encoder2.setCount(102);
    //     leds[1].setHue(maxHue);
    // } else if (Mute[1] == 1) {
    //     analogSliderValues[1] = 0;
    //     leds[1] = CRGB::mutecolor;
    // } else {
    //     analogSliderValues[1] = 0;
    //     encoder2.setCount(0);
    //     leds[1] = CRGB::mutecolor;
    // }
    // // End of Discord (encoder2)

    // // Chrome (encoder3)
    // if (Chrome > 0 && Chrome < 102 && Mute[2] == 0) {
    //     analogSliderValues[2] = Chrome * 10;
    //     int encoder3toLedHue = map(Chrome, 0, 102, minHue, maxHue);
    //     leds[2].setHue(encoder3toLedHue);
    // } else if (Mute[2] == 0 && (Chrome > 102 || Chrome == 102)) {
    //     analogSliderValues[2] = 102 * 10;
    //     encoder3.setCount(102);
    //     leds[2].setHue(maxHue);
    // } else if (Mute[2] == 1) {
    //     analogSliderValues[2] = 0;
    //     leds[2] = CRGB::mutecolor;
    // } else {
    //     analogSliderValues[2] = 0;
    //     encoder3.setCount(0);
    //     leds[2] = CRGB::mutecolor;
    // }
    // // End of Chrome (encoder3)

    // // Gaming (encoder4)
    // if (Gaming > 0 && Gaming < 102 && Mute[3] == 0) {
    //     analogSliderValues[3] = Gaming * 10;
    //     int encoder4toLedHue = map(Gaming, 0, 102, minHue, maxHue);
    //     leds[3].setHue(encoder4toLedHue);
    // } else if (Mute[3] == 0 && (Gaming > 102 || Gaming == 102)) {
    //     analogSliderValues[3] = 102 * 10;
    //     encoder4.setCount(102);
    //     leds[3].setHue(maxHue);
    // } else if (Mute[3] == 1) {
    //     analogSliderValues[3] = 0;
    //     leds[3] = CRGB::mutecolor;
    // } else {
    //     analogSliderValues[3] = 0;
    //     encoder4.setCount(0);
    //     leds[3] = CRGB::mutecolor;
    // }
    // // End of Gaming (encoder4)

    // // Music (encoder5)
    // if (Music > 0 && Music < 102 && Mute[4] == 0) {
    //     analogSliderValues[4] = Music * 10;
    //     int encoder5toLedHue = map(Music, 0, 102, minHue, maxHue);
    //     leds[4].setHue(encoder5toLedHue);
    // } else if (Mute[4] == 0 && (Music > 102 || Music == 102)) {
    //     analogSliderValues[4] = 102 * 10;
    //     encoder5.setCount(102);
    //     leds[4].setHue(maxHue);
    // } else if (Mute[4] == 1) {
    //     analogSliderValues[4] = 0;
    //     leds[4] = CRGB::mutecolor;
    // } else {
    //     analogSliderValues[4] = 0;
    //     encoder5.setCount(0);
    //     leds[4] = CRGB::mutecolor;
    // }
    // // End of Music (encoder5)
}
