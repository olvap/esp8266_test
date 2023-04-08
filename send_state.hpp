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
