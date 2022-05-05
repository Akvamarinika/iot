#include "Config.h"
#include "WIFI.h"
#include "Server.h"
#include "MQTT.h"

#define PIN_TRIG 14 //D5
#define PIN_ECHO 12 //D6
long acoustic_signal_delay_time, distance_in_cm;

void setup(void){
  Serial.begin(9600);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  WIFI_init(false);
  server_init();
  MQTT_init();
  //mqtt_cli.publish("lab/leds/sensor/sonar_back", "sonar-back start msg...");
}

void loop(void){
  server.handleClient();                   
  mqtt_cli.loop();
  sendValueFromSensor();
}

void sendValueFromSensor(){
  //генерируем короткий импульс длит. 2-5 микросек:
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(5); 
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  acoustic_signal_delay_time = pulseIn(PIN_ECHO, HIGH);
  distance_in_cm = (acoustic_signal_delay_time / 2) / 29.1;
  Serial.print(distance_in_cm);
  Serial.println(" см");
  delay(250);

  // convert the value to a char array:
  char distCharArray[16];
  itoa(distance_in_cm, distCharArray, 10);
  
  mqtt_cli.publish("lab/leds/sensor/sonar_back", distCharArray);

}
