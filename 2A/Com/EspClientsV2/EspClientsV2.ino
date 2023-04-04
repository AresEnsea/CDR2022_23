#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti WiFiMulti;

void setup() {

  Serial.begin(115200);
  Serial2.begin(2000000);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
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
  const char* host = "10.20.1.1";  // ip or dns

  Serial.print("Connecting to ");
  Serial.println(host);

  WiFiClient client;

  if(!client.connect(host, port)) {
    delay(10);
  }else{
    int printSelect=0;
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Connection succeeded.");
    while(client.connected() > 0){
      if (client.available() > 0) {            // Returns the number of bytes available for reading
        uint8_t WifiDataRX = client.read();  // Read the next byte recieved from the server the client is connected to
        if(WifiDataRX==0x7E){
          printSelect=0;
        }
        //Serial.print("From client: ");
        //Serial.println(WifiDataRX);
        Serial2.write(WifiDataRX);
      }
      if ((Serial2.available() > 0)&&(printSelect==0)) {
        uint8_t WifiDataTX = Serial2.read();
        if(WifiDataTX==0x7F){
          printSelect=1;
          Serial.print("in debug\n");
        }
        //Serial.print("From STM: ");
        //Serial.println(WifiDataTX);
        client.write(WifiDataTX);
      }else if((Serial2.available() > 0)&&(printSelect==1)){
        String WifiDataTXstring = Serial2.readStringUntil('\n');
        //Serial.println(WifiDataTXstring);
        if(WifiDataTXstring[0]==0x7E){
          printSelect=0;
          Serial.print("out of debug\n");
        }
        client.println(WifiDataTXstring);
      }
    }
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Connection closed.");
    client.stop();
  }
}