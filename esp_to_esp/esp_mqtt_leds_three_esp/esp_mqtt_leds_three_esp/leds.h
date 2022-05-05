#define FASTLED_INTERNAL
#include <FastLED.h> // https://github.com/FastLED/FastLED

#define DATA_PIN 4 //D2
#define CLOCK_PIN 3
const CRGB BLACK_COLOR = CRGB::Black;
const CRGB BLUE_COLOR = CRGB::Blue;

CRGB leds[NUM_LEDS];

void off_leds() {
//  for(int i=0; i < NUM_LEDS; i++) {
 //   leds[i] = CRGB::Black; 
 //   delay(10); 
//    FastLED.show();
//  }
//  FastLED.clear();

   FastLED.clear();  // clear all pixel data
  FastLED.show();
}

void off_led(int n_led) {
  if (leds[n_led] == BLUE_COLOR) { 
    leds[n_led] = CRGB::Black;
    delay(10);    
    FastLED.show();
  }
  //leds[n_led] = CRGB::Black; 
  //FastLED.show();
}

void leds_init(){
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  Serial.println("Init leds");
  for(int i=0; i < NUM_LEDS;i++) {
    leds[i] = CRGB::Green; 
    //delay(10); 
    FastLED.show();
  }
  
  off_leds();
}

void set_led(int num_led) {
   if (leds[num_led] == BLACK_COLOR) { 
     // delay(10);
    leds[num_led] = CRGB::Blue;    
    FastLED.show();
  }
}



int get_channel(byte* p, int s) {
  int v = (p[s] - '0')*100 + (p[s+1] - '0')*10 + (p[s+2] - '0');
  return v;
}



int set_leds_bytes(byte *payload, unsigned int length) {
  int nleds = (length/3);
  nleds = nleds > NUM_LEDS ? NUM_LEDS : nleds;
  for (int i = 0; i < nleds ; i++) {
      int s = i * 3;
      int r = payload[s]; 
      int g = payload[s+1]; 
      int b = payload[s+2]; 
      //Serial.println(r);
      //Serial.println(g);
      //Serial.println(b);
      leds[i] = CRGB(r, g, b);
  }  
  FastLED.show();
  return nleds;
}

void rotate_leds() {
  CRGB first_led = leds[0];
  for (int i = 0; i < NUM_LEDS-1; i++) {
    leds[i] = leds[i+1];
  }
  leds[NUM_LEDS-1] = first_led;  
  FastLED.show();
}
