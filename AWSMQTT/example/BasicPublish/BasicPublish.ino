/*
    Author: Jimmy Tai
    Date: 2017/06/02
*/
#include <ESP8266WiFi.h>
#include <AWSMQTT.h>

/* your MQTT client ID */
const char *CLIENT_ID = "--- your id ---";

/* your MQTT end point, you can find in AWS IoT console */
const char *END_POINT = "--- your end point ---";

/*
    AWSMQTT is a class which helps you to load cert and private key.
    The client variable in AWSMQTT is pubsubclient which is the most popular MQTT library written by Imroy(https://github.com/Imroy/pubsubclient)
*/
AWSMQTT awsMqtt(END_POINT, CLIENT_ID, true, "/your cert name", "/your private key name");

void setup() {
  Serial.begin(115200);
  while (!Serial)
    delay(10);
  Serial.setDebugOutput(true);
  Serial.printf("boot\r\n");
  WiFi.mode(WiFiMode_t::WIFI_STA);
  WiFi.begin("--- your ssid ---", "--- your password ---");
  while (!WiFi.isConnected()) {
    Serial.printf(".");
    delay(500);
  }
  Serial.printf("\r\nWiFi Connected\r\n");
  /* after WiFi connected, you can connect to AWS IoT */
  awsMqtt.connect();
}

void loop() {
  ESP.wdtFeed();
  delay(10);
  if (!WiFi.isConnected()) {
    delay(1 * 1000);
    return;
  }
  /* if it is not connected, try to connect again*/
  if (!awsMqtt.client->connected()) {
    awsMqtt.connect();
    return;
  }
  /* you must run this loop function for regular ping message */
  awsMqtt.client->loop();
  if (Serial.available() > 0) {
    String rec = Serial.readString();
    /*
      Read the Serial command and publish. You can use the client variable inside AWSMQTT to publish.
      if you want to know how to use function of this client, please check the pubsubclient library
    */
    awsMqtt.client->publish(MQTT::Publish("--- your topic ---", "--- your payload ---").set_qos(0));
  }
  delay(1000);
}
