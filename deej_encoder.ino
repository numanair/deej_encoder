#include <ESP32Encoder.h>
//#include <Encoder.h>
#include <FastLED.h>
const int NUM_SLIDERS = 5;
// Number of LEDs (generally same as NUM_SLIDERS):
#define NUM_LEDS 5
// LED control pin:
#define DATA_PIN 2
CRGB leds[NUM_LEDS];
int analogSliderValues[NUM_SLIDERS];
// Change these pin numbers to the pins connected to your encoder.
// Best Performance: both pins have interrupt capability
// Good Performance: only the first pin has interrupt capability
// Low Performance:  neither pin has interrupt capability
//Encoder knob1(8,9);
//Encoder knob2(6,7);
//Encoder knob3(10,11);
//Encoder knob4(4,5);
//Encoder knob5(12,3);

ESP32Encoder encoder1;

int Master, Discord, Chrome, Gaming, Music;
int Mute1, Mute2, Mute3, Mute4, Mute5;
// avoid using pins with LEDs attached
//buttons:
const int button1_pin=27;
const int button2_pin=2;
const int button3_pin=3;
const int button4_pin=4;
const int button5_pin=5;
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
  
  ESP32Encoder::useInternalWeakPullResistors=UP;
  encoder1.attachHalfQuad(15, 16);
  encoder1.clearCount();
  pinMode(button1_pin, INPUT_PULLUP);

  analogSliderValues[0]  = 512;
  encoder1.setCount(51);

  Mute1=0;
  Mute2=0;
  Mute3=0;
  Mute4=0;
  Mute5=0;
}

void loop() {
  
  Master = encoder1.getCount();

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
        encoder1.setCount(102);
        leds[4] = CRGB::Green;
    }
    else if (Mute1==1){
      analogSliderValues[0]=0;
        leds[4] = CRGB::Red;
        
    }
    
    else {
        analogSliderValues[0]=0;
        encoder1.setCount(0);
        leds[4] = CRGB::Red;
        
    }
}
