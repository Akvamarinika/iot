#include <PubSubClient.h>

PubSubClient mqtt_cli(wifiClient);
int val_sensor_front = 0;
int DIST_val_sensor_back = 0;
int dist = 0;
int prev_led_num = 0;
int number_led;

int get_int(byte* p, int s) {
  int v = (p[s] - '0')*100 + (p[s+1] - '0')*10 + p[s+2];
  return v;
}

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("With length: ");
    Serial.println(length);   

   payload[length] = '\0'; 
   int sensor_number = atoi((char *)payload); 

    Serial.print("sensor_number ");
    Serial.println(sensor_number);

    if(dist < 5){ dist = sensor_number;}
    
    //and val_sensor_back != 0
    if (strcmp(topic, "lab/leds/sensor/sonar_front") == 0 and DIST_val_sensor_back != 0) {
      val_sensor_front = sensor_number;
      int sum_dist_front_back_sensors = val_sensor_front + DIST_val_sensor_back;

        Serial.print("SUM ===== ");
        Serial.println(sum_dist_front_back_sensors);
        Serial.print("DIST ===== ");
        Serial.println(dist);

      if(sum_dist_front_back_sensors <= dist){
        prev_led_num = number_led;
        number_led = map(val_sensor_front, 0, sum_dist_front_back_sensors, 0, NUM_LEDS);

        if(number_led != prev_led_num){
           off_led(prev_led_num);
           set_led(number_led);
           Serial.print("FRONT =====>");
           Serial.println(val_sensor_front);
           Serial.print("DIST =====>");
           Serial.println(sum_dist_front_back_sensors);
           Serial.print("Set color for led number =====>");
           Serial.println(number_led);
          }


      } 

    }

    /******************************************************************************/
    if (strcmp(topic, "lab/leds/sensor/sonar_back") == 0) {
      DIST_val_sensor_back = sensor_number;
    }
    /******************************************************************************/

   // sum_dist_front_back_sensors = X;
   // val_sensor_front = Y;
   // number_led = map(val_sensor_front, 0, sumfrontbacksensors, NUM_LEDS);
    
  
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
