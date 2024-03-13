#include <ArduinoJson.h>
#include <SPI.h>
#include <RF24.h>

#define CE_PIN 9
#define CSN_PIN 10

RF24 radio(CE_PIN, CSN_PIN);

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.startListening();
  radio.openReadingPipe(1, 0xF0F0F0E1E1LL);
}

void loop() {
  char receivedData[13] = "";

  if (radio.available()) {
    radio.read(&receivedData, sizeof(receivedData));

    Serial.println("Received Data: " + String(receivedData));
    String data = String(receivedData);

    int enter = data.substring(0, 2).toInt();
    int exit = data.substring(2, 4).toInt();

    int totalNum = 0;
    String sign = data.substring(4, 5);
    if (sign == "-"){
      totalNum -= data.substring(5, 7).toInt();
    } else {
      totalNum += data.substring(5, 7).toInt();
    }
    int numStop = data.substring(7, 9).toInt();
    int numCycle = data.substring(9, 10).toInt();
    int numBus = data.substring(10, 12).toInt();

    // Create a JSON object
    StaticJsonDocument<128> doc;
    doc["Enter"] = enter;
    doc["Exit"] = exit;
    doc["Total"] = totalNum;
    doc["Stop"] = numStop;
    doc["Cycle"] = numCycle;
    doc["Bus"] = numBus;

    // Serialize the JSON object to a string
    String jsonString;
    serializeJson(doc, jsonString);

    Serial.println("JSON Data: " + jsonString);
  }
}
