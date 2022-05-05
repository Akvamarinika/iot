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
  mqtt_cli.subscribe("lab/leds/sensor/sonar_front");
  mqtt_cli.subscribe("lab/leds/sensor/sonar_back");
  //mqtt_cli.publish("lab/leds/strip/set_leds", "led test msg"); //000000FF
  //mqtt_cli.publish("lab/leds/strip/state", "hello emqx");
  //mqtt_cli.subscribe("lab/leds/strip/set_leds");
  //mqtt_cli.subscribe("lab/leds/strip/set_leds_bytes");
  //mqtt_cli.subscribe("lab/leds/strip/set_leds_bytes/111");
  //mqtt_cli.subscribe("lab/leds/strip/rotate_leds");
  
}

void loop(void){
  server.handleClient();                   
  mqtt_cli.loop();
}
