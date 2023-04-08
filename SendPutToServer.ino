#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "secrets.h"
#include "pin_layout.h"
#include "send_state.hpp"
#include "ESP8266_Utils.hpp"


void setup() {
  Serial.begin(9600);
  delay(10);

  // Configurar pin del botón como entrada
  pinMode(buttonPin, INPUT);

  ConnectWiFi_STA();
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
