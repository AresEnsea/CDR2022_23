#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti WiFiMulti;

void setup() {

  Serial.begin(115200);
  Serial2.begin(115200);
  //WiFiMulti.addAP("Ares", "AR30AS08//1998");
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
  const char* host = "192.168.137.1";  // ip or dns

  Serial.print("Connecting to ");
  Serial.println(host);

  WiFiClient client;

  if(!client.connect(host, port)) {
    Serial.println("Connection failed.");
    Serial.println("Waiting 5 seconds before retrying...");
    delay(5000);
  }else{
    Serial.println("Connection succeeded.");
    while(client.connected() > 0){
      if (client.available() > 0) {            // Returns the number of bytes available for reading
        uint8_t WifiDataRX = client.read();  // Read the next byte recieved from the server the client is connected to
        //Serial.print("From client: ");
        //Serial.println(WifiDataRX);
        Serial2.write(WifiDataRX);
      }
      if (Serial2.available() > 0) {
        uint8_t WifiDataTX = Serial2.read();
        //Serial.print("From STM: ");
        //Serial.println(WifiDataTX);
        client.write(WifiDataTX);
      }
    }
    Serial.println("Connection closed.");
    client.stop();
  }
}