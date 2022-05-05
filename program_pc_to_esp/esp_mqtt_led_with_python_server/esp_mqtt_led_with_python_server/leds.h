#define FASTLED_INTERNAL
#include <FastLED.h> // https://github.com/FastLED/FastLED

#define DATA_PIN 4 //D2
#define CLOCK_PIN 3

const CRGB BLACK_COLOR = CRGB::Black;
const CRGB BLUE_COLOR = CRGB::Blue;
CRGB leds[NUM_LEDS];

void leds_init()
{
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  Serial.println("Init leds");
  for(int i=0; i < NUM_LEDS;i++) {
    leds[i] = CRGB::Green; 
    delay(10); 
    FastLED.show();
  }
  
  for(int i=0; i < NUM_LEDS;i++) {
    leds[i] = CRGB::Black; 
    delay(10); 
    FastLED.show();
  }
  FastLED.clear();
}

void on_all_leds(){
  for(int i=0; i < NUM_LEDS;i++) {
    if (leds[i] == BLACK_COLOR) { 
      leds[i] = BLUE_COLOR;
      delay(10);    
      FastLED.show();
    }
  }
  //byte brightness = 50;
 // byte hue_led = 0; //оттенок

 // for (int i = 0; i < NUM_LEDS; i++) {
 //     leds[i] = CHSV(hue_led + i * 5, 255, 255);
 // }
  
//    hue_led++;
    //FastLED.setBrightness(brightness);
 //   FastLED.show();
 //   delay(20);
}

void off_all_leds(){
  for(int i=0; i < NUM_LEDS;i++) {
    if (leds[i] == BLUE_COLOR) { 
      leds[i] = BLACK_COLOR;
      delay(10); 
      FastLED.show();
    }

  }
  
  // FastLED.clear();
 } 
