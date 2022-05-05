#include <PubSubClient.h>

PubSubClient mqtt_cli(wifiClient);

int get_int(byte* p, int s) {
  int v = (p[s] - '0')*100 + (p[s+1] - '0')*10 + p[s+2];
  return v;
}

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("With length: ");
    Serial.println(length);    
    
    if (strcmp(topic, "lab/leds/strip/set_leds_bytes/1") == 0) {  // для каждой ленты свой номер топика == номеру ленты (8шт. esp) "lab/leds/strip/set_leds_bytes/#"
      char message[length + 1];
      
      for(int i=0; i < length; i++) {
        message[i] = (char)payload[i];
      }
      
      //on_all_leds();
      Serial.print("payload: ");
      Serial.println(message);
      
      if (strcmp(message, "on") == 0){
          on_all_leds();
        }
      else if (strcmp(message, "off") == 0){
         off_all_leds();
        }  
      
    }
    

    Serial.println("-----------------------");
}

void MQTT_init(){
  mqtt_cli.setServer(mqtt_broker, mqtt_port);
  mqtt_cli.setBufferSize(2048);
  mqtt_cli.setCallback(callback);
  while (!mqtt_cli.connected()) {
      String client_id = "esp8266-" + String(WiFi.macAddress());
      Serial.print("The client " + client_id);
      Serial.println(" connects to the public mqtt broker\n");
      if (mqtt_cli.connect(client_id.c_str())){
          Serial.println("MQTT Connected");
      } else {
          Serial.print("failed with state ");
          Serial.println(mqtt_cli.state());
          delay(2000);
      }
  }  
}
