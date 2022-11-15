#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti WiFiMulti;

void setup() {

  Serial.begin(115200);
  Serial2.begin(115200);
  // We start by connecting to a WiFi network
  //WiFiMulti.addAP("Ares", "AR30AS08//1998");
  //WiFiMulti.addAP("Livebox-59f0", "CA2D973D94C2CC543A627AF93F");
  WiFiMulti.addAP("porchetta", "guillaume");

  Serial.print("\nWaiting for WiFi...");

  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nWiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void loop() {
  const uint16_t port = 25565;
  const char* host = "10.20.1.1";  // ip or dns

  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  if (!client.connect(host, port)) {
    Serial.println("Connection failed.");
    Serial.println("Waiting 5 seconds before retrying...");
    delay(5000);
    return;
  }

  // This will send a request to the server
  //uncomment this line to send an arbitrary string to the server
  //client.print("Send this data to the server");
  //uncomment this line to send a basic document request to the server
  int maxloops = 0;

  //wait for the server's reply to become available
  while (!client.available() && maxloops < 500) {
    maxloops++;
  }
  if (client.available()) {  // Returns the number of bytes available for reading
    String line = client.readStringUntil('\r');
    Serial2.print(line);
    Serial.print("from client :     ");
    Serial.println(line);
  }
  if (Serial2.available() > 0) {
    String msg = Serial2.readStringUntil('\n');
    client.print(msg);
    Serial.print("from Nucleo : ");
    Serial.println(msg);
  }
  client.stop();
}