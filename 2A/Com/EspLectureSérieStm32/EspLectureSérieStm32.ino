
void setup() {
  // put your setup code here, to run once
  Serial2.begin(115200);
  Serial2.setTimeout(100);
  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial2.available() > 0) {
    // read the incoming byte:
    String incomingByte = Serial2.readString();

    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte);
  }
}
