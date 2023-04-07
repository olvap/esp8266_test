#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid     = "OLDANI";
const char* password = "CLAROP5DA";
const char* host = "alarmx.up.railway.app";
//const char* host = "192.168.100.9:3000";
const char* token = "e2b8f9dcf908c334";

int buttonPin = 4; //GPIO4; //D2; // Pin digital conectado al botón
bool buttonState = false; // Estado actual del botón

void sendState(bool state, WiFiClientSecure client) {
  if (!client.connected()) {
    if (!client.connect(host, 443)) {
      Serial.println("Connection failed");
      return;
    }
  }
  // Create HTTP request
  String url = "/api/emitters/1111/1";
  String body = "{\"sensor\": {\"state\": " + String(state) + "}}";
  Serial.print("Desde la funcion: ");
  Serial.print("PUT ");
  Serial.print(url);
  Serial.println(" HTTP/1.1");
  client.println(String("PUT ") + url + " HTTP/1.1");
  Serial.print("Host: ");
  Serial.println(host);
  client.println("Host: " + String(host));
  client.println("Content-Type: application/json");
  client.println("Authorization: Bearer " + String(token));
  client.println("Content-Length: " + String(body.length()));
  client.println();
  client.println(body);

  Serial.println("Request sent");
}

void setup() {
  Serial.begin(9600);
  delay(10);

  // Configurar pin del botón como entrada
  pinMode(buttonPin, INPUT);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
  client.setInsecure();

  // check if a message has been received over serial
  if (Serial.available() > 0) {
    String message = Serial.readStringUntil('\n');
    Serial.print("Message received: ");
    Serial.println(message);

    Serial.print("Connecting to ");
    Serial.println(host);

    if (!client.connect(host, 443)) {
      Serial.println("Connection failed");
      return;
    }

    // send the message received over serial
    sendState(message.toInt(), client);
  }
  bool newButtonState = digitalRead(buttonPin);

  // Detectar cambios en el estado del botón
  if (newButtonState != buttonState) {
    buttonState = newButtonState;
    sendState(buttonState, client);
  }

  delay(50);
}
