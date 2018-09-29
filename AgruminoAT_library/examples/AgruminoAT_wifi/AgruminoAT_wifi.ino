/*
  AgruminoAT_wifi.ino - AT commands interpreter project for Agrumino board using the Agrumino library.
  Created by Antonello Meloni & Hicham Lafhouli.
  Mosquitto Wifi example
*/

#include <Agrumino.h>
#include "AgruminoAt.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define LENGHT 20

char mqtt_server [LENGHT] = {"192.168.0.106"};
char ssid[LENGHT] = {"ESIOT"};
char password[LENGHT] = {"Esiot.2018"};
char channel_pub[LENGHT] = {"AgruminoAt"};
char channel_sub[LENGHT] = {"AgruminoAt/cmd"};
char sleep_mode = 'd';
uint8_t ok_start = 0;

WiFiClient espClient;

PubSubClient client(espClient);

Agrumino agrumino;
AgruminoAt interpreter;
config* st ;

void setup() {
  Serial.begin(115200);
  agrumino.setup();
  agrumino.turnBoardOn();
  interpreter.setup_interpreter(agrumino);
  st = interpreter.get_config();

  ok_start = st->ok_start;

  if (strlen(st->mqtt_server) > 0) {
    strcpy (mqtt_server, st->mqtt_server);
  }
  if (strlen(st->ssid) > 0) {
    strcpy (ssid, st->ssid);
  }
  if (strlen(st->password) > 0) {
    strcpy (password, st->password);
  }
  if (strlen(st->channel_pub) > 0) {
    strcpy (channel_pub, st->channel_pub);
  }
  if (strlen(st->channel_sub) > 0) {
    strcpy (channel_sub, st->channel_sub);
  }

  if (st->sleep_mode == 's') {
    sleep_mode = 's';
  } else {
    sleep_mode = 'd';
  }

  if (ok_start == 1) {
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    reconnect();
  }
}

void setup_wifi() {
  int times=9;
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED && times>0) {
    delay(500);
    times--;
  }
}


void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  if ((char)payload[0] == 's' && (char)payload[1] == 'o') {
    pub_soil();

  }
  Serial.println();
}


void reconnect() {

  Serial.print("Attempting MQTT connection...");
  // Attempt to connect
  if (client.connect("ESP8266Client")) {
    Serial.println("connected");
    // Once connected, publish an announcement...
    client.publish(channel_pub, "Connected!");

    // ... and resubscribe
    Serial.println(channel_sub);
    client.subscribe(channel_sub);
  } else {
    Serial.print("failed, rc=");
    Serial.println(client.state());
  }
}

void loop() {
  if (!agrumino.isBoardOn()) {
    agrumino.turnBoardOn();
  }

  boolean isButtonPressed =   agrumino.isButtonPressed();

  if (agrumino.isAttachedToUSB()) {

    Serial.println("Press a key within 2 sec. to go to AT interpreter");
    delay(2000);
    if (Serial.available()) {
      interpreter.atInterpreter();
    }
  }

  if (isButtonPressed) {
    agrumino.turnWateringOn();
    delay(2000);
    agrumino.turnWateringOff();
  }



  if (ok_start == 1) {

    if (!client.connected()) reconnect();
    client.loop();
    pub_all();
    delay(2000);
  }

  agrumino.turnBoardOff(); // Board off before delay/sleep to save battery :)

  if (st->sleep_mode == 'd') {
    delaySec(interpreter.get_config()->sleep_time_sec); // The ESP8266 stays powered, executes the loop repeatedly
  } else {

    agrumino.deepSleepSec(interpreter.get_config()->sleep_time_sec); // ESP8266 enter in deepSleep and after the selected time starts back from setup() and then loop()
  }
}

/////////////////////
// Utility methods //
/////////////////////

void blinkLed() {
  agrumino.turnLedOn();
  delay(200);
  agrumino.turnLedOff();
}

void delaySec(int sec) {
  delay (sec * 1000);
}

void pub_soil() {
  char  p[50];
  strcpy (p, "Soil moisture: ");
  int soil = agrumino.readSoil();
  char s[10];
  dtostrf(soil, 9, 0, s);
  strcat(p, s);
  client.publish(channel_pub, p);
}

void pub_all() {

  pub_soil();

  char  p[50];
  char s[10];

  dtostrf(agrumino.readTempC(), 9, 0, s);
  strcpy (p, "Temperature: ");
  strcat(p, s);
  client.publish(channel_pub, p);

  dtostrf(agrumino.readBatteryLevel(), 9, 0, s);
  strcpy (p, "Battery level: ");
  strcat(p, s);
  client.publish(channel_pub, p);

}


