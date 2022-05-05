#include "Config.h"
#include "WIFI.h"
#include "Server.h"
#include "leds.h"
#include "MQTT.h"


void setup(void){
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  
  for(int i=0; i< 3; i++) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    delay(500);
  }
  
  leds_init();
  WIFI_init(false);
  server_init();
  MQTT_init();
  mqtt_cli.subscribe("lab/leds/strip/set_leds_bytes/1"); // для каждой ленты свой номер топика == номеру ленты (8шт. esp) "lab/leds/strip/set_leds_bytes/#"
  //mqtt_cli.subscribe("lab/leds/strip/set_leds_bytes/2");
  //mqtt_cli.subscribe("lab/leds/strip/set_leds_bytes/8");
}

void loop(void){
  server.handleClient();                   
  mqtt_cli.loop();
}
