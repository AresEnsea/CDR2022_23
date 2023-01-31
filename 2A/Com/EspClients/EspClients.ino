#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti WiFiMulti;

void setup() {

  Serial.begin(115200);
  Serial2.begin(115200);
  // We start by connecting to a WiFi network
  //WiFiMulti.addAP("Ares", "AR30AS08//1998");
  WiFiMulti.addAP("Livebox-59f0", "CA2D973D94C2CC543A627AF93F");

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
  const char* host = "192.168.1.12";  // ip or dns

  Serial.print("Connecting to ");
  Serial.println(host);

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
  while (!client.available() && maxloops < 1000) {
    maxloops++;
    delay(1);  //delay 1 msec
  }
  if (client.available()) {            // Returns the number of bytes available for reading
    int incomingByte = client.read();  // Read the next byte recieved from the server the client is connected to
    Serial.print("From client: \"");
    while (client.available())
      Serial.print((char)client.read());
    Serial.println("\"");
  } else {
    Serial.println("client.available() timed out ");
  }
  if (Serial2.available() > 0) {
    String msg = Serial2.readStringUntil('\n');
    client.print(msg);
  }
  Serial.println("Closing connection.");
  client.stop();
}