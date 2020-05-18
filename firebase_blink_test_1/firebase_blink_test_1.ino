#include <WiFi.h>
#include "FirebaseESP32.h"



#define FIREBASE_HOST "xxx.firebaseio.com" //Change to your Firebase RTDB project ID e.g. Your_Project_ID.firebaseio.com
#define FIREBASE_AUTH "xxx" //Change to your Firebase RTDB secret password
#define WIFI_SSID "xxxx"
#define WIFI_PASSWORD "xxxx"


//Define Firebase Data objects
FirebaseData firebaseData1;

bool swState = false;
void streamCallback(StreamData data)
{

  if (data.dataType() == "boolean") {
    if (data.boolData())
      Serial.println("Set  to High");
    else
      Serial.println("Set to Low");
    digitalWrite(LED_BUILTIN, data.boolData());
  }


}


void streamTimeoutCallback(bool timeout)
{
  if (timeout)
  {
    Serial.println();
    Serial.println("Stream timeout, resume streaming...");
    Serial.println();
  }
}


void setup()
{

  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println();
  Serial.println();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  if (!Firebase.beginStream(firebaseData1, "start"))
  {
    Serial.println("Could not begin stream");
    Serial.println("REASON: " + firebaseData1.errorReason());
    Serial.println();
  }
  Firebase.setStreamCallback(firebaseData1, streamCallback, streamTimeoutCallback);
}
void getStartState() {}
void loop()
{
  if (Firebase.getBool(firebaseData1, "/start")) {
    Serial.println("start state: ");
    Serial.println( firebaseData1.boolData());

  } else {
    Serial.println("start error state: " );
    Serial.println( firebaseData1.errorReason());
  }
}
