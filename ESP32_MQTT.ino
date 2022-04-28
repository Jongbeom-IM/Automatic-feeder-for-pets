#include <WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "MJU_Wireless";
const char* password = "00000000";
const char* mqtt_server = "broker.mqtt-dashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char distribute_msg[MSG_BUFFER_SIZE];
char amount_left_msg[MSG_BUFFER_SIZE];
int val[10];
int i;

void setup_wifi() {
  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid);
  while (WiFi.status() != WL_CONNECTED) {}
  randomSeed(micros());
}

void callback(char* topic, byte* payload, unsigned int length) {
  char MQTT_msg[MSG_BUFFER_SIZE];
  for (int i = 0; i < length; i++) {
   Serial.write((char)payload[i]);
  }
}

void reconnect() {
  while (!client.connected()) {
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      client.publish("13_output", "Pet_Service ON");
      client.subscribe("13_input");
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  i = 0;
  val[0]=0;
  val[1]=0;
  val[2]=0;
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  if (Serial.available()){
    val[i] = Serial.read();
    i++;
  }
  if(val[2] == 99){
    snprintf (distribute_msg, MSG_BUFFER_SIZE, "distribute : %d g", val[0]);
    client.publish("13_output", distribute_msg);
    snprintf (amount_left_msg, MSG_BUFFER_SIZE, "left : %d g", val[1]);
    client.publish("13_output", amount_left_msg);
    i = 0;
    val[0]=0;
    val[1]=0;
    val[2]=0;
  }
  else if(i == 2 && val[2] != 99){
    i = 0;
    val[0]=0;
    val[1]=0;
    val[2]=0;
  }
}
