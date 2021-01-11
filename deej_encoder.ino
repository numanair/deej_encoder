

/* Encoder Library - TwoKnobs Example
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 *
 * This example code is in the public domain.
 */

#include <Encoder.h>
#include <FastLED.h>
const int NUM_SLIDERS = 5;
// Number of LEDs (generally same as NUM_SLIDERS):
#define NUM_LEDS 5
// LED control pin:
#define DATA_PIN 15
CRGB leds[NUM_LEDS];
int analogSliderValues[NUM_SLIDERS];
// Change these pin numbers to the pins connected to your encoder.
// Best Performance: both pins have interrupt capability
// Good Performance: only the first pin has interrupt capability
// Low Performance:  neither pin has interrupt capability
Encoder knob1(8,9);
Encoder knob2(6,7);
Encoder knob3(10,11);
Encoder knob4(4,5);
Encoder knob5(12,3);
int Master, Discord, Chrome, Gaming, Music;
int Mute1, Mute2, Mute3, Mute4, Mute5;
int masterDebounceTime;
// avoid using pins with LEDs attached
//buttons:
const int button1_pin=A5;
const int button2_pin=A4;
const int button3_pin=A3;
const int button4_pin=A2;
const int button5_pin=A0;
//unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
//unsigned long debounceDelay = 175;    // the debounce time; increase if the output flickers

int knob1State=0;
int knob2State=0;
int knob3State=0;
int knob4State=0;
int knob5State=0;


void setup() {
  
  LEDS.addLeds<WS2812B,DATA_PIN,GRB>(leds,NUM_LEDS);
	LEDS.setBrightness(5);
  
  Serial.begin(9600);
  pinMode(button1_pin, INPUT_PULLUP);
  pinMode(button2_pin, INPUT_PULLUP);
  pinMode(button3_pin, INPUT_PULLUP);
  pinMode(button4_pin, INPUT_PULLUP);
  pinMode(button5_pin, INPUT_PULLUP);

analogSliderValues[0]  = 512;
knob1.write(51);
analogSliderValues[1] = 512;
knob2.write(51);
analogSliderValues[2] = 512;
knob3.write(51);
analogSliderValues[3] = 512;
knob4.write(51);
analogSliderValues[4] = 512;
knob5.write(51);
Mute1=0;
Mute2=0;
Mute3=0;
Mute4=0;
Mute5=0;
}



void loop() {
  
  Master = knob1.read();
  Discord = knob2.read();
  Chrome = knob3.read();
  Gaming = knob4.read();
  Music = knob5.read();



checkButtons();
checkEncoders();
sendSliderValues();
//printSliderValues();//debug
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
    String printedString = String("Slider #") + String(i + 1) + String(": ") + String(analogSliderValues[i]) + String(" mV");
    Serial.write(printedString.c_str());

    if (i < NUM_SLIDERS - 1) {
      Serial.write(" | ");
    } else {
      Serial.write("\n");
    }
  }
}
void checkButtons(){
if (digitalRead(button1_pin)==0 && knob1State ==0){
Mute1 = !Mute1 ;
knob1State =1;
delay(25);
}
 if (digitalRead(button1_pin)==1){
  knob1State=0;
 }
if (digitalRead(button2_pin)==0 && knob2State ==0){
Mute2 = !Mute2 ;
knob2State =1;
delay(25);
}
 if (digitalRead(button2_pin)==1){
  knob2State=0;
 }
 if (digitalRead(button3_pin)==0 && knob3State ==0){
Mute3 = !Mute3 ;
knob3State =1;
delay(25);
}
 if (digitalRead(button3_pin)==1){
  knob3State=0;
 }
 if (digitalRead(button4_pin)==0 && knob4State ==0){
Mute4 = !Mute4 ;
knob4State =1;
delay(25);
}
 if (digitalRead(button4_pin)==1){
  knob4State=0;
 }
 if (digitalRead(button5_pin)==0 && knob5State ==0){
Mute5 = !Mute5 ;
knob5State =1;
delay(25);
}
 if (digitalRead(button5_pin)==1){
  knob5State=0;
 }
}
void checkEncoders(){
    if (Master > 0 && Master < 102 && Mute1==0){
        analogSliderValues[0]=Master*10;
        leds[4] = CRGB::Green;
    }
    else if (Mute1==0 && (Master > 102 || Master==102)){
        analogSliderValues[0]=102*10;
        knob1.write(102);
        leds[4] = CRGB::Green;
    }
    else if (Mute1==1){
      analogSliderValues[0]=0;
        leds[4] = CRGB::Red;
        
    }
    
    else {
        analogSliderValues[0]=0;
        knob1.write(0);
        leds[4] = CRGB::Red;
        
    }
  
    if (Discord > 0 && (Discord < 102 && Mute2==0)){
        analogSliderValues[1]=Discord*10;
        leds[3] = CRGB::Green;
    }
    else if (Mute2==0 && Discord > 102 || Discord==102){
        analogSliderValues[1]=102*10;
        knob2.write(102);
        leds[3] = CRGB::Green;
    }
    else if (Mute2==1){
        analogSliderValues[1]=0;
        leds[3] = CRGB::Red;
    }
    else {
        analogSliderValues[1]=0;
        knob2.write(0);
        leds[3] = CRGB::Red;
    }
    if (Mute3==0 && Chrome > 0 && Chrome < 102){
        analogSliderValues[2]=Chrome*10;
        leds[2] = CRGB::Green;
    }
    else if (Mute3 ==0 && (Chrome > 102 || Chrome==102)){
        analogSliderValues[2]=102*10;
        knob3.write(102);
        leds[2] = CRGB::Green;
    }
    else if (Mute3==1){
        analogSliderValues[2]=0;
        leds[2] = CRGB::Red;
    }
    else {
        analogSliderValues[2]=0;
        knob3.write(0);
        leds[2] = CRGB::Red;
    }
        if (Mute4==0 && Gaming > 0 && Gaming < 102){
        analogSliderValues[3]=Gaming*10;
        leds[1] = CRGB::Green;
        
    }
    else if (Mute4==0 && (Gaming > 102 || Gaming==102)){
        analogSliderValues[3]=102*10;
        knob4.write(102);
        leds[1] = CRGB::Green;
    }
    else if (Mute4==1){
        analogSliderValues[3]=0;
        leds[1] = CRGB::Red;
    }
    else {
        analogSliderValues[3]=0;
        knob4.write(0);
        leds[1] = CRGB::Red;
    }
    if (Mute5==0 && Music > 0 && Music < 102){
      analogSliderValues[4]=Music*10;
        leds[0] = CRGB::Green;
   }
    else if (Mute5 ==0 && (Music > 102 || Music==102)){
        analogSliderValues[4]=102*10;
        knob5.write(102);
          leds[0] = CRGB::Green;
    }
    else if (Mute5==1){
        analogSliderValues[4]=0;
          leds[0] = CRGB::Red;
    }
    else {
        analogSliderValues[4]=0;
        knob5.write(0);
          leds[0] = CRGB::Red;
    }
  // FastLED.show();
}
