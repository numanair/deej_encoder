#include <ESP32Encoder.h>
#include <FastLED.h>

// LED setup:
// Number of LEDs (generally same as NUM_SLIDERS):
#define NUM_LEDS 5
// LED control pin:
#define DATA_PIN 2
CRGB leds[NUM_LEDS];

const int NUM_SLIDERS = 5;
int analogSliderValues[NUM_SLIDERS];

ESP32Encoder encoder1;
ESP32Encoder encoder2;
ESP32Encoder encoder3;
ESP32Encoder encoder4;
ESP32Encoder encoder5;

int Master, Discord, Chrome, Gaming, Music;
int Mute1, Mute2, Mute3, Mute4, Mute5;

// avoid using pins with LEDs attached
// buttons:
const int button1_pin = 27;
const int button2_pin = 33;
const int button3_pin = 35;
const int button4_pin = 32;
const int button5_pin = 34;

int knob1State = 0;
int knob2State = 0;
int knob3State = 0;
int knob4State = 0;
int knob5State = 0;

void setup() {
    LEDS.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
    LEDS.setBrightness(5);

    Serial.begin(9600);

    ESP32Encoder::useInternalWeakPullResistors = UP;
    encoder1.attachHalfQuad(15, 16);
    encoder2.attachHalfQuad(17, 18);
    encoder3.attachHalfQuad(25, 26);
    encoder4.attachHalfQuad(19, 21);
    encoder5.attachHalfQuad(22, 23);
    encoder1.clearCount();
    encoder2.clearCount();
    encoder3.clearCount();
    encoder4.clearCount();
    encoder5.clearCount();
    
    pinMode(button1_pin, INPUT_PULLUP);
    pinMode(button2_pin, INPUT_PULLUP);
    pinMode(button3_pin, INPUT_PULLUP);
    pinMode(button4_pin, INPUT_PULLUP);
    pinMode(button5_pin, INPUT_PULLUP);

    for (int i = 0; i < NUM_SLIDERS; i++) {
        analogSliderValues[i] = 512;
    }

    encoder1.setCount(51);
    encoder2.setCount(51);
    encoder3.setCount(51);
    encoder4.setCount(51);
    encoder5.setCount(51);

    Mute1 = 0;
    Mute2 = 0;
    Mute3 = 0;
    Mute4 = 0;
    Mute5 = 0;
}

void loop() {
    Master = encoder1.getCount();
    Discord = encoder2.getCount();
    Chrome = encoder3.getCount();
    Gaming = encoder4.getCount();
    Music = encoder5.getCount();

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
    if (digitalRead(button1_pin) == 0 && knob1State == 0) {
        Mute1 = !Mute1;
        knob1State = 1;
        delay(25);
    }
    if (digitalRead(button1_pin) == 1) {
        knob1State = 0;
    }
    if (digitalRead(button2_pin) == 0 && knob2State == 0) {
        Mute2 = !Mute2;
        knob2State = 1;
        delay(25);
    }
    if (digitalRead(button2_pin) == 1) {
        knob2State = 0;
    }
    if (digitalRead(button3_pin) == 0 && knob3State == 0) {
        Mute3 = !Mute3;
        knob3State = 1;
        delay(25);
    }
    if (digitalRead(button3_pin) == 1) {
        knob3State = 0;
    }
    if (digitalRead(button4_pin) == 0 && knob4State == 0) {
        Mute4 = !Mute4;
        knob4State = 1;
        delay(25);
    }
    if (digitalRead(button4_pin) == 1) {
        knob4State = 0;
    }
    if (digitalRead(button5_pin) == 0 && knob5State == 0) {
        Mute5 = !Mute5;
        knob5State = 1;
        delay(25);
    }
    if (digitalRead(button5_pin) == 1) {
        knob5State = 0;
    }
}
void checkEncoders() {
    // Master (encoder1)
    if (Master > 0 && Master < 102 && Mute1 == 0) {
        analogSliderValues[0] = Master * 10;
        leds[4] = CRGB::Green;
    } else if (Mute1 == 0 && (Master > 102 || Master == 102)) {
        analogSliderValues[0] = 102 * 10;
        encoder1.setCount(102);
        leds[4] = CRGB::Green;
    } else if (Mute1 == 1) {
        analogSliderValues[0] = 0;
        leds[4] = CRGB::Red;
    } else {
        analogSliderValues[0] = 0;
        encoder1.setCount(0);
        leds[4] = CRGB::Red;
    }
    // End of Master (encoder1)

    // Discord (encoder2)
    if (Discord > 0 && Discord < 102 && Mute2 == 0) {
        analogSliderValues[1] = Discord * 10;
        leds[3] = CRGB::Green;
    } else if (Mute2 == 0 && (Discord > 102 || Discord == 102)) {
        analogSliderValues[1] = 102 * 10;
        encoder1.setCount(102);
        leds[3] = CRGB::Green;
    } else if (Mute2 == 1) {
        analogSliderValues[1] = 0;
        leds[3] = CRGB::Red;
    } else {
        analogSliderValues[1] = 0;
        encoder1.setCount(0);
        leds[3] = CRGB::Red;
    }
    // End of Discord (encoder2)

    // Chrome (encoder3)
    if (Chrome > 0 && Chrome < 102 && Mute3 == 0) {
        analogSliderValues[2] = Chrome * 10;
        leds[2] = CRGB::Green;
    } else if (Mute3 == 0 && (Chrome > 102 || Chrome == 102)) {
        analogSliderValues[2] = 102 * 10;
        encoder1.setCount(102);
        leds[2] = CRGB::Green;
    } else if (Mute3 == 1) {
        analogSliderValues[2] = 0;
        leds[2] = CRGB::Red;
    } else {
        analogSliderValues[2] = 0;
        encoder1.setCount(0);
        leds[2] = CRGB::Red;
    }
    // End of Chrome (encoder3)

    // Gaming (encoder4)
    if (Gaming > 0 && Gaming < 102 && Mute4 == 0) {
        analogSliderValues[3] = Gaming * 10;
        leds[1] = CRGB::Green;
    } else if (Mute4 == 0 && (Gaming > 102 || Gaming == 102)) {
        analogSliderValues[3] = 102 * 10;
        encoder1.setCount(102);
        leds[1] = CRGB::Green;
    } else if (Mute4 == 1) {
        analogSliderValues[3] = 0;
        leds[1] = CRGB::Red;
    } else {
        analogSliderValues[3] = 0;
        encoder1.setCount(0);
        leds[1] = CRGB::Red;
    }
    // End of Gaming (encoder4)

    // Music (encoder5)
    if (Music > 0 && Music < 102 && Mute5 == 0) {
        analogSliderValues[4] = Music * 10;
        leds[0] = CRGB::Green;
    } else if (Mute5 == 0 && (Music > 102 || Music == 102)) {
        analogSliderValues[4] = 102 * 10;
        encoder1.setCount(102);
        leds[0] = CRGB::Green;
    } else if (Mute5 == 1) {
        analogSliderValues[4] = 0;
        leds[0] = CRGB::Red;
    } else {
        analogSliderValues[4] = 0;
        encoder1.setCount(0);
        leds[0] = CRGB::Red;
    }
    // End of Music (encoder5)
}
